#ifndef CNETDOOR_GAME_MANAGER_H
#define CNETDOOR_GAME_MANAGER_H

#include "base.h"

BOOL game_exists(char *game_name);
BOOL create_game(NewGameForm *new_game_form);
GameFile **get_game_list(int *num_games, int game_type);
BOOL read_game_file(char *filename, GameFile *game_file);
BOOL join_game(int game_number, char *result_message);
BOOL leave_game(int game_number, char *result_message);
BOOL player_is_in_game(GameFile *game_file);
int num_joined_players(GameFile *game_file);
int next_game_number(void);
void free_game_list(GameFile **gamelist, int num_games);
BOOL delete_game_file(GameFile *game);
#endif //CNETDOOR_GAME_MANAGER_H
