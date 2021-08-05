#include "isometric.h"

void iso_cut(engine_t *e, SDL_Rect *tile){
	/* Recorte superior */
   if(e->playground.y > tile->y){
      tile->h = tile->y + e->tile_height - e->playground.y;
      tile->y = e->playground.y;
      e->tileset_cut.y += (e->tile_height - tile->h);
   }
   /* Recorte izquierdo */
   if(e->playground.x > tile->x){
      tile->w = tile->x + e->tile_width - e->playground.x;
      tile->x = e->playground.x;
      e->tileset_cut.x += (e->tile_width - tile->w);
   }
   /* Recorte inferior */
   if((e->playground.y + e->playground.h) < (tile->y + e->tile_height)){
      tile->h = e->tile_height - (tile->y + e->tile_height -(e->playground.y + e->playground.h));
   }
   /* Recorte derecho */
   if((e->playground.x + e->playground.w) < (tile->x + e->tile_width)){
      tile->w = e->tile_width - (tile->x + e->tile_width -(e->playground.x + e->playground.w));
   }

	e->tileset_cut.h = tile->h;
   e->tileset_cut.w = tile->w;
}
