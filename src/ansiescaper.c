#include "base.h"

#include "screen.h"
#include "ansiescaper.h"

char capture_buf[512] = "";
char temp_buf[512] = "";

void escape_string(char *string) {
    int capturing = FALSE;

    char *in = string;
    char *cap = capture_buf;

    memset(capture_buf, 0, sizeof(capture_buf));

    while (*in) {
        if (*in == '{') {
            capturing = TRUE;
            cap = capture_buf;
        }
        else if (*in == '}') {
            capturing = FALSE;
            *cap = '\0';
            output_placeholders(capture_buf);
        }
        else {
            if (capturing) {
                *cap = *in;
                cap++;
            }
            else {
                putchar(*in);
            }
        }
        in++;
    }
}


int real_len(char *string) {
    int capturing = FALSE;
    char *in = string;
    int len = 0;

    while (*in) {
        if (!capturing && *in == '{') {
            capturing = TRUE;
        }
        else if (capturing && *in == '}') {
            capturing = FALSE;
        }
        else if (!capturing) {
            len++;
        }
        in++;
    }

    return len;
}

void output_placeholders(char *cap) {
    char cmd[100];

    if (strcmp(cap, "CLS") == 0) {
        clearscreen();
    }
    else if (strcmp(cap, "RESET") == 0) {
        resetscreen();
    }
    else if (strcmp(cap, "CHIDE") == 0) {
        hideCursor();
    }
    else if (strcmp(cap, "CSHOW") == 0) {
        showCursor();
    }
    else if (strncmp(cap, "RECT", 4) == 0) {
        int r1, c1, r2, c2;
        char color_name[8];

        sscanf(cap, "%s %d %d %d %d %s", &cmd, &r1, &c1, &r2, &c2, &color_name[0]);
        drawborder(r1, c1, r2, c2, 40 + color_for_name(color_name));
    }
    else if (strncmp(cap, "WINDOW", 6) == 0) {
        int r1, c1, r2, c2;
        char color_name[8];
        char color_name2[8];

        sscanf(cap, "%s %d %d %d %d %s %s", &cmd, &r1, &c1, &r2, &c2, &color_name[0], &color_name2[0]);
        draw_window(r1, c1, r2, c2, 40 + color_for_name(color_name), 40 + color_for_name(color_name2));
    }
    else if (strncmp(cap, "HLINE", 5) == 0) {
        int r1, c1, c2;
        char color_name[8];

        sscanf(cap, "%s %d %d %d %s", &cmd, &r1, &c1, &c2, &color_name[0]);
        drawhorizontalline(r1, c1, c2, 40 + color_for_name(color_name));
    }
    else if (strncmp(cap, "VLINE", 5) == 0) {
        int c1, r1, r2;
        char color_name[8];
        sscanf(cap, "%s %d %d %d %s", &cmd, &c1, &r1, &r2, &color_name[0]);
        drawverticalline(c1, r1, r2, 40 + color_for_name(color_name));
    }
    else if (strncmp(cap, "HBAR", 4) == 0) {
        int r, c1, c2;
        char color_name[8];
        sscanf(cap, "%s %d %d %d %s", &cmd, &r, &c1, &c2, &color_name[0]);
        drawhorizontalbar(r, c1, c2, 40 + color_for_name(color_name));
    }
    else if (strncmp(cap, "VBAR", 4) == 0) {
        int c, r1, r2;
        char color_name[8];
        sscanf(cap, "%s %d %d %d %s", &cmd, &c, &r1, &r2, &color_name[0]);
        drawverticalbar(c, r1, r2, 40 + color_for_name(color_name));
    }
    else if (strncmp(cap, "CLEARROWS", 9) == 0) {
        int r1, r2;
        char color_name[8];

        sscanf(cap, "%s %d %d %s", &cmd, &r1, &r2, &color_name[0]);
        eraselines(r1, r2, color_for_name(color_name));
    }
    else if (strncmp(cap, "CLEARCOLS", 9) == 0) {
        int r, c1, c2;
        char color_name[8];

        sscanf(cap, "%s %d %d %d %s", &cmd, &r, &c1, &c2, &color_name[0]);
        clearcols(r, c1, c2, color_for_name(color_name));
    }
    else if (strncmp(cap, "CLEARTOEND", 10) == 0) {
        int r, c;

        sscanf(cap, "%s %d %d", &cmd, &r, &c);
        setcursorpos(r, c);
        puts("\033[0K");
    }
    else if (strncmp(cap, "CENTER", 6) == 0) {
        int r;
        int c;
        char text[81];

        sscanf(cap, "%s %d %[^\n]", &cmd, &r, &text[0]);

        c = (79 - strlen(text)) / 2;
        setcursorpos(r, c);
        printf(text);
    }

    else if (strncmp(cap, "FGB", 3) == 0) {
        char color_name[8];

        sscanf(cap, "%s %s", &cmd, &color_name[0]);
        setbrightcolor(30 + color_for_name(color_name));
    }
    else if (strncmp(cap, "FG", 2) == 0) {
        char color_name[8];

        sscanf(cap, "%s %s", &cmd, &color_name[0]);
        setcolor(30 + color_for_name(color_name));
    }

    else if (strncmp(cap, "BG", 2) == 0) {
        char color_name[8];

        sscanf(cap, "%s %s", &cmd, &color_name[0]);
        setcolor(40 + color_for_name(color_name));
    }
    else if (strncmp(cap, "CPOS", 4) == 0) {
        int r, c;

        sscanf(cap, "%s %d %d", &cmd, &r, &c);
        printf("\033[%d;%df", r, c);
    }
    else if (strncmp(cap, "BELL", 4) == 0) {
        printf("\x7");
    }

}

int color_for_name(char *color_name) {
    if (strcmp(color_name, "BLACK") == 0) {
        return 0;
    }
    else if (strcmp(color_name, "RED") == 0) {
        return 1;
    }
    else if (strcmp(color_name, "GREEN") == 0) {
        return 2;
    }
    else if (strcmp(color_name, "YELLOW") == 0) {
        return 3;
    }
    else if (strcmp(color_name, "BLUE") == 0) {
        return 4;
    }
    else if (strcmp(color_name, "MAGENTA") == 0) {
        return 5;
    }
    else if (strcmp(color_name, "CYAN") == 0) {
        return 6;
    }
    else if (strcmp(color_name, "WHITE") == 0) {
        return 7;
    }

    return 0;
}

