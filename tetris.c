#define SDL_MAIN_USE_CALLBACKS 1 
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>

#define SHAPECOUNT 100
#define MAXX 600
#define MAXY 600
#define MAXFRAMES 2000

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static int frames = 0;
static bool movdown[SHAPECOUNT];
static int current = 0;

struct cshape{
	int xval;
	int yval;
	int diff;
};

void initializearr(struct cshape* allshapes){
	for(int i=0;i<SHAPECOUNT;i++){
		(allshapes+i)->diff = 50;
		(allshapes+i)->xval = MAXX/2;
		(allshapes+i)->yval = -((allshapes+i)->diff);
		movdown[i] = true;
	}
}


int checkcollision(int thecase, struct cshape* allshapes){
	if(current==0){ return 0; }
	for(int i=0;i<current;i++){
		switch(thecase){
			case 0:
				if(((((allshapes+i)->xval < (allshapes+current)->xval && (allshapes+current)->xval < (allshapes+i)->xval+2*(allshapes+current)->diff) ||
				((allshapes+current)->xval < (allshapes+i)->xval && (allshapes+i)->xval < (allshapes+current)->xval+2*(allshapes+current)->diff)) ||
				((allshapes+current)->xval == (allshapes+i)->xval)) &&
				((allshapes+i)->yval == (allshapes+current)->yval+(allshapes+current)->diff))
				{ return 1; }
				break;
			case 1:
				if(((allshapes+i)->xval < (allshapes+current)->xval+(3*(allshapes+current)->diff)) &&
				((allshapes+current)->xval+(3*(allshapes+current)->diff) < (allshapes+i)->xval+(2*(allshapes+i)->diff)) &&	
				(allshapes+i)->yval == (allshapes+current)->yval)
				{ return 1; }
				break;
			case 2:
				if(((allshapes+i)->xval < (allshapes+current)->xval-(allshapes+current)->diff && 
				(allshapes+current)->xval-(allshapes+current)->diff < (allshapes+i)->xval+2*(allshapes+current)->diff) && 
				(allshapes+i)->yval == (allshapes+current)->yval)
				{ return 1; }
				break;
		}
	}
	return 0;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char* argb[]){
	window = SDL_CreateWindow("tetris", 700,700,0);
	renderer = SDL_CreateRenderer(window,NULL);
	struct cshape *allshapes = malloc(sizeof(struct cshape)*SHAPECOUNT);
	initializearr(allshapes);
	*appstate = allshapes;
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
	
	struct cshape* allshapes = (struct cshape*)appstate;

	if(event->type == SDL_EVENT_QUIT){
		return SDL_APP_SUCCESS;
	}if(event->type == SDL_EVENT_KEY_DOWN){
		if(event->key.key == SDLK_RIGHT){
			if((allshapes+current)->xval<MAXX && movdown[current] && checkcollision(1,allshapes)==0){
				(allshapes+current)->xval+=(allshapes+current)->diff;
			}
		}else if(event->key.key == SDLK_LEFT){
			if((allshapes+current)->xval>0 && movdown[current] && checkcollision(2,allshapes)==0){
				(allshapes+current)->xval-=(allshapes+current)->diff;
			}
		}else if(event->key.key == SDLK_DOWN){
			while((allshapes+current)->yval<MAXY && checkcollision(0,allshapes)==0){
				(allshapes+current)->yval+=(allshapes+current)->diff;
				if(checkcollision(0,allshapes)==1 || (allshapes+current)->yval>MAXY){
					movdown[current] = false;
					current++;
					break;
				}
			}
			
		}
	}
	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){
	SDL_RenderClear(renderer);

	struct cshape* allshapes = (struct cshape*)appstate;

	int index[6] = {0,1,2,0,2,3};
	for(int i = 0; i < SHAPECOUNT; i++){
		SDL_Vertex rectvert[4] = {
		{ {(allshapes+i)->xval,(allshapes+i)->yval}, {255,0,0,0}, {0, 0} },
		{ {(allshapes+i)->xval+2*(allshapes+current)->diff, (allshapes+i)->yval}, {255,0,0,0}, {1, 0} },
		{ {(allshapes+i)->xval+2*(allshapes+current)->diff, (allshapes+i)->yval+(allshapes+current)->diff}, {255,0,0,0}, {1, 1} },
		{ {(allshapes+i)->xval, (allshapes+i)->yval+(allshapes+current)->diff}, {255,0,0,0}, {0,1}},
		};
		SDL_RenderGeometry(renderer,NULL,rectvert,4,index,6);
	}
	SDL_RenderPresent(renderer);
	if(movdown[current] && frames++>MAXFRAMES){
		(allshapes+current)->yval+=(allshapes+current)->diff;
		frames=0;
		if((allshapes+current)->yval>MAXY || checkcollision(0,allshapes)==1){
			movdown[current]=false;
			current++;
		}
	}
	if((current == SHAPECOUNT && !movdown[current]) || (allshapes+current)->yval<-(allshapes+current)->diff){
		printf("OVER\n");
		sleep(2);
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result){
}
