#ifndef CNETDOOR_MD_STRUCTS_H
#define CNETDOOR_MD_STRUCTS_H

#include <stdio.h>
#include <proto/datatypes.h>

#define MAX_GAMES 10
#define MAX_PLAYERS 4

#define PLAYER_HANDLE_LEN 20

#define GAME_NAME_LEN 20
#define GAME_DESC_LEN 40
#define PASSWORD_LEN 20
#define MAP_NAME_LEN 30
#define MAP_DESC_LEN 80

enum GameState {
    OPEN,
    IN_PROGRESS,
    PAUSED,
    OVER
};

enum GameMode {
    ORDERS,
    SURVEY
};

typedef struct UserContext {
    unsigned int player_id;
    char handle[PLAYER_HANDLE_LEN + 1];
} UserContext;

typedef struct MapFile {
    char name[MAP_NAME_LEN+1];
    char desc[MAP_DESC_LEN+1];
    BYTE width;
    BYTE height;
    BYTE players;
} MapFile;

typedef struct GameData {
    int map_rows;
    int map_cols;
    char *terrain;
    char *owners;
    char *fog[MAX_PLAYERS];
} GameData;

typedef struct NewGameForm {
    char name[GAME_NAME_LEN + 1];
    char desc[GAME_DESC_LEN + 1];
    char password[PASSWORD_LEN + 1];
    char map[MAP_NAME_LEN + 1];
    BYTE num_players;
    BYTE num_autoturns;
} NewGameForm;

typedef struct GameFile {
    int game_id;
    enum GameState state;
    unsigned int round_number;
    BYTE cur_player_index;

    char name[GAME_NAME_LEN + 1];
    char desc[GAME_DESC_LEN + 1];
    char password[PASSWORD_LEN + 1];
    char map[MAP_NAME_LEN + 1];
    BYTE num_players;
    BYTE num_autoturns;

    unsigned int host_player_id;
    unsigned int player_ids[MAX_PLAYERS];
    char player_handles[MAX_PLAYERS][PLAYER_HANDLE_LEN +1];
    unsigned int player_journal_index[MAX_PLAYERS];
} GameFile;

#endif //CNETDOOR_MD_STRUCTS_H
