#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define MAX_COMMAND_ARRAY_SIZE 5

typedef bool (*command_handler_t)(int argc, char* argv[]);

typedef struct command_argument {
    const char* name;
    const char* description;
    const bool required;
} command_argument_t;

typedef struct command {
    const char* name;
    const char* aliases[MAX_COMMAND_ARRAY_SIZE];
    const char* description;
    const command_argument_t arguments[MAX_COMMAND_ARRAY_SIZE];
    const command_handler_t handler;
} command_t;

bool process_command(int argc, char* argv[]);
