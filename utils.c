#include <stdio.h>

void print_header() {
    // Simple ASCII art logo
    printf(PURPLE BOLD);
    printf("       _               _         _          \n");
    printf("      | |             | |       (_)         \n");
    printf("    __| |_   ___ __ __| | _____  _  ___  ___ \n");
    printf("   / _` \\ \\ / / '__/ _` |/ / _ \\| |/ _ \\/ __|\n");
    printf("  | (_| |\\ V /| | | (_| |   < (_) | |  __/ (__ \n");
    printf("   \\__,_| \\_/ |_|  \\__,_|_|\\_\\___/|_|\\___|\\___|\n");
    printf(RESET);
    printf("\n\n");
}

void print_line(const char *label, const char *value) {
    printf("%s%s%s: %s\n", PURPLE, label, RESET, value);
}
