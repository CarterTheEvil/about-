#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern void print_line(const char *label, const char *value);

void print_cpu() {
    FILE *f = fopen("/proc/cpuinfo", "r");
    if (!f) {
        print_line("CPU", "Unknown");
        return;
    }
    
    char line[512];
    char *cpu_model = NULL;
    int cores = 0;
    
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "model name", 10) == 0) {
            cpu_model = strchr(line, ':') + 2;
            cpu_model[strcspn(cpu_model, "\n")] = 0;
        }
        if (strncmp(line, "processor", 9) == 0) {
            cores++;
        }
    }
    fclose(f);
    
    if (cpu_model) {
        printf("%sCPU%s: %s (%d cores)\n", PURPLE, RESET, cpu_model, cores);
    } else {
        printf("%sCPU%s: Unknown (%d cores)\n", PURPLE, RESET, cores);
    }
}

void print_gpu() {
    #ifdef __linux__
    FILE *f = popen("lspci 2>/dev/null | grep -i 'VGA\\|3D'", "r");
    if (!f) {
        print_line("GPU", "Unknown");
        return;
    }
    
    char line[256];
    int first = 1;
    while (fgets(line, sizeof(line), f)) {
        char *gpu_name = strchr(line, ':');
        gpu_name = gpu_name ? gpu_name + 2 : line;
        gpu_name[strcspn(gpu_name, "\n")] = 0;
        
        if (first) {
            printf("%sGPU%s: %s\n", PURPLE, RESET, gpu_name);
            first = 0;
        }
    }
    pclose(f);
    
    if (first) {
        print_line("GPU", "Integrated / Unknown");
    }
    #else
    print_line("GPU", "Platform not supported");
    #endif
}

void print_ram() {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) {
        print_line("RAM", "Unknown");
        return;
    }
    
    char line[256];
    unsigned long mem_total = 0, mem_available = 0;
    
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "MemTotal:", 9) == 0) {
            sscanf(line + 9, "%lu", &mem_total);
        }
        if (strncmp(line, "MemAvailable:", 13) == 0) {
            sscanf(line + 13, "%lu", &mem_available);
        }
    }
    fclose(f);
    
    float total_gb = (float)mem_total / 1048576.0f;
    float used_gb = (total_gb - ((float)mem_available / 1048576.0f));
    float usage_pct = (used_gb / total_gb) * 100.0f;
    
    printf("%sRAM%s: %.1f GB / %.1f GB (%.0f%%)\n", 
           PURPLE, RESET, used_gb, total_gb, usage_pct);
}

void print_swap() {
    FILE *f = fopen("/proc/meminfo", "r");
    if (!f) {
        print_line("Swap", "Unknown");
        return;
    }
    
    char line[256];
    unsigned long swap_total = 0, swap_free = 0;
    
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "SwapTotal:", 10) == 0) {
            sscanf(line + 10, "%lu", &swap_total);
        }
        if (strncmp(line, "SwapFree:", 9) == 0) {
            sscanf(line + 9, "%lu", &swap_free);
        }
    }
    fclose(f);
    
    if (swap_total == 0) {
        printf("%sSwap%s: Disabled\n", PURPLE, RESET);
    } else {
        float total_gb = (float)swap_total / 1048576.0f;
        float used_gb = (total_gb - ((float)swap_free / 1048576.0f));
        printf("%sSwap%s: %.1f GB / %.1f GB\n", PURPLE, RESET, used_gb, total_gb);
    }
}

void print_disk() {
    FILE *f = popen("df -h / 2>/dev/null | tail -1", "r");
    if (!f) {
        print_line("Disk", "Unknown");
        return;
    }
    
    char line[256];
    char size[32], used[32], avail[32], mount[32];
    int pct;
    
    if (fscanf(f, "%*s %s %s %s %d%% %s", size, used, avail, &pct, mount) == 5) {
        printf("%sDisk%s: %s (%d%% used, mounted at %s)\n", 
               PURPLE, RESET, used, pct, mount);
    } else {
        print_line("Disk", "Unable to read");
    }
    pclose(f);
}

void print_locale() {
    char *lang = getenv("LANG");
    lang = lang ? lang : "C.UTF-8";
    
    char *locale = getenv("LC_ALL");
    locale = locale ? locale : (getenv("LC_CTYPE") ? getenv("LC_CTYPE") : lang);
    
    printf("%sLocale%s: %s (LANG: %s)\n", PURPLE, RESET, locale, lang);
}
