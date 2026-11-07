#include "packages.h"
#include "paths.h"
#include "ini.h"
#include "doctor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef struct index_find_ctx {
    char* target_name;
    index_package_t* out_pkg;
    bool found;
} index_find_ctx_t;

static int index_find_handler(void* user, const char* section, const char* name, const char* value) {
    index_find_ctx_t* ctx = (index_find_ctx_t*) user;

    if (strcmpi(section, ctx->target_name) != 0) {
        if (ctx->found) return 0;
        return 1;
    }

    if (!ctx->found) {
        ctx->found = true;
        strncpy(ctx->out_pkg->id, section, sizeof(ctx->out_pkg->id) - 1);
    }

    if (strcmpi(name, "description") == 0) {
        strncpy(ctx->out_pkg->description, value, sizeof(ctx->out_pkg->description) - 1);
    } else {
        const char* os_key = doctor_result.os_version == WINDOWS_95 ? "win95" : doctor_result.os_version == WINDOWS_98 ? "win98" : NULL;
        if (os_key != NULL && strcmpi(name, os_key) == 0) {
            strncpy(ctx->out_pkg->version, value, sizeof(ctx->out_pkg->version) - 1);
        } else if (strcmpi(name, "version") == 0 && ctx->out_pkg->version[0] == '\0') {
            strncpy(ctx->out_pkg->version, value, sizeof(ctx->out_pkg->version) - 1);
        }
    }

    return 1;
}

bool find_index_package(char* package_name, index_package_t* out_index_package) {
    char index_path[MAX_PATH];
    if (!path_get_index_path(index_path, sizeof(index_path))) {
        fprintf(stderr, "Error: Could not determine index path\n");
        return false;
    }

    FILE* fp = fopen(index_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Package index does not exist. Run '9xget update' first\n");
        return false;
    }

    memset(out_index_package, 0, sizeof(index_package_t));

    index_find_ctx_t ctx;
    ctx.target_name = package_name;
    ctx.out_pkg = out_index_package;
    ctx.found = false;

    ini_parse_file(fp, index_find_handler, &ctx);
    fclose(fp);

    if (!ctx.found) {
        fprintf(stderr, "Error: Package '%s' not found in package index. Run '9xget update' to see if you have an outdated index file\n", package_name);
        return false;
    }

    return true;
}

static void parse_file_section(package_file_t* file, const char* name, const char* value) {
    if (strcmpi(name, "version") == 0) {
        strncpy(file->version, value, sizeof(file->version) - 1);
    } else if (strcmpi(name, "url") == 0) {
        strncpy(file->url, value, sizeof(file->url) - 1);
    } else if (strcmpi(name, "mirror") == 0) {
        strncpy(file->mirror, value, sizeof(file->mirror) - 1);
    } else if (strcmpi(name, "main") == 0) {
        strncpy(file->main, value, sizeof(file->main) - 1);
    } else if (strcmpi(name, "size") == 0) {
        file->size = (unsigned long) strtoul(value, NULL, 10);
    } else if (strcmpi(name, "sha256") == 0) {
        strncpy(file->sha256, value, sizeof(file->sha256) - 1);
    } else if (strcmpi(name, "type") == 0) {
        strncpy(file->type, value, sizeof(file->type) - 1);
    } else if (strcmpi(name, "format") == 0) {
        strncpy(file->format, value, sizeof(file->format) - 1);
    } else if (strcmpi(name, "kind") == 0) {
        strncpy(file->kind, value, sizeof(file->kind) - 1);
    } else if (strcmpi(name, "reboot") == 0) {
        file->reboot = (strcmpi(value, "true") == 0 || strcmp(value, "1") == 0);
    }
}

static int package_parse_handler(void* user, const char* section, const char* name, const char* value) {
    package_t* pkg = (package_t*) user;
    const char* os_key = doctor_result.os_version == WINDOWS_95 ? "win95" : doctor_result.os_version == WINDOWS_98 ? "win98" : NULL;

    if (strcmpi(section, "package") == 0) {
        if (strcmpi(name, "id") == 0) {
            strncpy(pkg->id, value, sizeof(pkg->id) - 1);
        } else if (strcmpi(name, "name") == 0) {
            strncpy(pkg->name, value, sizeof(pkg->name) - 1);
        } else if (strcmpi(name, "description") == 0) {
            strncpy(pkg->description, value, sizeof(pkg->description) - 1);
        }
        return 1;
    }

    char dep_section[64] = "";
    char file_section[128] = "";
    if (os_key != NULL) {
        snprintf(dep_section, sizeof(dep_section), "dependencies:%s", os_key);
        snprintf(file_section, sizeof(file_section), "file:%s", os_key);
    }

    if (strcmpi(section, "dependencies") == 0 || (dep_section[0] && strcmpi(section, dep_section) == 0)) {
        if (strcmpi(name, "packages") == 0) {
            strncpy(pkg->dependencies, value, sizeof(pkg->dependencies) - 1);
        }
        return 1;
    }

    if (strcmpi(section, "file") == 0 || (file_section[0] && strcmpi(section, file_section) == 0)) {
        parse_file_section(&pkg->file, name, value);
        return 1;
    }

    return 1;
}

bool load_package(const char* package_name, package_t* out_package) {
    char packages_dir[MAX_PATH];
    if (!path_get_packages_dir(packages_dir, sizeof(packages_dir))) {
        fprintf(stderr, "Error: Could not determine packages directory\n");
        return false;
    }

    char file_path[MAX_PATH];
    snprintf(file_path, sizeof(file_path), "%s\\%s.ini", packages_dir, package_name);

    FILE* fp = fopen(file_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Package definition '%s.ini' not found\n", package_name);
        return false;
    }

    memset(out_package, 0, sizeof(package_t));

    ini_parse_file(fp, package_parse_handler, out_package);
    fclose(fp);

    return true;
}
