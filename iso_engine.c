/* Motor grafico isometrico */

#include "iso_engine.h"

void iso_init(iso_t *i){
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      SDL_LogError(	SDL_LOG_CATEGORY_APPLICATION,
							"Couldn't initialize SDL: %s",
							SDL_GetError());
      return 3;
   }

	i->windows = SDL_CreateWindow("SDL_CreateTexture",
         SDL_WINDOWPOS_UNDEFINED,
         SDL_WINDOWPOS_UNDEFINED,
         1024, 768,
         SDL_WINDOW_RESIZABLE);

	i->renderer = SDL_CreateRenderer(i->windows, -1, 0);
}

void iso_draw(iso_t *i){
	floor_draw(i->floor);
}

floor_t *iso_floor(iso_t *i){
	return i->floor;
}

void iso_free(iso_t *i){
	SDL_DestroyRenderer(i->renderer);
   SDL_Quit();
}
