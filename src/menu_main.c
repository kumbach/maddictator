#include "base.h"
#include "menu_main.h"
#include "input.h"
#include "main.h"
#include "menu_new_game.h"
#include "game_select_menu.h"

#define START_ROW 14
#define MAX_ROWS 8

GameFile **game_view_list = NULL;
int num_games = 0;
int selected_game_index = 0;

void draw_game_list_ui(int game_type) {
    output("{FGB BLUE}{WINDOW 13 2 22 79 BLUE BLACK}{CPOS 13 3}{BG BLUE}{FGB BLUE}");

    switch (game_type) {
        case 0:
            output("GAME TITLE            ROUND  STATUS");
            break;
        case 1:
            output("GAME TITLE            PLAYERS  DETAILS");
            break;
        case 2:
            output("GAME TITLE            SUMMARY DETAILS");
            break;
    }

    output("{CPOS 22 13}{BG BLUE}{FGB BLUE}List Games: ");
    output("[M]y Games ");
    output("[J]oinable Games ");
    output("[C]ompleted Games");

    output("{FGB BLUE{BG BLACK}{CPOS 23 15}");
    output("Use cursor {FGB WHITE}UP/DOWN{FGB BLUE} and {FGB WHITE}ENTER{FGB BLUE} to select a game.");
}

void show_games(int game_type, int selected_game_index, BOOL redraw_game_ui) {
    int i;
    int row;
    int start_index = 0;
    int end_index;
    char *buf;

    if (redraw_game_ui) {
       draw_game_list_ui(game_type);
    }
    free_game_list(game_view_list, num_games);
    game_view_list = NULL;

    game_view_list = get_game_list(&num_games, game_type);

    row = START_ROW;
    buf = malloc(256);

    if (num_games == 0) {
        if (game_type == 0) {
            sprintf(buf, "{CPOS %d 3}{FGB WHITE}{BG BLACK}You are not currently in any games. Join or start a game!             ", row);
        }
        else if (game_type == 1) {
            sprintf(buf, "{CPOS %d 3}{FGB WHITE}{BG BLACK}There are no open games to join right now. Why wait, start a new game!", row);
        }
        else if (game_type == 2) {
            sprintf(buf, "{CPOS %d 3}{FGB WHITE}{BG BLACK}There are no completed games to view.                                 ", row);
        }
        output(buf);
        free(buf);
        return;
    }

    if (selected_game_index >= MAX_ROWS) {
        start_index = selected_game_index - MAX_ROWS + 1;
    }

    end_index = start_index + MAX_ROWS - 1;
    if (end_index > num_games) {
        end_index = num_games - 1;
    }

    for (i=start_index; i <= end_index; ++i) {
        if (i == selected_game_index) {
            output("{FG BLACK}{BG CYAN}");
        }
        else {
            output("{FG CYAN}{BG BLACK}");
        }

        switch (game_type) {
            case 0:
                sprintf(buf, "{CPOS %d 3}%-*s  %5d  %s                              ",
                        row,
                        GAME_NAME_LEN,
                        game_view_list[i]->name,
                        game_view_list[i]->round_number,
                        game_view_list[i]->state == IN_PROGRESS ? "In Progress" : "Open"
                );
                break;
            case 1:
                sprintf(buf, "{CPOS %d 3}%-*s    %d/%d    %-*s     ",
                        row,
                        GAME_NAME_LEN, game_view_list[i]->name,
                        num_joined_players(game_view_list[i]),
                        game_view_list[i]->num_players,
                        GAME_DESC_LEN, game_view_list[i]->desc
                );
                break;
            case 2:
                sprintf(buf, "{CPOS %d 3}%-*s    ", row,
                        GAME_NAME_LEN+2, game_view_list[i]->name
                );

                break;
        }
        output(buf);
        row++;
    }

    free(buf);
}

void show_main_menu(void) {
    char key;
    char *temp;
    char *result_message;
    int len;

    int cur_game_type = 0;
    BOOL redraw = TRUE;
    BOOL redraw_title = TRUE;

    temp = malloc(256);
    result_message = malloc(256);
    memset(result_message, 0, 256);

    do {
        if (redraw_title) {
            output("{RESET}{CLS}");
            outputfile("MAD_DICT:art/title.ans");

            redraw_title = FALSE;
        }

        if (redraw) {
            output("{CLEARROWS 9 23 BG BLACK}");

            output("{CPOS 11 20}{FGB BLUE}");
            output("[N]ew Game ");
            output("[S]ettings ");
            output("[H]elp ");
            output("[Q]]uit to BBS");

            show_games(cur_game_type, selected_game_index, TRUE);

            sprintf(&temp[0], "{CPOS 9 2}{FG GREEN}{BG BLACK}Welcome {FG MAGENTA}%s{FG GREEN}, the battlefield awaits. Choose an option:", user_context.handle);
            output(temp);

            redraw = FALSE;
        }

        output("{CLEARROWS 12 12 BLACK}");
        if (strlen(result_message) > 0) {
            int n;
            len = real_len(result_message);
            n = (80 - len) / 2;

            sprintf(temp, "{BELL}{CPOS 12 %d}%s",n, result_message);
            output(temp);
            result_message[0] = 0;
        }
        output("{CPOS 9 60}{FGA GREEN}{BG BLACK}");

        key = getkey();
        switch (key) {
            case 'N':
                do_new_game_menu(result_message);
                redraw=TRUE;
                break;

            case 'M':
                cur_game_type = 0;
                selected_game_index  = 0;
                show_games(cur_game_type, selected_game_index, TRUE);
                break;

            case 'J':
                cur_game_type = 1;
                selected_game_index  = 0;
                show_games(cur_game_type, selected_game_index, TRUE);
                break;

            case 'C':
                cur_game_type = 2;
                selected_game_index  = 0;
                show_games(cur_game_type, selected_game_index, TRUE);
                break;

            case 'S':
                strcpy(result_message, "{FGB RED}{BG BLACK}Sorry, there are no settings yet.");
                break;

            case 'H':
                show_help_file("main_menu.help", 9, 5, 22, 74);
                redraw = TRUE;
                break;

            case 13:
                if (num_games == 0) break;
                game_select_menu(cur_game_type, game_view_list[selected_game_index], result_message);
                redraw = TRUE;
                redraw_title = TRUE;
                break;

            case 27:
                key = getkey();
                if (key == 91) {
                    key = getkey();
                    if (key == 'A') {
                        if (selected_game_index > 0) {
                            selected_game_index--;
                            show_games(cur_game_type, selected_game_index, FALSE);
                        }
                    }
                    else if (key == 'B') {
                        if (selected_game_index < num_games -1) {
                            selected_game_index++;
                            show_games(cur_game_type, selected_game_index, FALSE);
                        }
                    }
                }
                break;
            default:
                sprintf(temp, "{FGB WHITE}{BG BLACK}{CPOS 1 1}%d", key);
                output(temp);
                break;

        }
    }  while (key != 'Q');

    free_game_list(game_view_list, num_games);
    game_view_list = NULL;

    free(temp);
    free(result_message);
}

