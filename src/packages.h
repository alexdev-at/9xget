#pragma once

#include <stdbool.h>

typedef struct index_package {
    char id[64];
    char description[256];
    char version[32];
} index_package_t;

typedef struct package_file {
    char version[32];
    char url[256];
    char mirror[256];
    char main[128];
    unsigned long size;
    char sha256[65];
    char type[32];
    char format[32];
    char kind[32];
    bool reboot;
} package_file_t;

typedef struct package {
    char id[64];
    char name[128];
    char description[256];
    char dependencies[256];
    package_file_t file;
} package_t;

bool find_index_package(char* package_name, index_package_t* out_index_package);
bool load_package(const char* package_name, package_t* out_package);

