#include "deep_fetch.h"
#include <dirent.h>

void print_username(void) {
    struct passwd *pw = getpwuid(getuid());
    print_line("User", pw ? pw->pw_name : "unknown");
}

void print_hostname(void) {
    char hostname[HOST_NAME_MAX];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        hostname[HOST_NAME_MAX - 1] = '\0';
        print_line("Host", hostname);
    } else {
        print_line("Host", "unknown");
    }
}

void print_os_info(void) {
    FILE *f = fopen("/etc/os-release", "r");
    if (!f) {
        print_line("OS", "Unknown Linux");
        return;
    }

    char line[256];
    char *pretty_name = NULL;

    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
            pretty_name = line + 12;
            char *end = strchr(pretty_name, '"');
            if (end) *end = '\0';
            else pretty_name[strcspn(pretty_name, "\n")] = '\0';
            break;
        }
    }
    fclose(f);

    print_line("OS", pretty_name ? pretty_name : "Linux");
}

void print_kernel(void) {
    struct utsname buf;
    uname(&buf);
    print_line("Kernel", buf.release);
}

void print_uptime(void) {
    struct sysinfo si;
    sysinfo(&si);

    long seconds = si.uptime;
    int days = (int)(seconds / 86400);
    int hours = (int)((seconds % 86400) / 3600);
    int minutes = (int)((seconds % 3600) / 60);

    if (days > 0)
        printf("%sUp%s: %dd %dh %dm\n", PURPLE, RESET, days, hours, minutes);
    else if (hours > 0)
        printf("%sUp%s: %dh %dm\n", PURPLE, RESET, hours, minutes);
    else
        printf("%sUp%s: %ldm\n", PURPLE, RESET, (long)(seconds / 60));
}

void print_shell(void) {
    const char *shell = getenv("SHELL");
    char name[256];
    if (shell) {
        const char *slash = strrchr(shell, '/');
        strncpy(name, slash ? slash + 1 : shell, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    } else {
        strcpy(name, "unknown");
    }
    print_line("Shell", name);
}

void print_packages(void) {
    /* Try to count packages via the system's package manager */
    FILE *f = popen("dpkg --list 2>/dev/null | grep -c '^ii' || rpm -qa 2>/dev/null | wc -l || pacman -Q 2>/dev/null | wc -l", "r");
    if (f) {
        char buf[64];
        if (fgets(buf, sizeof(buf), f)) {
            int count = atoi(buf);
            if (count > 0) {
                printf("%sPackages%s: %d\n", PURPLE, RESET, count);
                pclose(f);
                return;
            }
        }
        pclose(f);
    }
    print_line("Packages", "Unknown");
}

void print_de(void) {
    const char *desktop = getenv("XDG_CURRENT_DESKTOP");
    if (!desktop) desktop = getenv("DESKTOP_SESSION");
    print_line("DE", desktop ? desktop : "None / Tiling WM");
}

void print_terminal(void) {
    const char *term = getenv("TERM");
    print_line("Terminal", term ? term : "unknown");
}
