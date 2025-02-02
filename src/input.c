#include "input.h"
#include "screen.h"
#include <math.h>


char getkey(void) {
    char key = getch();
    while (key == EOF) {
        key = getch();
    }
    return (char) toupper(key);
}

char prompt_char(const char *allowed_chars) {
    char c;
    char uc;

    do {
        c = getch();
        uc = toupper(c);
    }
    while (strchr(allowed_chars, toupper(c)) == NULL);
    return uc;
}

/*
 * Make sure buf is max_len +1 in size otherwise crash.
 */
void text_field_at(int row, int col, char *buf, int max_len, const char *allowed_chars) {
    char c;
    char uc;
    char *ptr;
    int in_len = strlen(buf);

    ptr = buf + in_len;
    setcursorpos(row, col + in_len);

    do {
        c = getch();
        switch (c) {
            case 13:
                return;
            case 8:
                if (ptr > buf) {
                    printf("\033[1D \033[1D");
                    ptr--;
                    *ptr = 0;
                }
                else if (ptr == buf) {
                    *ptr = 0;
                    putchar(7);
                }
                break;
            case 7:
            case 9:
            case 127:
                break;
            case 27:
                getch();
                getch();
                break;
            default:
                if (allowed_chars != NULL) {
                    uc = toupper(c);
                    if (strchr(allowed_chars, uc) == NULL) {
                        putchar(7);
                        continue;
                    }
                }
                if (ptr - buf >= max_len ) {
                    putchar(7);
                    continue;
                }
                putchar(c);
                *ptr = c;
                ptr++;
                *ptr = 0;
                break;
        }
    }
    while (c != 13);

}
