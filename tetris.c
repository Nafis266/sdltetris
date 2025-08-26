#define SDL_MAIN_USE_CALLBACKS 1 
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<unistd.h>
#include<stdbool.h>

#define SHAPECOUNT 10

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static int xval[SHAPECOUNT];
static int yval[SHAPECOUNT];
static int diff = 100;
static int frames = 0;
static bool movdown[SHAPECOUNT];
static int cshape = 0;

void initializearr(){
	for(int i=0;i<SHAPECOUNT;i++){
		xval[i] = 300;
		yval[i] = -100;
		movdown[i] = true;
	}
}

int checkcollision(int current){
	if(cshape==0){ return 0; }
	for(int i=0;i<current;i++){
		if(xval[i] == xval[current] && yval[i] == yval[current]+100){
			return 1;
		}
	}
	return 0;
}

int xrightcollision(int current){
	if(current==0){ return 0; }
	for(int i=0;i<current;i++){
		if(xval[i] == xval[current]+100 && yval[i] == yval[current]){
			return 1;
		}
	}
	return 0;
}

int xleftcollision(int current){
	if(current==0){ return 0; }
	for(int i=0;i<current;i++){
		if(xval[i] == xval[current]-100 && yval[i] == yval[current]){
			return 1;
		}
	}
	return 0;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argb[]){
	window = SDL_CreateWindow("tetris", 700,700,0);
	renderer = SDL_CreateRenderer(window,NULL);
	initializearr();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
	if(event->type == SDL_EVENT_QUIT){
		return SDL_APP_SUCCESS;
	}if(event->type == SDL_EVENT_KEY_DOWN){
		if(event->key.key == SDLK_RIGHT){
			if(xval[cshape]<600 && movdown[cshape] && xrightcollision(cshape)==0){
				xval[cshape]+=100;
			}
		}else if(event->key.key == SDLK_LEFT){
			if(xval[cshape]>0 && movdown[cshape] && xleftcollision(cshape)==0){
				xval[cshape]-=100;
			}
		}
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate){
	SDL_RenderClear(renderer);

	int index[6] = {0,1,2,0,2,3};
	for(int shape = 0; shape < SHAPECOUNT; shape++){
		SDL_Vertex rectvert[4] = {
		{ {xval[shape],yval[shape]}, {255,0,0,0}, {0, 0} },
		{ {xval[shape]+diff, yval[shape]}, {255,0,0,0}, {1, 0} },
		{ {xval[shape]+diff, yval[shape]+diff}, {255,0,0,0}, {1, 1} },
		{ {xval[shape], yval[shape]+diff}, {255,0,0,0}, {0,1}},
		};
		SDL_RenderGeometry(renderer,NULL,rectvert,4,index,6);
	}
	SDL_RenderPresent(renderer);
	if(movdown[cshape] && frames++>2000){
		yval[cshape]+=100;
		frames=0;
		if(yval[cshape]>500 || checkcollision(cshape)==1){
			movdown[cshape]=false;
			cshape++;
		}
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
}
