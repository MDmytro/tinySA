#ifndef SHELL_H
#define SHELL_H

#include "hal.h"

static BaseSequentialStream *shell_stream; // TODO: Later need to change as extern or move or create function
threads_queue_t shell_thread;

#endif
