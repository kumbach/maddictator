#ifndef CNETDOOR_MAP_RENDERER_H
#define CNETDOOR_MAP_RENDERER_H

#include "base.h"

void init_renderer(GameData *game_data, int width, int height);
void cleanup_renderer(void);
void render_map(void);
BOOL scroll_map(int ox, int oy);

#endif //CNETDOOR_MAP_RENDERER_H
