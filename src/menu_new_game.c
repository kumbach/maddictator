#include "menu_new_game.h"

void validate_start_game_fields(char *errorBuf, char *title, char *desc, char *numPlayers, char *turns) {
    if (strlen(title) == 0) {
        strcpy(errorBuf, "The game needs a title.");
        return;
    }

    if (game_exists(title)) {
        strcpy(errorBuf, "A game with this name already exists. Choose another.");
        return;
    }

    if (strlen(desc) == 0) {
        strcpy(errorBuf, "Enter a description of the game.");
        return;
    }

    if (atoi(numPlayers) <= 1) {
        strcpy(errorBuf, "How many players in the game?");
        return;
    }

    if (atoi(turns) <= 0) {
        strcpy(errorBuf, "How many quick-turns at start of game?");
        return;
    }
}

void show_field_value(int row, int col, int len, char *value, char *buf) {
    sprintf(buf, "{CPOS %d %d}{FGB WHITE}{BG BLUE}%-*s", row, col, len, value);
    output(buf);
}

void do_new_game_menu(char *result_message) {
    char game_name[GAME_NAME_LEN + 1] = "";
    char game_desc[GAME_DESC_LEN + 1] = "";
    char c[MAP_NAME_LEN + 1] = "First Blood";
    char game_players[2] = "2";
    char game_quickturns[3] = "1";
    char game_password[PASSWORD_LEN + 1] = "";
    char errorMsg[81] = "";
    BOOL redraw = TRUE;

    char *temp;

    int m, len, l2;
    int msg_len;
    struct NewGameForm *new_game_form = NULL;
    BOOL keep_looping = TRUE;
    temp = malloc(128);

    do {
        if (redraw) {
            output("{FG WHITE}");
            output("{WINDOW 9 10 23 74 BLUE BLACK}{FGB WHITE}{CENTER 9 START NEW GAME}");

            output("{CPOS 11 12}{BG BLACK}{FGB BLUE}[T]itle");
            show_field_value(11, 33, GAME_NAME_LEN, game_name,temp);

            output("{CPOS 12 12}{BG BLACK}{FGB BLUE}[D]escription");
            show_field_value(12, 33, GAME_DESC_LEN, game_desc,temp);

            output("{CPOS 13 12}{BG BLACK}{FGB BLUE}[M]ap");
            show_field_value(13, 33, MAP_NAME_LEN, c,temp);

            output("{CPOS 14 12}{BG BLACK}{FGB BLUE}[P]layers (2-4)");
            show_field_value(14, 33, 1, game_players, temp);

            output("{CPOS 15 12}{BG BLACK}{FGB BLUE}[Q]uick Turns (1-99)");
            show_field_value(15, 33, 2, game_quickturns, temp);

            output("{CPOS 16 12}{BG BLACK}{FGB BLUE}P[a]ssword");
            show_field_value(16, 33, PASSWORD_LEN, game_password, temp);

            output("{CPOS 18 12}{BG BLACK}{FG GREEN}Options: {BG BLACK}{FGB BLUE}[C]reate Game [B]ack");

            output("{FG GREEN}{CENTER 21 Select a field to change or an option:}");

            redraw = FALSE;
        }
        output("{FG GREEN}{CPOS 21 58}");
        m = prompt_char("TDMPQACB");
        output("{FGB WHITE}{BG BLUE}");
        *errorMsg = 0;

        switch (m) {
            case 'T':
                text_field_at(11, 33, game_name, GAME_NAME_LEN, NULL);
                break;
            case 'D':
                text_field_at(12, 33, game_desc, GAME_DESC_LEN, NULL);
                break;
            case 'M':
                output("{BELL}{BG BLACK}{CPOS 17 11}                                                             }{FGB RED}{BG BLACK}{CENTER 17 Sorry, there is only one map available right now...}");
                break;
            case 'P':
                text_field_at(14, 33, game_players, 1, "234");
                break;
            case 'Q':
                text_field_at(15, 33, game_quickturns, 2, "1234567890");
                break;
            case 'A':
                text_field_at(16, 33, game_password, PASSWORD_LEN, NULL);
                break;
            case 'B':
                keep_looping = FALSE;
                break;
            case 'C':
                trim(game_name);
                trim(game_desc);
                trim(game_password);

                validate_start_game_fields(errorMsg, game_name, game_desc, game_players, game_quickturns);
                if (strlen(errorMsg) == 0) {
                    new_game_form = malloc(sizeof(NewGameForm));
                    memset(new_game_form, 0, sizeof(NewGameForm));
                    strcpy(new_game_form->name, game_name);
                    strcpy(new_game_form->desc, game_desc);
                    strcpy(new_game_form->password, game_password);
                    strcpy(new_game_form->map, "First Blood");
                    new_game_form->num_players = atoi(game_players);
                    new_game_form->num_autoturns = atoi(game_quickturns);

                    create_game(new_game_form);
                    sprintf(result_message, "{BG BLACK}{FG CYAN}A new game named {FGB MAGENTA}%s{FG CYAN} was created.", game_name);

                    free(new_game_form);
                    keep_looping = FALSE;

                    break;
                }
                msg_len = (80 - real_len(errorMsg)) / 2;
                sprintf(temp, "{BG BLACK}{CPOS 17 11}                                                             }{FGB RED}{BG BLACK}{BELL}{CPOS 17 %d}%s", msg_len, errorMsg);
                output(temp);

                break;
        }
    } while (keep_looping);
    output("{CLEARROWS 9 23 BLACK}");
    free(temp);
}
