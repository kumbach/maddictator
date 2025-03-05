#include "base.h"
#include "map.h"

#define MAX_MAPS 10

BOOL *init_game_data(char *map_name, GameData *game_data) {
    char *buf = malloc(255);
    char *p;
    int ch;
    FILE *file;
    int height, width, num_players, mem_size;

    sprintf(buf, "MAD_DICT:maps/%s.map", map_name);

    file = fopen(buf, "r");
    if (file == NULL) {
        free(buf);
        return FALSE;
    }

    fgets(buf, 255, file);  // description
    fscanf(file, "%d %d %d", &num_players, &width, &height);

    mem_size = width * height;
    game_data->map_width = width;
    game_data->map_height = height;

    game_data->terrain = malloc(mem_size);
    memset(game_data->terrain, 0, mem_size);

    game_data->owners = malloc(mem_size);
    memset(game_data->owners, 255, mem_size);

    game_data->fog[0] = malloc(mem_size);
    memset(game_data->fog[0], 0, mem_size);

    game_data->fog[1] = malloc(mem_size);
    memset(game_data->fog[1], 0, mem_size);

    game_data->fog[2] = malloc(mem_size);
    memset(game_data->fog[2], 0, mem_size);

    game_data->fog[3] = malloc(mem_size);
    memset(game_data->fog[3], 0, mem_size);

    // Read the map into memory
    p = game_data->terrain;
    ch = fgetc(file);
    while (ch != EOF) {
        if (ch != '\r' && ch != '\n' && (p - game_data->terrain < mem_size)) {
            *p++ = ch;
        }
        ch = fgetc(file);
    }
    fclose(file);

    free(buf);
    return TRUE;
}

MapFile ** map_list(void) {
    struct FileInfoBlock fib;
    int i = 0;
    BPTR lock;

    MapFile **map_list = malloc(sizeof(MapFile) * MAX_MAPS);

    lock = Lock("MAD_DICT:maps", MODE_OLDFILE);

    if (lock) {
        while (ExNext(lock, &fib)) {
            map_list[i] = malloc(sizeof(MapFile));
            strcpy(map_list[i]->name, fib.fib_FileName);
            map_list[i]->players = (BYTE)2;
            i++;
        }
        UnLock(lock); // Release the lock when done

    }
    else {
        puts("ERROR: Could not read map directory.");
    }
    return map_list;
}
