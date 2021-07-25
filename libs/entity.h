#ifndef ENTITY_H
#define ENTITY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "isometric.h"
#include "point.h"
#include "vector.h"
#include "border.h"

typedef struct {
	point_t *position;	/* Ancla */
	vector_t *direction;
	SDL_Texture *texture;
	SDL_Rect *rect;		/* Para recortar la textura */
	SDL_Renderer *renderer;
} entity_t

void entity_init(entity_t *e);
void entity_set_direction(entity_t *e, vector_t *v);
bool entity_collision(entity_t *e, border_t *b);
void entity_draw(entity_t *e);
vector_t *entity_get_direction(entity_t *e);
void entity_free(entity_t *e);

#endif
