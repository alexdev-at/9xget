#include "packages.h"
#include "paths.h"
#include "ini.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

typedef struct index_find_ctx {
    char* target_name;
    index_package_t* out_pkg;
    bool found;
} index_find_ctx_t;

static int index_find_handler(void* user, const char* section, const char* name, const char* value) {
    index_find_ctx_t* ctx = (index_find_ctx_t*)user;

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
    } else if (strcmpi(name, "win95") == 0) {
        strncpy(ctx->out_pkg->win95, value, sizeof(ctx->out_pkg->win95) - 1);
    } else if (strcmpi(name, "win98") == 0) {
        strncpy(ctx->out_pkg->win98, value, sizeof(ctx->out_pkg->win98) - 1);
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
