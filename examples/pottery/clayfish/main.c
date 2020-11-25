#include <string.h>
#include "cli.h"
#include "uci.h"

int main(int argc, char** argv) {

    // If -u, launch the UCI interface instead of the CLI interface.
    if (argc == 2 && 0 == strcmp(argv[1], "-u"))
        return uci_main();

    return cli_main();

}
