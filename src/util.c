#include "util.h"

char *trimwhitespace(char *str) {
    char *end;
    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;
    if (*str == 0)  // All spaces?
        return str;
    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;
    // Write new null terminator character
    end[1] = '\0';
    return str;
}

void trim(char * s) {
    char * p = s;
    int l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(* p && isspace(* p)) ++p, --l;

    memmove(s, p, l + 1);
}

int count_files_in_directory(char *directory_name) {
    struct FileInfoBlock fib;
    BPTR lock;

    int i = 0;

    lock = Lock(directory_name, MODE_OLDFILE);

    if (lock) {
        Examine(lock, &fib);
        while (ExNext(lock, &fib)) {
            ++i;
        }
        UnLock(lock);
    }
    else {
        return 0;
    }
    return i;
}

int random(int min, int max) {
    return rand() % (max + 1 - min) + min;
}