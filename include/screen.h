//
// Created by Kevin on 2024-05-17.
//

#ifndef SCREEN_H
#define SCREEN_H

#include "base.h"


#define BLACK_FG 30
#define RED_FG 31
#define GREEN_FG 32
#define YELLOW_FG 33
#define BLUE_FG 34
#define MAGENTA_FG 35
#define CYAN_FG 36
#define WHITE_FG 37

#define BLACK_BG 40
#define RED_BG 41
#define GREEN_BG 42
#define YELLOW_BG 43
#define BLUE_BG 44
#define MAGENTA_BG 45
#define CYAN_BG 46
#define WHITE_BG 47

void output(char *string);
void clearscreen(void);
void resetscreen(void);
void setcolor(int color);
void setbrightcolor(int color);
void fillarea(int r1, int c1, int r2, int c2, int color);
void drawborder(int r1, int c1, int r2, int c2, int color);
void draw_window(int r1, int c1, int r2, int c2, int color, int fillcolor);
void eraselines(int r1, int r2, int color);
void clearcols(int r, int c1, int c2, int color);

void setcursorpos(int r1, int c1);
void drawhorizontalline(int r1, int c1, int c2, int color);
void drawhorizontalbar(int r, int c1, int c2, int color);

void drawverticalline(int c1, int r1, int r2, int color);
void drawverticalbar(int c, int r1, int r2, int color);
void hideCursor(void);
void showCursor(void);
void printat(int r, int c, char *text);

void outputfile(char const *filename);

#endif //SCREEN_H
