#include "vna.h"

char shell_line[VNA_SHELL_MAX_LENGTH];
char *shell_args[VNA_SHELL_MAX_ARGUMENTS + 1];
uint16_t shell_nargs;

freq_t frequencyStart;
freq_t frequencyStop;
int32_t frequencyExtra;

volatile vna_shellcmd_t shell_function = 0;

uint8_t sweep_mode = SWEEP_ENABLE;
uint16_t sweep_once_count = 1;

int set_frequency(freq_t freq)
{
  (void) freq;
  return 1;
}