#ifndef CNETDOOR_INPUT_H
#define CNETDOOR_INPUT_H

#include "base.h"

char getkey(void);
void text_field(char *buf, int maxLen, const char *allowed_chars);
void text_field_at(int row, int col, char *buf, int maxLen, const char *allowed_chars);
char prompt_char(const char *allowed_chars);

#endif //CNETDOOR_INPUT_H
