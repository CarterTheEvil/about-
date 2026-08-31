#include "deep_fetch.h"

void print_header(void) {
    printf(PURPLE BOLD);
    printf("    _________     _____   _____  __ \n");
    printf("   /  _/  _ |   /  _  \\  \\   \\/  / \n");
    printf("   /   ( ` )/   |  \\_  /   \\     /  \n");
    printf("   \\   /  \\  \\  |   Y /     )   \\   \n");
    printf("    \\_/    \\__\\ |___|_\\    /_\\  \\_\\  \n");
    printf("\n\n");
    printf(RESET);
}

void print_line(const char *label, const char *value) {
    printf("%s%-14s%s %s\n", PURPLE, label, RESET, value);
}
