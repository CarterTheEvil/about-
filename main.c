#include "deep_fetch.h"

int main(void) {
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
