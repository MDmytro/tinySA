#ifndef VNA_H
#define VNA_H

#include "ch.h"
#include "hal.h"

// Shell new line
#define VNA_SHELL_NEWLINE_STR    "\r\n"
// Shell command promt
#define VNA_SHELL_PROMPT_STR     "ch> "
// Shell max arguments
#define VNA_SHELL_MAX_ARGUMENTS   4
// Shell max command line size
#define VNA_SHELL_MAX_LENGTH     48

// Shell command functions prototypes
typedef void (*vna_shellcmd_t)(int argc, char *argv[]);
#define VNA_SHELL_FUNCTION(command_name) \
      static void command_name(int argc, char *argv[])

// Shell command line buffer, args, nargs, and function ptr
char shell_line[VNA_SHELL_MAX_LENGTH];
char *shell_args[VNA_SHELL_MAX_ARGUMENTS + 1];
uint16_t shell_nargs;
static volatile vna_shellcmd_t shell_function = 0;

#endif
