#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <time.h>

// Color codes
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define PURPLE  "\033[38;5;129m"
#define WHITE   "\033[37m"
#define CYAN    "\033[36m"

void print_header();
void print_username();
void print_hostname();
void print_os_info();
void print_kernel();
void print_uptime();
void print_shell();
void print_packages();
void print_de();
void print_terminal();
void print_cpu();
void print_gpu();
void print_ram();
void print_swap();
void print_disk();
void print_locale();

int main(int argc, char *argv[]) {
    print_header();
    print_username();
    print_hostname();
    print_os_info();
    print_kernel();
    print_uptime();
    print_shell();
    print_packages();
    print_de();
    print_terminal();
    print_cpu();
    print_gpu();
    print_ram();
    print_swap();
    print_disk();
    print_locale();
    
    printf("\n");
    return 0;
}
