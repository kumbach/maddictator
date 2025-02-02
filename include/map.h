//
// Created by Kevin on 2024-05-29.
//

#ifndef CNETDOOR_MAP_H
#define CNETDOOR_MAP_H

#include "md_structs.h"

MapFile ** map_list(void);
BOOL *init_game_data(char *map_name, GameData *game_data);

#endif //CNETDOOR_MAP_H
