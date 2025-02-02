#include "/include/map_renderer.h"

int offset_row, offset_col;
int disp_rows, disp_cols;
int map_rows, map_cols;
char *terrain;
char *map_temp;
char *row_temp;

void init_renderer(GameData *game_data, int rows, int cols) {
    map_temp = malloc(512);
    row_temp = malloc(512);

    terrain = game_data->terrain;
    disp_rows = rows;
    disp_cols = cols;
    offset_col = 0;
    offset_row = 0;
    map_rows = game_data->map_rows;
    map_cols = game_data->map_cols;
}

void cleanup_renderer() {
    if (map_temp != NULL) {
        free(map_temp);
        map_temp = NULL;
    }
    if (row_temp != NULL) {
        free(row_temp);
        row_temp = NULL;
    }
}

BOOL scroll_map(int rows, int cols) {
    offset_row += rows;
    if (offset_row < 0) {
        offset_row = 0;
    }
    else if (offset_row > map_rows - disp_rows - 1) {
        offset_row = map_rows - disp_rows;
    }

    offset_col += cols;
    if (offset_col < 0) {
        offset_col = 0;
    }
    else if (offset_col > map_cols - disp_cols - 1) {
        offset_col = map_cols - disp_cols;
    }

    return TRUE;
}

void render_map() {
    int row, col;
    char *p;
    int cur_color = CYAN_BG;

    output("{FGB WHITE}{BG CYAN}{CHIDE}");
    for (row = 0; row < disp_rows; ++row) {
        sprintf(row_temp, "{CPOS %d 2}", row + 4);
        output(row_temp);

        p = terrain + ((row + offset_row) * map_cols) + offset_col;
        for (col = 1; col <= disp_cols; ++col) {
            switch (*p) {
                case '~':
                    if (cur_color != BLUE_BG) {
                        output("{BG BLUE}");
                        cur_color = BLUE_BG;
                    }
                    putchar(' ');
                    break;
                case '.':
                    if (cur_color != GREEN_BG) {
                        output("{BG GREEN}");
                        cur_color = GREEN_BG;
                    }
                    putchar(' ');
                    break;
                case '-':
                    if (cur_color != GREEN_BG) {
                        output("{BG GREEN}");
                        cur_color = GREEN_BG;
                    }
                    output("{FG BLUE}~");
                    break;
                case 'c':
                    output("{BG GREEN}{FGB WHITE}*");
                    cur_color = GREEN_BG;
                    break;
                case '^':
                    if (cur_color != GREEN_BG) {
                        output("{BG GREEN}");
                        cur_color = GREEN_BG;
                    }
                    output("{FG WHITE}^");
                    break;
                default:
                    putchar(*p);
            }
            p++;
        }
    }
    output("{CSHOW}");

}

