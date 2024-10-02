#include "vna.h"

char shell_line[VNA_SHELL_MAX_LENGTH];
char *shell_args[VNA_SHELL_MAX_ARGUMENTS + 1];
uint16_t shell_nargs;