#include "game_main_ui.h"

#define MAX_COMMAND_LEN 30

enum CommandResult {
    EXIT,
    OK,
    OK_REDRAW_MAP,
    OK_REDRAW
};

GameFile *game_file;
char ui_buf[160];

void draw_frame(void) {
    output("{CLS}");
    output("{RECT 1 1 20 79 BLUE}\n");
    output("{HBAR 3 1 79 BLUE}\n");
    output("{FG CYAN}{CPOS 1 2}");
    output(game_file->name);

    output("{VBAR 60 3 20 BLUE}");
    output("{HLINE 2 2 78 BLUE}");
}

void draw_player_status(void) {
    int i;
    output("{CPOS 2 2}{BG BLUE}{FG CYAN}Turn: {FGB WHITE}1");
    output("{CPOS 2 47}{FG CYAN}Mode: {FGB WHITE}Orders");

    // draw player handles, highlighting the player whose turn it is
    output("{CPOS 2 12}{FG CYAN}Players: ");
    for (i=0; i < game_file->num_players; ++i) {
        output(i == game_file->cur_player_index ? "{BG WHITE}{FG BLUE} " : "{BG BLUE}{FG WHITE} ");
        output(game_file->player_handles[i]);
        output(" {BG BLUE}{FG WHITE} ");
    }
}

char get_command() {
    output("{CPOS 21 2}{BG BLACK}{FG GREEN}Command (?=help):");
    return getkey();
}

enum CommandResult process_command(char command) {
    switch (command ) {
        case 'Q':
            return EXIT;
        case '?':
            show_help_file("commands.help", 4, 5, 21, 74);
            return OK_REDRAW;

        case '1':
            scroll_map(1, -1);
            return OK_REDRAW_MAP;
        case '2':
            scroll_map(1, 0);
            return OK_REDRAW_MAP;
        case '3':
            scroll_map(1, 1);
            return OK_REDRAW_MAP;

        case '7':
            scroll_map(-1, -1);
            return OK_REDRAW_MAP;
        case '8':
            scroll_map(-1, 0);
            return OK_REDRAW_MAP;
        case '9':
            scroll_map(-1, 1);
            return OK_REDRAW_MAP;

        case '4':
            scroll_map(0, -1);
            return OK_REDRAW_MAP;
        case '6':
            scroll_map(0,1);
            return OK_REDRAW_MAP;
        case 'I':
            sprintf(ui_buf, "{CPOS 4 2}{FG WHITE}{BG BLUE}cur_idx:%d", game_file->cur_player_index);
            output(ui_buf);
        default:
            break;
    }

    return OK;
}

void play_game(int game_id, char *game_name) {
    char cmd_key;
    char *temp;
    GameData game_data;
    enum CommandResult result = OK_REDRAW;

    temp = malloc(255);

    game_file = malloc(sizeof(GameFile));
    sprintf(temp, "MAD_DICT:data/games/%d.game", game_id);
    if (!read_game_file(temp, game_file)) {
        free(temp);
        output("{CLR}{FGB RED}Game file error. Press any key...");
        getkey();
        return;
    }

    if (init_game_data(game_file->map, &game_data) == NULL) {
        output("{CLR}{FGB RED}Map error. Press any key...");
        getkey();
        return;
    }
    init_renderer(&game_data, 16, 58);

    do {
        if (result == OK_REDRAW) {
            draw_frame();
            draw_player_status();
            render_map();
            result = OK;
        }
        if (result == OK_REDRAW_MAP) {
            render_map();
            result = OK;
        }

        output("{CLEARCOLS 21 19 79 BLACK}");
        cmd_key = get_command();
        result = process_command(cmd_key);
    } while (result != EXIT);

    cleanup_renderer();
    free(temp);
    free(game_file);
}