#include "commands.h"

#include <stdio.h>
#include <string.h>

static const command_t COMMANDS[];

static command_status_t handle_help(int argc, char* argv[]) {
    printf("9xget - A Windows 95/98 package manager\n");
    printf("Commands:\n");

    for (int i = 0; ; i++) {
        command_t cmd = COMMANDS[i];

        if (cmd.name == NULL) break;

        printf("\t9xget %s", cmd.name);

        for (int j = 0; j < MAX_COMMAND_ARRAY_SIZE; j++) {
            command_argument_t arg = cmd.arguments[j];
            
            if (arg.name == NULL) break;

            if (j != 0) printf(",");

            if (arg.required) {
                printf(" <%s>", arg.name);
            } else {
                printf(" [%s]", arg.name);
            }
        }

        printf("\n");
        printf("\t\t %s\n", cmd.description);
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
    
    for (int i = 0; ; i++) {
        command_t cmd = COMMANDS[i];

        if (cmd.name == NULL) return ERROR_UNKNOWN_COMMAND;

        if (strcmpi(cmd.name, argv[1]) == 0) {
            return cmd.handler(argc, argv);
        } else {
            for (int j = 0; j < MAX_COMMAND_ARRAY_SIZE; j++) {
                const char* alias = cmd.aliases[j];

                if (alias == NULL) break;

                if (strcmpi(alias, argv[1]) == 0) {
                    return cmd.handler(argc, argv);
                } 
            }
        }
    }

    return ERROR_UNKNOWN;
}
