#define SDL_MAIN_USE_CALLBACKS 1 
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<unistd.h>
#include<stdbool.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static int xval = 100;
static int yval = 100;
static int diff = 100;
static int i=0;
static bool movdown = true;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argb[]){
	window = SDL_CreateWindow("tetris", 700,700,0);
	renderer = SDL_CreateRenderer(window,NULL);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
	if(event->type == SDL_EVENT_QUIT){
		return SDL_APP_SUCCESS;
	}if(event->type == SDL_EVENT_KEY_DOWN){
		if(event->key.key == SDLK_RIGHT){
			if(xval<600 && movdown){
				xval+=100;
			}
		}else if(event->key.key == SDLK_LEFT){
			if(xval>0 && movdown){
				xval-=100;
			}
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
	SDL_RenderClear(renderer);
	SDL_Vertex rectvert[4] = {
	    { {xval,yval}, {255,0,0,0}, {0, 0} }, 
	    { {xval+diff, yval}, {255,0,0,0}, {1, 0} }, 
	    { {xval+diff, yval+diff}, {255,0,0,0}, {1, 1} },
	    { {xval, yval+diff}, {255,0,0,0}, {0,1}},
	};
	int index[6] = {0,1,2,0,2,3};
	SDL_RenderGeometry(renderer,NULL,rectvert,4,index,6);
	SDL_RenderPresent(renderer);
	if(movdown && i++>2000){
		yval+=100;
		i=0;
		if(yval>500){
			movdown=false;
		}
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
}
