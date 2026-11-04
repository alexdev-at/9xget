#include "commands.h"
#include "doctor.h"

#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (!run_system_check()) {
        sprintf("System check failed, aborting execution");
        return EXIT_FAILURE;
    }

    if (!process_command(argc, argv)) {
        sprintf("Command failed, aborting execution");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
