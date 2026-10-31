#pragma once

#include <stdbool.h>
#include <stdlib.h>

#define MAX_COMMAND_ARRAY_SIZE 5

typedef enum command_status {
    SUCCESS,
    ERROR_INVALID_ARGUMENTS,
    ERROR_NO_INTERNET,
    ERROR_UNKNOWN_COMMAND,
    ERROR_UNKNOWN
} command_status_t;

typedef command_status_t (*command_handler_t)(int argc, char* argv[]);

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

command_status_t process_command(int argc, char* argv[]);
