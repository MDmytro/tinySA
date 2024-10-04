#ifndef UTILS_H
#define UTILS_H

#include "nanovna.h"

// Use macro, std isdigit more big
#define _isdigit(c) (c >= '0' && c <= '9')

long_t my_atoi(const char *p);

freq_t my_atoui(const char *p);

float my_atof(const char *p);

int get_str_index(const char *v, const char *list);

int parse_line(char *line, char* args[], int max_cnt);

#endif