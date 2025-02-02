#include "game_select_menu.h"

BOOL delete_game_prompt(GameFile *game, char *result) {
    output("{CLEARCOLS 19 11 69 BLACK}");
    output("{CPOS 19 12}{FGB YELLOW}Are you sure (Y/N)?");
    if (prompt_char("YN") == 'N') {
        return FALSE;
    }
    return (BOOL)delete_game_file(game);
}

BOOL join_game_prompt(GameFile *game, char *result) {
    output("{CLEARCOLS 19 11 69 BLACK}");
    output("{CPOS 19 12}{FGB YELLOW}Are you sure (Y/N)?");
    if (prompt_char("YN") == 'N') {
        return FALSE;
    }
    return (BOOL)join_game(game->game_id, result);
}

BOOL leave_game_prompt(GameFile *game, char *result) {
    output("{CLEARCOLS 19 11 69 BLACK}");
    output("{CPOS 19 12}{FGB YELLOW}Are you sure (Y/N)?");
    if (prompt_char("YN") == 'N') {
        return FALSE;
    }
    return (BOOL)leave_game(game->game_id, result);
}

void make_player_display_name(char *result, GameFile *game, int player_index) {
    if (game->player_ids[player_index] == 0) {
        strcpy(result, "---OPEN SPOT---");
        return;
    }

    strcpy(result, game->player_handles[player_index]);
    if (game->player_ids[player_index] == game->host_player_id) {
        strcat(result, " (host)");
    }
}

void make_status_display(char *p1, GameFile *game) {
    if (game->state == OPEN) {
        strcpy(p1, "Waiting for players");
    }
    else if (game->state == IN_PROGRESS) {
        strcpy(p1, "In Progress");
    }
    else if (game->state == PAUSED) {
        strcpy(p1, "Paused");
    }
    else if (game->state == OVER) {
        strcpy(p1, "Game has finished");
    }
    else {
        strcpy(p1, "Unknown. Contact Sysop!");
    }
}

void show_game_details(int game_type, GameFile *game) {
    char *temp, *p1;

    temp = malloc(128);
    p1 = malloc(80);
    output("{FG WHITE}");
    output("{WINDOW 9 10 23 70 BLUE BLACK}{CPOS 10 21}{FGB WHITE}");
    sprintf(temp, "{CENTER 9 %s}", game->name);
    output(temp);

    sprintf(temp, "{BG BLACK}{FG GREEN}{CENTER 11 %s}", game->desc);
    output(temp);

    make_status_display(p1, game);
    sprintf(temp, "{CPOS 13 12}{FGB BLUE}Status  {FG WHITE}%s", p1);
    output(temp);

    sprintf(temp, "{CPOS 14 12}{FGB BLUE}Round # {FG WHITE}%d", game->round_number);
    output(temp);

    sprintf(temp, "{CPOS 15 12}{FGB BLUE}Map     {FG WHITE}%s", game->map);
    output(temp);

    make_player_display_name(p1, game, 0);
    sprintf(temp, "{CPOS 16 12}{FGB BLUE}Players {FG WHITE}%-20s  ", p1);
    output(temp);

    make_player_display_name(p1, game, 1);
    output(p1);

    if (game->num_players > 2) {
        make_player_display_name(p1, game, 2);
        sprintf(temp, "{CPOS 17 20}{FGB BLUE}{FG WHITE}%-20s  ", p1);
        output(temp);

        if (game->num_players == 4) {
            make_player_display_name(p1, game, 3);
            output(p1);
        }
    }
    free(temp);
    free(p1);
}

