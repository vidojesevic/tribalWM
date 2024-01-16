#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void die(const char *message) {
    fprintf(stderr, "%s", message);
    exit(EXIT_FAILURE);
}

