#include "/include/map_renderer.h"

int offset_y, offset_x;
int view_height, view_width;
int map_height, map_width;
char *terrain;
char *map_temp;
char *row_temp;

void init_renderer(GameData *game_data, int width, int height) {
    map_temp = malloc(512);
    row_temp = malloc(512);

    terrain = game_data->terrain;
    view_height = height;
    view_width = width;
    offset_x = 0;
    offset_y = 0;
    map_height = game_data->map_height;
    map_width = game_data->map_width;
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

BOOL scroll_map(int ox, int oy) {
    if (view_width != map_width) {
        offset_x += ox;
        if (offset_x < 0) {
            offset_x = map_width;
        }
    }

    if (view_height != map_height) {
        offset_y += oy;
        if (offset_y < 0) {
            offset_y = map_height;
        }
    }

    return TRUE;
}

void render_map() {
    int y, x;

    char *p;
    int cur_color = CYAN_BG;
    output("{FGB WHITE}{BG CYAN}{CHIDE}");

    for (y = 0; y < view_height; ++y) {
        sprintf(row_temp, "{CPOS %d 2}", y + 4);
        output(row_temp);
        for (x = 0; x < view_width; ++x) {
            p = terrain + ((x + offset_x) % map_width) +
                    (((y + offset_y) % map_height) * map_width);
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

