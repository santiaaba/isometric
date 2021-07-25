#include "lib/isometric.h"
#include "lib/engine.h"

void main(char argc, char * argv[]){
	engine_t *engine;
	
	SDL_Window *window;
	SDL_Renderer *renderer;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(	SDL_LOG_CATEGORY_APPLICATION,
							"Couldn't initialize SDL: %s",
							SDL_GetError());
		return 3;
	}

	window = SDL_CreateWindow(
					"SDL_CreateTexture",
					SDL_WINDOWPOS_UNDEFINED,
					SDL_WINDOWPOS_UNDEFINED,
					1024, 768,
					SDL_WINDOW_RESIZABLE);

	renderer = SDL_CreateRenderer(window, -1, 0);

	engine_init(engine,renderer);
	engine_set_tileset("tiles.png",2,32,64);
}
