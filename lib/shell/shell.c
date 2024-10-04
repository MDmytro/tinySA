#include "shell.h"
#include <chprintf.h>

BaseSequentialStream *shell_stream;
threads_queue_t shell_thread;

// Shell commands output
int shell_printf(const char *fmt, ...)
{
  if (shell_stream == NULL) return 0;
  va_list ap;
  int formatted_bytes = 0;
  va_start(ap, fmt);
  formatted_bytes = chvprintf(shell_stream, fmt, ap);
  va_end(ap);
  return formatted_bytes;
}

// Shell commands output
int usage_printf(const char *fmt, ...)
{
  if (shell_stream == NULL) return 0;
  va_list ap;
  int formatted_bytes = 0;
  va_start(ap, fmt);
  shell_printf("usage: ");
  formatted_bytes += chvprintf(shell_stream, fmt, ap);
  va_end(ap);
  return formatted_bytes;
}

#ifdef __USE_SERIAL_CONSOLE__
// Serial Shell commands output
int shell_serial_printf(const char *fmt, ...)
{
  va_list ap;
  int formatted_bytes;
  va_start(ap, fmt);
  formatted_bytes = chvprintf((BaseSequentialStream *)&SD1, fmt, ap);
  va_end(ap);
  return formatted_bytes;
}
#endif