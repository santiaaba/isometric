#include "libs/engine.h"
#include "libs/entity.h"

int main(char argc, char * argv[]){
	engine_t *engine;
	
	SDL_Event event;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *casita_texture;
	entity_t *casita;
	int go_to;
	int playgroundx;
	
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

	engine_create(&engine,renderer);
	//engine_debug(engine,true);
	engine_set_tileset(engine,"tiles_128.png",2,128,64);
	//engine_set_tileset(engine,"tiles.png",2,64,32);
	engine_load_mosaic(engine,"level1.data");
	engine_set_screen(engine,50,50,800,600);
	engine_show_screen_rect(engine,true);

	casita_texture = IMG_LoadTexture(renderer,"casita.png");

	/* Creamos casita 1 */
	entity_create(&casita,renderer,128,64);
	entity_add_texture(casita,casita_texture,145,177,44,0,0,0,64);
	engine_add_entity(engine,3,3,casita);

	/* Creamos casita 2 */
	entity_create(&casita,renderer,128,64);
	entity_add_texture(casita,casita_texture,145,177,44,0,0,0,64);
	engine_add_entity(engine,3,4,casita);

	/* Creamos casita 3 */
	entity_create(&casita,renderer,128,64);
	entity_add_texture(casita,casita_texture,145,177,44,0,0,0,64);
	engine_add_entity(engine,3,6,casita);

	end = false;
	go_to = 0;
	playgroundx = -125;
	engine_set_playground(engine,-125,-57);
	while(!end){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_WINDOWEVENT)
				if(event.window.event == SDL_WINDOWEVENT_CLOSE)
					end = true;

			if(event.type == SDL_MOUSEBUTTONDOWN){
				printf("Boton presionado\n");
				go_to = 40;
			}
		}
		
		if(go_to > 0){
			engine_set_playground(engine,playgroundx,-57);
			go_to --;
			playgroundx -= 3;
		}

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

		engine_draw(engine);
		SDL_Delay(30);
	}

	SDL_DestroyRenderer(renderer);
	engine_free(engine);
	return 0;
}
