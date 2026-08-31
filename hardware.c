#include "deep_fetch.h"
#include <dirent.h>

void print_cpu(void) {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) {
        print_line("CPU", "Unknown");
        return;
    }

    char line[512];
    char *cpu_model = NULL;
    int cores = 0;

    while (fgets(line, sizeof(line), f)) {
        if (!cpu_model && strncmp(line, "model name", 10) == 0) {
            cpu_model = strchr(line, ':');
            if (cpu_model) cpu_model++;
            while (*cpu_model == ' ') cpu_model++;
            cpu_model[strcspn(cpu_model, "\n")] = '\0';
        }
        if (strncmp(line, "processor", 9) == 0) {
            cores++;
        }
    }
    fclose(f);

    char buf[512];
    if (cpu_model)
        snprintf(buf, sizeof(buf), "%s (%d threads)", cpu_model, cores);
    else
        snprintf(buf, sizeof(buf), "Unknown (%d threads)", cores);

    print_line("CPU", buf);
}

void print_gpu(void) {
    FILE *f = popen("lspci 2>/dev/null | grep -iE 'VGA|3D' | head -1", "r");
    if (f) {
        char line[256];
        if (fgets(line, sizeof(line), f)) {
            char *gpu = strchr(line, ':');
            if (gpu) gpu++;
            while (*gpu == ' ') gpu++;
            gpu[strcspn(gpu, "\n")] = '\0';
            pclose(f);
            print_line("GPU", gpu);
            return;
        }
        pclose(f);
    }
    print_line("GPU", "Integrated / Unknown");
}

void print_ram(void) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) {
        print_line("RAM", "Unknown");
        return;
    }

    char line[256];
    unsigned long mem_total = 0, mem_avail = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "MemTotal:", 9) == 0)
            sscanf(line + 9, "%lu", &mem_total);
        else if (strncmp(line, "MemAvailable:", 13) == 0)
            sscanf(line + 13, "%lu", &mem_avail);
    }
    fclose(f);

    float total_gb = mem_total / 1048576.0f;
    float used_gb = (mem_total - mem_avail) / 1048576.0f;
    float pct = total_gb > 0 ? (used_gb / total_gb) * 100.0f : 0.0f;

    char buf[128];
    snprintf(buf, sizeof(buf), "%.1f GiB / %.1f GiB (%.0f%%)", used_gb, total_gb, pct);
    print_line("RAM", buf);
}

void print_swap(void) {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) {
        print_line("Swap", "Unknown");
        return;
    }

    char line[256];
    unsigned long swap_total = 0, swap_free = 0;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "SwapTotal:", 10) == 0)
            sscanf(line + 10, "%lu", &swap_total);
        else if (strncmp(line, "SwapFree:", 9) == 0)
            sscanf(line + 9, "%lu", &swap_free);
    }
    fclose(f);

    if (swap_total == 0) {
        print_line("Swap", "Disabled");
    } else {
        float total_gb = swap_total / 1048576.0f;
        float used_gb = (swap_total - swap_free) / 1048576.0f;
        char buf[128];
        snprintf(buf, sizeof(buf), "%.1f GiB / %.1f GiB", used_gb, total_gb);
        print_line("Swap", buf);
    }
}

void print_disk(void) {
    FILE *f = popen("df -h / 2>/dev/null | awk 'NR==2 {print $2\" used of \"$4\" free (\"$5\")\"}'", "r");
    if (f) {
        char buf[256];
        if (fgets(buf, sizeof(buf), f)) {
            buf[strcspn(buf, "\n")] = '\0';
            pclose(f);
            print_line("Disk (/)", buf);
            return;
        }
        pclose(f);
    }
    print_line("Disk (/)", "Unknown");
}

void print_locale(void) {
    const char *lang = getenv("LANG");
    print_line("Locale", lang ? lang : "C.UTF-8");
}
