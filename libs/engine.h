#ifndef FLOOR_H
#define FLOOR_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdbool.h>
#include "rect.h"
#include "lista.h"
#include "types.h"
#include "entity.h"

anchor_t *anchor_add(anchor_t *a, entity_t *e);
void anchor_anchor(anchor_t *a, anchor_t *b);
void anchor_delete(anchor_t *a);

void entities_empty(entities_t **e);

void engine_create(engine_t **e, SDL_Renderer *r);
void engine_draw(engine_t *e);
void engine_set_tileset(engine_t *e, char *fileName, int columns, int tile_width, int tile_height);
void engine_set_screen(engine_t *e, int x, int y, int w, int h);
void engine_place_entity(engine_t *e, int row, int col, entity_t *entity);
void engine_set_playground(engine_t *e, uint32_t x, uint32_t y);
void engine_load_mosaic(engine_t *e, char *fileName);
void engine_debug(engine_t *e, bool debug);
void engine_iso_move(SDL_Rect *rect, int ix, int iy);
void engine_add_entity(engine_t *e, entity_t *entity);
void engine_iso_cord(engine_t *e, int ix, int iy, int *x, int *y);
void engine_iso_tile(engine_t *e, int ix, int iy, int *row, int*col, int *ixx, int *iyy);
void engine_collisions(engine_t *e, entity_t *ee, entities_t **c);
tile_t *engine_tile(engine_t *e, int row, int col);
void engine_show_screen_rect(engine_t *e, bool show);
int engine_tile_width(engine_t *e);
int engine_tile_height(engine_t *e);
uint32_t engine_mosaic_rows(engine_t *e);
uint32_t engine_mosaic_cols(engine_t *e);
void engine_free(engine_t *e);
#endif
