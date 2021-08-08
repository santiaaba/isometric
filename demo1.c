#include "libs/engine.h"
#include "libs/entity.h"

int main(char argc, char * argv[]){
	engine_t *engine;
	
	SDL_Event event;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *rojo_texture;
	SDL_Texture *azul_texture;
	SDL_Texture *character_texture;
	entity_t *rojo;
	entity_t *azul;
	entity_t *character;
	int go_to;
	int playgroundy, x, y, ix, iy;
	bool onclick = false;
	
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
	engine_debug(engine,true);
	engine_set_tileset(engine,"tiles_128.png",2,128,64);
	engine_load_mosaic(engine,"level1.data");
	engine_set_screen(engine,100,100,800,600);
	engine_show_screen_rect(engine,true);

	rojo_texture = IMG_LoadTexture(renderer,"tocho_rojo.png");
	azul_texture = IMG_LoadTexture(renderer,"tocho_azul.png");
	character_texture = IMG_LoadTexture(renderer,"character1.png");

	printf("ACA\n");
	/* Tocho Rojo */
/*
	entity_create(&rojo,engine);
	entity_add_texture(rojo,rojo_texture,128,159,0,0,64,64,64);
	entity_position_set(rojo,69,300);

	/* Tocho Azul */
/*
	entity_create(&azul,engine);
	entity_add_texture(azul,azul_texture,256,189,0,0,128,128,128);
	entity_position_set(azul,162,182);

	/* Character */
/*
	entity_create(&character,engine);
	entity_add_texture(character,character_texture,45,83,0,0,17,17,17);
	entity_position_set(character,290,186);
*/


	end = false;
	go_to = 0;
	playgroundy = -25;
	engine_set_playground(engine,-250,playgroundy);
	while(!end){
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_WINDOWEVENT)
				if(event.window.event == SDL_WINDOWEVENT_CLOSE)
					end = true;
			if(event.type == SDL_MOUSEBUTTONDOWN && onclick == false){
				SDL_GetMouseState( &x, &y );
				onclick = true;
				if(event.button.button == SDL_BUTTON_LEFT)
					printf("Click: (%i,%i)\n",x,y);
					if(engine_into_screen(engine,x,y,&ix,&iy)){
						printf("Click: isometrico (%i,%i)\n",ix,iy);
					} else {
						printf("Click fuera del screen\n");
					}
			}
			if(event.type == SDL_MOUSEBUTTONUP)
				onclick = false;

			if(event.type == SDL_KEYDOWN)
				switch(event.key.keysym.sym){
					case SDLK_KP_1:
						entity_position_add(character,0,1);
						break;
					case SDLK_KP_2:
						entity_position_add(character,1,1);
						break;
					case SDLK_KP_3:
						entity_position_add(character,1,0);
						break;
					case SDLK_KP_4:
						entity_position_add(character,-1,1);
						break;
					case SDLK_KP_6:
						entity_position_add(character,1,-1);
						break;
					case SDLK_KP_7:
						entity_position_add(character,-1,0);
						break;
					case SDLK_KP_8:
						entity_position_add(character,-1,-1);
						break;
					case SDLK_KP_9:
						entity_position_add(character,0,-1);
						break;
				}
		}

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
		SDL_RenderClear(renderer);

		engine_draw(engine);
	}

	SDL_DestroyRenderer(renderer);
	engine_free(engine);
	return 0;
}
