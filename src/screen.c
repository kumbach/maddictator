#include "screen.h"
#include "ansiescaper.h"

void output(char *string) {
    escape_string(string);
}

void clearscreen(void) {
    setcursorpos(0,0);
    setcolor(BLACK_BG);
    printf("\n\033[2J");
}

void resetscreen(void) {
    printf("\033[0m");
}

void setcolor(int color) {
    printf("\033[2;%dm", color);
}

void setbrightcolor(int color) {
    printf("\033[1;%dm", color);
}

void setcursorpos(int r1, int c1) {
    printf("\033[%d;%df", r1, c1);
}

void drawborder(int r1, int c1, int r2, int c2, int color) {
    int c;
    int r;

    setcolor(color);
    setcursorpos(r1, c1);
    putchar(201);
    for (c=c1+1; c < c2; ++c) {
        putchar(205);
    }
    putchar(187);

    for (r =r1+1; r < r2; ++r) {
        setcursorpos(r, c1);
        putchar(186);

        setcursorpos(r, c2);
        putchar(186);
    }

    setcursorpos(r2, c1);
    putchar(200);

    for (c=c1+1; c < c2; ++c) {
        putchar(205);
    }
    putchar(188);

}

void eraselines(int r1, int r2, int color) {
    int r;

    setcolor(30 + color);
    setcolor(40 + color);
    setcursorpos(r1, 1);

    for (r =r1; r <= r2; ++r) {
        puts("\033[2K");
    }
}
void clearcols(int r, int c1, int c2, int color) {
    char *blanks;
    int len = c2 - c1 + 1;
    if (len <= 0 || len > 80) return;

    blanks = malloc(len);
    memset(blanks, ' ', len);
    blanks[len]= 0;

    setcolor(40 + color);
    setcursorpos(r, c1);
    printf(blanks);
    free(blanks);
}

void draw_window(int r1, int c1, int r2, int c2, int color, int fillcolor) {
    int c;
    int r;
    char *blank_line;
    int fill_len = c2 - c1;

    if (fillcolor >= 0) {
        blank_line = (char *)malloc(fill_len);
        memset(blank_line, ' ', fill_len);
        blank_line[fill_len-1] = 0;
    }

    setcolor(color);
    setcursorpos(r1, c1);

    putchar(201);

    for (c=c1+1; c < c2; ++c) {
        putchar(205);
    }
    putchar(187);

    for (r =r1+1; r < r2; ++r) {
        setcursorpos(r, c1);
        putchar(186);

        if (fillcolor >= 0 && r < r2) {
            setcolor(fillcolor);
            setcursorpos(r+1, c1+1);
            printf("%s", blank_line);
            setcolor(color);
        }

        setcursorpos(r, c2);
        putchar(186);
    }

    setcursorpos(r2, c1);
    putchar(200);
    for (c=c1+1; c < c2; ++c) {
        putchar(205);
    }
    putchar(188);
    if (blank_line) {
        free(blank_line);
    }
}

void drawhorizontalline(int r1, int c1, int c2, int color) {
    int c;

    setcursorpos(r1, c1);
    setcolor(color);
    for (c=c1; c <= c2; ++c) {
        putchar(' ');
    }
}

void drawhorizontalbar(int r, int c1, int c2, int color) {
    int c;

    setcursorpos(r, c1);
    setcolor(color);
    putchar(204);
    for (c=c1+1; c < c2; ++c) {
        putchar(205);
    }
    putchar(185);
}

void drawverticalline(int c1, int r1, int r2, int color) {
    int r;
    setcolor(color);
    for (r=r1; r <= r2; ++r) {
        setcursorpos(r, c1);
        putchar(' ');
    }
}

void drawverticalbar(int c, int r1, int r2, int color) {
    int r;
    setcolor(color);
    setcursorpos(r1, c);
    putchar(203);
    for (r=r1+1; r < r2; ++r) {
        setcursorpos(r, c);
        putchar(186);
    }
    setcursorpos(r2, c);
    putchar(202);
}

void hideCursor(void) {
    printf("\033[?25l");
}

void showCursor(void) {
    printf("\033[?25h");
}

void fillarea(int r1, int c1, int r2, int c2, int color) {
    int r;
    char blah[100];

    sprintf(blah, "%-*s", c2-c1+1, "");

    setcolor(color);
    for (r =r1; r <= r2; ++r) {
        setcursorpos(r, c1);
        printf(blah);
    }
}

void printat(int r, int c, char *text) {
    setcursorpos(r,c);
    printf(text);
}

void outputfile(char const *filename) {
    FILE* file = fopen(filename, "r");

    if (file) {
        int ch;
        while ((ch = fgetc(file)) != EOF) {
            putchar(ch); // Output character to stdout
        }

        fclose(file);
    } else {
        printf("Error opening file: %s\n", filename);
    }
}