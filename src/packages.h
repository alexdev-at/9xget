#pragma once

#include <stdbool.h>

typedef struct index_package {
    char id[64];
    char description[256];
    char win95[32];
    char win98[32];
} index_package_t;

bool find_index_package(char* package_name, index_package_t* out_index_package);
