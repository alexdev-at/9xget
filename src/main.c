#include "commands.h"
#include "doctor.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (!run_system_check()) {
        fprintf(stderr, "System check failed, aborting execution");
        return EXIT_FAILURE;
    }

    if (!process_command(argc, argv)) {
        fprintf(stderr, "Command failed, aborting execution");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
