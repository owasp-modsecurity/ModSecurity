#include <stdlib.h>
#include <stdio.h>
#include "standalone/api.h"

static void print_log(void* obj, int level, char* str)
{
    if (str != NULL) {
        puts(str);
    }
}

static void exit_with_error(const char* msg)
{
    puts(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        exit_with_error("Invalid number of command line arguments");
    }

    server_rec* modsec_server = modsecInit();
    if (modsec_server == NULL) {
        exit_with_error("Failed to create ModSecurity server structure");        
    }
    char hostname[] = "localhost";
    modsec_server->server_hostname = hostname;

    modsecSetLogHook(NULL, print_log);
    modsecStartConfig();
    directory_config *config = modsecGetDefaultConfig();
    if (config == NULL) {
        exit_with_error("Error creating default config");
    }

    const char *err = modsecProcessConfig(config, argv[1], NULL);
    if (err != NULL) {
        exit_with_error(err);
    }

    modsecTerminate();
    exit(EXIT_SUCCESS);
}
