//
// Created by Kevin on 2024-07-25.
//

#include "help_viewer.h"

void trimNewline(char* str) {
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == '\r' || str[len - 1] == '\n')) {
        str[--len] = '\0';
    }
}

void right_pad(char* str, int count, char pad) {
    int i;
    int len = (int)strlen(str);

    // Append padding characters
    for (i = 0; i < count; i++) {
        str[len + i] = pad;
        str[len + i + 1] = 0;
    }
}

char **read_help_file(char *filename, int max_line_len, int *lines_read) {
    char *fn = malloc(256);
    char *buf = NULL;
    char **content = NULL;

    FILE *file = NULL;

    size_t list_size = 40;
    size_t line_count = 0;

    int len;

    *lines_read = 0;

    sprintf(fn, "MAD_DICT:%s", filename);
    file = fopen(fn, "r");
    if (file == NULL) {
        fclose(file);
        free(fn);
        return NULL;
    }

    free(fn);
    buf = malloc(256);
    content = malloc(list_size * sizeof(size_t));
    memset(content, 0, list_size);

    while (fgets(buf, 255, file) != NULL) {
        if (strlen(buf) > max_line_len) {
            buf[max_line_len -1] = 0;
        }

        trimNewline(buf);
        if (strlen(buf) == 0) {
            strcpy(buf, " ");
        }
        len = real_len(buf);
        right_pad(buf, max_line_len - len, ' ');

        content[line_count] = malloc(strlen(buf) + 1);
        strcpy(content[line_count], buf);
        line_count++;

        if (line_count > list_size) {
            list_size *= 2;
            content = realloc(content, list_size);
        }
        memset(buf, 0, 256);
    }

    if (line_count < list_size) {
        content = realloc(content, line_count * sizeof(size_t));
    }

    fclose(file);
    free(buf);
    *lines_read = line_count ;

    return content;
}

void show_help_file(char *filename, int r1, int c1, int r2, int c2) {
    char *out = malloc(255);
    char **content = NULL;
    char *blank_line = NULL;
    BOOL redraw = TRUE;

    int start_index = 0;
    int end_index;
    int row;
    char ch;
    int lines = 0;
    int i;

    sprintf(out, "{WINDOW %d %d %d %dBLUE BLACK}", r1, c1, r2, c2);
    output(out);
    sprintf(out, "{FGB WHITE}{BG BLUE}{CENTER %d Mad Dictator Help}", r1);
    output(out);

    sprintf(out, "{FGB WHITE}{BG BLUE}{CENTER %d Press Q to exit help}", r2);
    output(out);

    content = read_help_file(filename, c2 - c1 - 1, &lines);

    sprintf(out, "{FGB WHITE}{BG BLUE}{CENTER %d Up/Dn scroll or Q to exit help}{FG CYAN}", r2);
    output(out);

    if (content == NULL) {
        sprintf(out,"{CPOS %d %d}{FGB RED}{BG BLACK}An error occurred reading the help file.", r1+1, c1+1);
        output(out);
        free(out);

        do {
            ch = getkey();
        }
        while (ch != 'Q');
        return;
    }

    blank_line = malloc(c2 - c1 + 1);
    memset(blank_line, ' ', c2 - c1 - 1);
    blank_line[c2 - c1 - 1] = '\0';

    do {
        row = r1 + 1;
        end_index = start_index + (r2 - r1) - 2;

        for (i=start_index; i <= end_index && redraw; i++) {
            if (i < lines) {
                sprintf(out, "{CPOS %d %d}%s", row, c1+1, content[i]);
                output(out);
            }
            else {
                sprintf(out, "{CPOS %d %d}%s", row, c1+1, blank_line);
                output(out);
            }
            row++;
        }

        sprintf(out, "{CPOS %d 54}{FG CYAN}", r2);
        output(out);

        redraw = FALSE;
        ch = getkey();
        if (ch == 27) {
            ch = getkey();
            if (ch == 91) {
                ch = getkey();
                if (ch == 'A') {
                    if (start_index > 0) {
                        start_index--;
                        redraw = TRUE;
                        continue;
                    }
                    output("{BELL}");
                }
                else if (ch == 'B') {
                    if (start_index < lines-1 && start_index + (r2 - r1) <= lines) {
                        start_index++;
                        redraw = TRUE;
                        continue;
                    }
                    output("{BELL}");
                }

            }
        }

    } while (ch != 'Q');
    free(blank_line);
    free(out);

    for (i=0; i < lines; ++i) {
        free(content[i]);
    }
    free(content);
}
