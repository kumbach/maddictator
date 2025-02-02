#ifndef CNETDOOR_MAP_RENDERER_H
#define CNETDOOR_MAP_RENDERER_H

#include "base.h"

void init_renderer(GameData *game_data, int rows, int cols);
void cleanup_renderer(void);
void render_map(void);
BOOL scroll_map(int rows, int cols);

#endif //CNETDOOR_MAP_RENDERER_H
