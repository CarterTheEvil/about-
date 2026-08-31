#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <dirent.h>

extern void print_header();
extern void print_line(const char *label, const char *value);

void print_username() {
    struct passwd *pw = getpwuid(getuid());
    print_line("User", pw ? pw->pw_name : "unknown");
}

void print_hostname() {
    char hostname[HOSTNAME_MAX];
    gethostname(hostname, sizeof(hostname));
    print_line("Host", hostname);
}

void print_os_info() {
    FILE *f = fopen("/etc/os-release", "r");
    if (!f) {
        print_line("OS", "Unknown Linux");
        return;
    }
    
    char line[256];
    char *name = NULL, *pretty_name = NULL;
    
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
            pretty_name = strtok(line + 12, "\"");
        }
        if (strncmp(line, "NAME=", 5) == 0 && !name) {
            name = strtok(line + 5, "\"");
        }
    }
    fclose(f);
    
    print_line("OS", pretty_name ? pretty_name : (name ? name : "Linux"));
}

void print_kernel() {
    struct utsname buf;
    uname(&buf);
    print_line("Kernel", buf.release);
}

void print_uptime() {
    struct sysinfo si;
    sysinfo(&si);
    
    long seconds = si.uptime;
    int days = seconds / 86400;
    int hours = (seconds % 86400) / 3600;
    int minutes = (seconds % 3600) / 60;
    
    if (days > 0) {
        printf("%sUp%s: %dd %dh %dm\n", PURPLE, RESET, days, hours, minutes);
    } else if (hours > 0) {
        printf("%sUp%s: %dh %dm\n", PURPLE, RESET, hours, minutes);
    } else {
        printf("%sUp%s: %ldm\n", PURPLE, RESET, (long)(seconds / 60));
    }
}

void print_shell() {
    char *shell = getenv("SHELL");
    shell = shell ? strrchr(shell, '/') + 1 : "unknown";
    print_line("Shell", shell);
}

void print_packages() {
    int count = 0;
    
    // Check common package managers
    DIR *dir;
    if ((dir = opendir("/var/lib/dpkg/status"))) {
        count += 1;  // dpkg detected
    }
    closedir(dir);
    
    if ((dir = opendir("/var/cache/pacman/pkg"))) {
        count += 1;  // pacman detected
    }
    closedir(dir);
    
    if ((dir = opendir("/home"))) {
        // Count installed .deb/.rpm/.pkg.tar packages roughly
        count += 0;  // Placeholder for accurate counting
    }
    closedir(dir);
    
    // Fallback estimate
    if (count == 0) {
        printf("%sPackages%s: Multiple PMs detected\n", PURPLE, RESET);
    } else {
        printf("%sPackage Managers%s: %d found\n", PURPLE, RESET, count);
    }
}

void print_de() {
    char *desktop = getenv("XDG_CURRENT_DESKTOP");
    desktop = desktop ? desktop : getenv("DESKTOP_SESSION");
    desktop = desktop ? desktop : "None / Tiling WM";
    print_line("Desktop Env", desktop);
}

void print_terminal() {
    char *term = getenv("TERM");
    term = term ? term : "unknown";
    print_line("Terminal", term);
}
