#include "base.h"
#include "game_manager.h"
#include "main.h"
#include <fcntl.h>
#include <errno.h>

BOOL create_game(NewGameForm *game_form) {
    FILE *file;
    char filename[GAME_NAME_LEN * 2];
    int game_number = 0;
    GameFile *game;

    game_number = next_game_number();
    if (game_number < 0) {
        return FALSE;
    }

    game = malloc(sizeof(GameFile));
    memset(game, 0, sizeof(GameFile));

    game->game_id = game_number;
    game->state = OPEN;

    strcpy(game->name, game_form->name);
    strcpy(game->desc, game_form->desc);
    strcpy(game->password, game_form->password);
    strcpy(game->map, game_form->map);

    game->num_players = game_form->num_players;
    game->num_autoturns = game_form->num_autoturns;

    game->player_ids[0] = user_context.player_id;
    game->host_player_id = user_context.player_id;
    strncpy(game->player_handles[0], user_context.handle, PLAYER_HANDLE_LEN);

    sprintf(filename, "MAD_DICT:data/games/%d.game", game_number);
    file = fopen(filename, "wb");
    if (file != NULL) {
        fwrite(game, sizeof(GameFile), 1, file);
        fclose(file);
    }
    free(game);

    return TRUE;
}


int next_game_number() {
    int next_game = 0;

    FILE *file = fopen("MAD_DICT:data/next_game_id", "r+");
    if (file == NULL) {
        file = fopen("MAD_DICT:data/next_game_id", "w");
        if (file == NULL) {
            return -1;
        }
    }
    else {
        fscanf(file, "%d", &next_game);
        rewind(file);
    }

    next_game++;
    fprintf(file, "%d", next_game);
    fclose(file);
    return next_game;
}

BOOL player_is_in_game(GameFile *game_file) {
    int i;

    for (i = 0; i < game_file->num_players; ++i) {
        if (game_file->player_ids[i] == user_context.player_id) {
            return TRUE;
        }
    }
    return FALSE;
}

int num_joined_players(GameFile *game_file) {
    int i;
    int count = 0;

    for (i = 0; i < game_file->num_players; ++i) {
        if (game_file->player_ids[i] != 0) {
            count++;
        }
    }
    return count;
}


void randomize_player_order(GameFile *game_file) {
    int i, p1, p2;
    char temp_handle[PLAYER_HANDLE_LEN +1];
    int temp_id;

    int num_players = num_joined_players(game_file);

    for (i=0; i < random(10, 50); i++) {
        p1 = random(0, num_players-1);
        while ((p2 = random(0, num_players-1)) == p1);

        temp_id = game_file->player_ids[p1];
        game_file->player_ids[p1] = game_file->player_ids[p2];
        game_file->player_ids[p2] = temp_id;

        strcpy(temp_handle, game_file->player_handles[p1]);
        strcpy(game_file->player_handles[p1], game_file->player_handles[p2]);
        strcpy(game_file->player_handles[p2], temp_handle);
    }
}


BOOL join_game(int game_number, char *result_message) {
    char full_filename[128];
    GameFile *game_file;
    int i;
    FILE *write_file;
    char *error = NULL;
    BOOL game_full = FALSE;

    game_file = malloc(sizeof(GameFile));
    sprintf(full_filename, "MAD_DICT:data/games/%d.game", game_number);

    read_game_file(full_filename, game_file);
    if (!game_file) {
        free(game_file);
        strcpy(result_message, "Sorry, something went wrong. Try again.");
        return FALSE;
    }

    if (player_is_in_game(game_file)) {
        free(game_file);
        strcpy(result_message, "You are already in that game!");
        return FALSE;
    }

    if (game_file->state == IN_PROGRESS || game_file->state == PAUSED) {
        free(game_file);
        strcpy(result_message, "It's too late to join, the game has already started.");
        return FALSE;
    }

    if (game_file->state == OVER) {
        free(game_file);
        strcpy(result_message, "You can't join this game because it has finished.");
        return FALSE;
    }

    error = "Sorry, this game is full!";
    for (i = 0; i < game_file->num_players; ++i) {
        if (game_file->player_ids[i] == 0) {
            game_file->player_ids[i] = user_context.player_id;
            strncpy(game_file->player_handles[i], user_context.handle, PLAYER_HANDLE_LEN);
            error = NULL;
            break;
        }
    }

    if (error) {
        free(game_file);
        return FALSE;
    }

    game_full = TRUE;
    for (i = 0; i < game_file->num_players; ++i) {
        if (game_file->player_ids[i] == 0) {
            game_full = FALSE;
            break;
        }
    }

    game_file->state = game_full ? IN_PROGRESS : OPEN;
    if (game_file->state == IN_PROGRESS) {
        game_file->round_number = 1;

        game_file->cur_player_index = 0;
        randomize_player_order(game_file);
    }

    write_file = fopen(full_filename, "wb");
    if (write_file != NULL) {
        fseek(write_file, 0, SEEK_SET);
        fwrite(game_file, sizeof(GameFile), 1, write_file);
        fclose(write_file);
        write_file = NULL;
    }

    // watch out for returns above...
    free(game_file);

    return TRUE;
}

