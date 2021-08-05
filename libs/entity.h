#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "point.h"
#include "vector.h"
#include "engine.h"
#include "types.h"

void entity_create(entity_t **e, engine_t *engine);
void entity_draw(entity_t *e, SDL_Rect *tile);
void entity_add_texture(entity_t *e, SDL_Texture *texture, int w,
								int h, int x, int y, int x_base);
void entity_position_set(entity_t *e, int ix, int iy);
void entity_position_add(entity_t *e, int addix, int addiy);
void entity_cut(entity_t *e, int row, int col);
void entity_free(entity_t *e);
int entity_ixx(entity_t *e);
int entity_iyy(entity_t *e);

#endif
