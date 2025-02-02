//
// Created by Kevin on 2024-05-25.
//

#ifndef CNETDOOR_MAIN_MENU_H
#define CNETDOOR_MAIN_MENU_H

#include "screen.h"
#include "ansiescaper.h"
#include "input.h"
#include "md_structs.h"
#include "game_main_ui.h"

void show_main_menu(void);
GameFile *join_a_game(void);
GameFile *continue_a_game();

#endif //CNETDOOR_MAIN_MENU_H
