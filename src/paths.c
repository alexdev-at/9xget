#include "paths.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

bool path_get_app_dir(char* out, size_t max_len) {
    if (GetModuleFileNameA(NULL, out, (DWORD)max_len) == 0) {
        fprintf(stderr, "Error: Could not locate 9xget executable path\n");
        return false;
    }

    char* last_slash = strrchr(out, '\\');
    if (last_slash != NULL) {
        *last_slash = '\0';
    }

    return true;
}

bool path_get_packages_dir(char* out, size_t max_len) {
    char app_dir[MAX_PATH];
    if (!path_get_app_dir(app_dir, sizeof(app_dir))) {
        fprintf(stderr, "Error: Could not resolve app directory for packages\n");
        return false;
    }

    snprintf(out, max_len, "%s\\packages", app_dir);

    if (!CreateDirectoryA(out, NULL) && GetLastError() != ERROR_ALREADY_EXISTS) {
        fprintf(stderr, "Error: Failed to create packages directory '%s'\n", out);
        return false;
    }

    return true;
}

bool path_get_index_path(char* out, size_t max_len) {
    char packages_dir[MAX_PATH];
    if (!path_get_packages_dir(packages_dir, sizeof(packages_dir))) {
        fprintf(stderr, "Error: Could not resolve packages directory for index\n");
        return false;
    }

    snprintf(out, max_len, "%s\\index.ini", packages_dir);
    return true;
}

bool path_get_installed_path(char* out, size_t max_len) {
    char app_dir[MAX_PATH];
    if (!path_get_app_dir(app_dir, sizeof(app_dir))) {
        fprintf(stderr, "Error: Could not resolve app directory for installed list\n");
        return false;
    }

    snprintf(out, max_len, "%s\\installed.ini", app_dir);
    return true;
}
