#include "commands.h"

int main(int argc, char* argv[]) {
    return process_command(argc, argv) ? 0 : 1;
}
