#define SDL_MAIN_USE_CALLBACKS 1 
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>

#include "config.h"
#include "game.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static unsigned int prevt = 0;

int current = 0;

static int squarep[] = {0,1,2,0,3,2};

SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argb[]){
	window = SDL_CreateWindow("tetris", MAXX, MAXY, 0);
	renderer = SDL_CreateRenderer(window,NULL);
	srand(time(NULL));

	struct cshape *allshapes = malloc(sizeof(struct cshape)*SHAPECOUNT);
	initializearr(allshapes);
	*appstate = allshapes;
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
	
	struct cshape* allshapes = (struct cshape*)appstate;

	if(event->type == SDL_EVENT_QUIT){
		return SDL_APP_SUCCESS;
	}
	
	if(event->type == SDL_EVENT_KEY_DOWN){
		if(event->key.key == SDLK_RIGHT){
			int rightmost = 0;
			for(int i=0; i < (allshapes+current)->indcount; i++){
				int xidiff = (allshapes+current)->indices[i] % MAXCOLS;
				int rightval = (allshapes+current)->xval + (xidiff+1)*GENDIFF;
				if(rightval  > rightmost){
					rightmost = rightval;
				}
			}
			if(rightmost + GENDIFF <= MAXX && (allshapes+current)->movdown && checkcollision(1,allshapes)==0){
				(allshapes+current)->xval += GENDIFF;
			}
		}else if(event->key.key == SDLK_LEFT){
			if((allshapes+current)->xval - GENDIFF >= 0 && (allshapes+current)->movdown && checkcollision(2,allshapes)==0){
				(allshapes+current)->xval -= GENDIFF;
			}
		}else if(event->key.key == SDLK_DOWN){
			while(checkcollision(-1,allshapes)==0 && checkcollision(0,allshapes)==0){
				(allshapes+current)->yval += GENDIFF;
			}
			(allshapes+current)->movdown = false;
			current++;
			checkrows(allshapes);
			
		}
	}
	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){
	SDL_RenderClear(renderer);

	struct cshape* allshapes = (struct cshape*)appstate;

	for(int i = 0; i < SHAPECOUNT; i++){
		for(int index=0; index < (allshapes+i)->indcount; index++){	
			int xdiff = (allshapes+i)->indices[index] % MAXCOLS;
			int ydiff = (allshapes+i)->indices[index] / MAXCOLS;
			SDL_Vertex rectvert[4] = {
			{{(allshapes+i)->xval + xdiff*GENDIFF, (allshapes+i)->yval + ydiff*GENDIFF}, {255,0,0,255}, {0, 0} },
			{{(allshapes+i)->xval + (xdiff+1)*GENDIFF, (allshapes+i)->yval + ydiff*GENDIFF}, {255,0,0,255}, {1, 0} },
			{{(allshapes+i)->xval + (xdiff+1)*GENDIFF, (allshapes+i)->yval + (ydiff+1)*GENDIFF}, {255,0,0,255}, {1, 1} },
			{{(allshapes+i)->xval + xdiff*GENDIFF, (allshapes+i)-> yval + (ydiff+1)*GENDIFF}, {255,0,0,255}, {0, 1} },	
			};
			SDL_RenderGeometry(renderer,NULL,rectvert,4,squarep,6);
		}
	}

	SDL_RenderPresent(renderer);
	unsigned int currt = SDL_GetTicks();
	if((allshapes+current)->movdown && (currt > prevt + MAXTICKS)){
		(allshapes+current)->yval += GENDIFF;
		if(checkcollision(-1,allshapes)==1 || checkcollision(0,allshapes)==1){
			(allshapes+current)->movdown=false;
			current++;
			checkrows(allshapes);
		}
		prevt = currt;
	}
	if((current == SHAPECOUNT && !(allshapes+current)->movdown)){
		printf("OVER\n");
		sleep(2);
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
}
