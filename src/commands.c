#include "commands.h"
#include "net.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

#define PROXY_HOST "9xget.org-fpact.workers.dev"
#define PACKAGES_HOST_PATH "/https://raw.githubusercontent.com/alexdev-at/9xget/main/packages/"

static const command_t COMMANDS[];

static bool handle_help(int argc, char* argv[]) {
    printf("9xget - A Windows 95/98 package manager\n");
    printf("Commands:\n");

    for (const command_t* cmd = COMMANDS; cmd->name != NULL; cmd++) {
        printf("\t9xget %s", cmd->name);

        for (int i = 0; i < MAX_COMMAND_ARRAY_SIZE; i++) {
            const command_argument_t* arg = &cmd->arguments[i];
            
            if (arg->name == NULL) break;

            if (i != 0) printf(",");

            if (arg->required) {
                printf(" <%s>", arg->name);
            } else {
                printf(" [%s]", arg->name);
            }
        }

        printf("\n");
        printf("\t\t %s\n", cmd->description);
    }

    return true;
}

static bool handle_update(int argc, char* argv[]) {
    char current_dir[2048];

    if (GetModuleFileNameA(NULL, current_dir, sizeof(current_dir)) == 0) {
        fprintf(stderr, "Error: Could not locate 9xget executable path.\n");
        return false;
    }

    char* last_slash = strrchr(current_dir, '\\');
    if (last_slash != NULL) {
        *last_slash = '\0';
    }

    char packages_dir[2048];
    snprintf(packages_dir, sizeof(packages_dir), "%s\\packages", current_dir);
    CreateDirectoryA(packages_dir, NULL);

    char destination_path[2048];
    snprintf(destination_path, sizeof(destination_path), "%s\\index.ini", packages_dir);

    char remote_path[2048];
    snprintf(remote_path, sizeof(remote_path), "%sindex.ini", PACKAGES_HOST_PATH);

    printf("Fetching package index...\n");

    if (!net_download(PROXY_HOST, remote_path, destination_path)) {
        fprintf(stderr, "Error: Failed to download index\n");
        return false;
    }

    printf("Package index successfully updated.\n");
    return true;
}

static const command_t COMMANDS[] = {
    {
        "help",
        {"h", NULL},
        "Displays a general help message and all commands",
        {NULL},
        handle_help
    },
    {
        "update",
        {"up", NULL},
        "Updates the package index",
        {NULL},
        handle_update
    },
    {NULL}
};

bool process_command(int argc, char* argv[]) {
    if (argc < 2) return handle_help(argc, argv);
    
    for (const command_t* cmd = COMMANDS; cmd->name != NULL; cmd++) {
        if (strcmpi(cmd->name, argv[1]) == 0) {
            return cmd->handler(argc, argv);
        } else {
            for (int i = 0; i < MAX_COMMAND_ARRAY_SIZE; i++) {
                const char* alias = cmd->aliases[i];

                if (alias == NULL) break;

                if (strcmpi(alias, argv[1]) == 0) {
                    return cmd->handler(argc, argv);
                } 
            }
        }
    }

    fprintf(stderr, "Error: Unknown command '%s'\n", argv[1]);
    return false;
}
