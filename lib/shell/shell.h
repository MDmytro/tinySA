#ifndef SHELL_H
#define SHELL_H

#include "hal.h"

extern BaseSequentialStream *shell_stream; // TODO: Later need to change as extern or move or create function
extern threads_queue_t shell_thread;

int shell_printf(const char *fmt, ...);
int usage_printf(const char *fmt, ...);

#endif
