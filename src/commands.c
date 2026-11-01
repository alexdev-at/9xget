#include "commands.h"

#include <stdio.h>
#include <string.h>

static const command_t COMMANDS[];

static command_status_t handle_help(int argc, char* argv[]) {
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

    return SUCCESS;
}

static command_status_t handle_update(int argc, char* argv[]) {
    return SUCCESS;
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

command_status_t process_command(int argc, char* argv[]) {
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

    return ERROR_UNKNOWN;
}