BOOL leave_game(int game_number, char *result_message) {
    char full_filename[128];
    GameFile *game_file;
    int i;
    FILE *write_file;
    BOOL game_full = FALSE;

    game_file = malloc(sizeof(GameFile));
    sprintf(full_filename, "MAD_DICT:data/games/%d.game", game_number);

    read_game_file(full_filename, game_file);
    if (!game_file) {
        free(game_file);
        strcpy(result_message, "Sorry, something went wrong. Try again.");
        return FALSE;
    }

    if (!player_is_in_game(game_file)) {
        free(game_file);
        strcpy(result_message, "You are not in this game.");
        return FALSE;
    }

    if (game_file->state == IN_PROGRESS || game_file->state == PAUSED) {
        free(game_file);
        strcpy(result_message, "You can't leave a game in progress. Try resigning instead.");
        return FALSE;
    }

    if (game_file->state == OVER) {
        free(game_file);
        strcpy(result_message, "This game is over; you can't leave it now.");
        return FALSE;
    }

    //TODO fill in gaps when player leaves a game
    for (i = 0; i < game_file->num_players; ++i) {
        if (game_file->player_ids[i] == user_context.player_id) {
            game_file->player_ids[i] = 0;
            strncpy(game_file->player_handles[i], "", PLAYER_HANDLE_LEN);
            break;
        }
    }

    write_file = fopen(full_filename, "wb");
    if (write_file != NULL) {
        fseek(write_file, 0, SEEK_SET);
        fwrite(game_file, sizeof(GameFile), 1, write_file);
        fclose(write_file);
        write_file = NULL;
    }

    // watch out for returns above...
    free(game_file);
    strcpy(result_message, "You have left the game.");

    return TRUE;
}

BOOL read_game_file(char *filename, GameFile *game_file) {
    FILE *filePtr;

    filePtr = fopen(filename, "rb");
    if (filePtr) {
        fseek(filePtr, 0, SEEK_SET);
        fread(game_file, sizeof(GameFile), 1, filePtr);
        fclose(filePtr);
        return TRUE;
    }
    return FALSE;
}

BOOL read_game_data(int game_id, GameData *game_data) {
    FILE *filePtr;
    char *buf = malloc(255);

    sprintf(buf, "MAD_DICT:games/%d.data");
    filePtr = fopen(buf, "rb");
    if (filePtr) {
        fseek(filePtr, 0, SEEK_SET);
        fread(game_data, sizeof(GameData), 1, filePtr);
        fclose(filePtr);
        free(buf);
        return TRUE;
    }
    free(buf);
    return FALSE;
}

GameFile **get_game_list(int *count, int game_type) {
    int num_games;
    GameFile **games;
    struct FileInfoBlock fib;
    BPTR lock;
    BOOL success;
    char full_filename[128];
    int i=0;

    *count = 0;

    num_games = count_files_in_directory("MAD_DICT:data/games");
    if (num_games == 0) {
        return NULL;
    }

    lock = Lock("MAD_DICT:data/games", ACCESS_READ);
    if (!lock) {
        return NULL;
    }

    games = (GameFile **)malloc(sizeof(GameFile) * num_games);
    memset(games, 0, sizeof(GameFile) * num_games);

    games[i]  = malloc(sizeof(GameFile));

    Examine(lock, &fib);
    while (success = ExNext(lock, &fib) && i < num_games) {
        sprintf(full_filename, "MAD_DICT:data/games/%s", fib.fib_FileName);
        if (read_game_file(full_filename, games[i])) {
            switch (game_type) {
                case 0: // my games
                    if (player_is_in_game(games[i])) {
                        i++;
                        games[i] = malloc(sizeof(GameFile));
                    }
                    break;

                case 1: // joinable games
                    if (games[i]->state == OPEN && !player_is_in_game(games[i])) {
                        i++;
                        games[i] = malloc(sizeof(GameFile));
                    }
                break;

                case 3: // completed games
                    if (games[i]->state == OVER) {
                        i++;
                        games[i] = malloc(sizeof(GameFile));
                    }
                break;
            }
        }
    }
    free(games[i]);
    UnLock(lock);

    if (i == 0) {
        free(games);
        games = NULL;
    }
    else if (i < num_games) {
        games = realloc(games, sizeof(GameFile) * i-1);
    }
    *count = i;
    return games;
}

BOOL delete_game_file(GameFile *game) {
    char *temp;
    int error;

    temp = malloc(128);

    sprintf(temp, "MAD_DICT:data/games/%d.game", game->game_id);
    error = remove(temp);
    free(temp);

    return (BOOL)(error == 0);
}

BOOL game_exists(char *game_name) {
    BOOL result = FALSE;
    BOOL success;
    struct FileInfoBlock fib;
    BPTR lock;
    char filename[128];
    GameFile *gameFile;

    gameFile = malloc(sizeof(GameFile));

    lock = Lock("MAD_DICT:data/games", ACCESS_READ);
    if (!lock) {
        return result;
    }

    Examine(lock, &fib);
    while (success = ExNext(lock, &fib)) {
        sprintf(filename, "MAD_DICT:data/games/%s", fib.fib_FileName);
        if (read_game_file(filename, gameFile)) {
            if (strcmpi(game_name, gameFile->name) == 0) {
                result = TRUE;
                break;
            }
        }
    }

    UnLock(lock);
    free(gameFile);

    return result;
}

void free_game_list(GameFile **gamelist, int num_games) {
    int i;

    if (gamelist == NULL) return;

    for (i=0; i < num_games; ++i) {
        free(gamelist[i]);
    }
    free(gamelist);
}