void show_menu_options(BOOL is_host, BOOL is_in_game, GameFile *game, char *allowed_keys) {
    if (is_host) {
        output("{CPOS 20 12}{FG GREEN}Select: {FGB WHITE}E{FG CYAN}nter Game, {FGB WHITE}D{FGB RED}elete Game{FG CYAN}, Go {FGB WHITE}B{FG CYAN}ack");
        strcpy(allowed_keys, "REDB");
    }
    else if (is_in_game && game->state == OPEN) {
        output("{CPOS 20 12}{FG GREEN}Select: {FGB WHITE}E{FG CYAN}nter Game, {FGB WHITE}L{FGB RED}eave Game{FG CYAN}, Go {FGB WHITE}B{FG CYAN}ack");
        strcpy(allowed_keys, "RELB");
    }
    else if (is_in_game) {
        output("{CPOS 20 12}{FG GREEN}Select: {FGB WHITE}E{FG CYAN}nter Game, Go {FGB WHITE}B{FG CYAN}ack");
        strcpy(allowed_keys, "REB");
    }
    else {
        output("{CPOS 20 12}{FG GREEN}Select: {FGB WHITE}J{FG CYAN}oin Game, Go {FGB WHITE}B{FG CYAN}ack");
        strcpy(allowed_keys, "RJB");
    }
}

void game_select_menu(int game_type, GameFile *game, char *result_message) {
    char *temp;
    BOOL is_host;
    BOOL is_in_game;
    char allowed_keys[10];
    char key;
    char *message;
    BOOL redraw = TRUE;

    temp = malloc(128);

    sprintf(temp, "MAD_DICT:data/games/%d.game", game->game_id);
    if (access(temp, F_OK)) {
        sprintf(result_message, "{FGB RED}The game '%s' does not exist.", game->name);
        free(temp);
        return;
    }

    message = malloc(128);
    message[0] = 0;

    is_host = game->host_player_id == user_context.player_id;
    is_in_game = player_is_in_game(game);

    do {
        if (redraw) {
            char filename[100];
            GameFile *updatedGame;
            sprintf(filename, "MAD_DICT:data/games/%d.game", game->game_id);

            updatedGame = malloc(sizeof(GameFile));

            if (read_game_file(filename, updatedGame)) {
                memcpy(game, updatedGame, sizeof(GameFile));
                free(updatedGame);
                updatedGame = NULL;
            }
            show_game_details(game_type, game);
            show_menu_options(is_host, is_in_game, game, allowed_keys);
            redraw = FALSE;
        }

        if (!redraw) {
            output("{CLEARCOLS 19 11 69 BLACK}");
        }

        if (strlen(message) > 0) {
            sprintf(temp, "{FGB MAGENTA}{CENTER 19 %s}", message);
            output(temp);
            message[0] = 0;
        }
        output("{CPOS 20 53}");
        key = prompt_char(allowed_keys);
        switch (key) {
            case 'L':
                if (leave_game_prompt(game, message)) {
                    sprintf(temp, "{FG CYAN}You have left '{FGB MAGENTA}%s}{FG CYAN}'.", game->name);
                    strcpy(result_message, temp);
                    is_in_game = FALSE;
                    key = 'B';
                };
                redraw = TRUE;
                break;
            case 'D':
                sprintf(temp, "{FG CYAN}Game '{FGB MAGENTA}%s}{FG CYAN}' was deleted.", game->name);
                if (delete_game_prompt(game, message)) {
                    strcpy(result_message, temp);
                    key = 'B';
                }
                break;
            case 'J':
                if (join_game_prompt(game, message)) {
                    sprintf(temp, "{FG CYAN}You are now in '{FGB MAGENTA}%s}{FG CYAN}'.", game->name);
                    strcpy(result_message, temp);
                    is_in_game = TRUE;
                };
                redraw = TRUE;
                break;
            case 'E':
                if (game->state != IN_PROGRESS) {
                    strcpy(message, "Cannot enter yet. The game has not started.");
                    break;
                }
                play_game(game->game_id, result_message);
                key = 'B';
                break;

            case 'R':
                strcpy(message, "Game details have been refreshed.");
                redraw = TRUE;
                break;
        }
    }
    while (key != 'B');

    free(temp);
    free(message);
}
