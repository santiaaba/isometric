#include "libs/isometric.h"
#include "libs/engine.h"

int main(char argc, char * argv[]){
	engine_t *engine;
	
	SDL_Event event;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *demo;
	bool end;

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

	demo = IMG_LoadTexture(renderer,"tiles.png");

	engine_create(&engine,renderer);
	//engine_debug(engine,true);
	engine_set_tileset(engine,"tiles.png",2,32,64);
	engine_load_mosaic(engine,"level1.data");
	engine_set_screen(engine,50,50,600,400);

	engine_set_playground(engine,0,0);

	end = false;
	while(!end){
		while(SDL_PollEvent(&event))
			if(event.type == SDL_WINDOWEVENT)
				if(event.window.event == SDL_WINDOWEVENT_CLOSE)
					end = true;

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

		printf("Llego\n");
		engine_draw(engine);
		SDL_Delay(1000);
	}

	SDL_DestroyRenderer(renderer);
	engine_free(engine);
	return 0;
}
