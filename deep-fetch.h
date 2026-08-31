#ifndef DEEP_FETCH_H
#define DEEP_FETCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <limits.h>

/* Colors */
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define PURPLE  "\033[38;5;129m"
#define WHITE   "\033[37m"
#define CYAN    "\033[36m"

/* Function declarations */
void print_header(void);
void print_line(const char *label, const char *value);

void print_username(void);
void print_hostname(void);
void print_os_info(void);
void print_kernel(void);
void print_uptime(void);
void print_shell(void);
void print_packages(void);
void print_de(void);
void print_terminal(void);

void print_cpu(void);
void print_gpu(void);
void print_ram(void);
void print_swap(void);
void print_disk(void);
void print_locale(void);

#endif
