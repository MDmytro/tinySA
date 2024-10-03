#ifndef VNA_H
#define VNA_H

#include <stdint.h>
#include "nanovna.h"

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
      void command_name(int argc, char *argv[])

// Shell command line buffer, args, nargs, and function ptr
extern char shell_line[VNA_SHELL_MAX_LENGTH];
extern char *shell_args[VNA_SHELL_MAX_ARGUMENTS + 1];
extern uint16_t shell_nargs;
extern volatile vna_shellcmd_t shell_function;

// Info about NanoVNA, need fore soft
#define ENABLE_INFO_COMMAND

extern freq_t frequencyStart;
extern freq_t frequencyStop;

void set_frequencies(freq_t start, freq_t stop, uint16_t points);
int set_frequency(freq_t freq);

extern uint16_t sweep_once_count;

#endif
