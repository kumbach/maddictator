#ifndef ANSIESCAPER_H
#define ANSIESCAPER_H

#include "base.h"


void escape_string(char *string);
void output_placeholders(char *cap);
int color_for_name(char *color_name);
int real_len(char *string);

#endif //ANSIESCAPER_H
