#define SDL_MAIN_USE_CALLBACKS 1 
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>

#define SHAPECOUNT 100
#define MAXX 600
#define MAXY 700
#define MAXFRAMES 2000
#define GENDIFF 50

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static int frames = 0;
static int current = 0;

static int squarep[] = {0,1,2,0,3,2};
static int linep[] = {0,6,5,0,4,5};

struct cshape{
	int xval;
	int yval;
	int xdiff;
	int ydiff;
	bool movdown;
	int* indices;
};

void setpoints(int** points, int n){
	if(n%2==0){
		*points = squarep;
		return;
	}
	*points = linep;
}

void initializearr(struct cshape* allshapes){
	for(int i=0;i<SHAPECOUNT;i++){
		(allshapes+i)->xdiff = (i%2==0 ? GENDIFF*2 : GENDIFF);
		(allshapes+i)->ydiff = (i%2==0 ? GENDIFF : GENDIFF*3); 
		(allshapes+i)->xval = MAXX/2;
		(allshapes+i)->yval = -((allshapes+i)->ydiff); 
		(allshapes+i)->movdown = true;
		setpoints(&(allshapes+i)->indices, i);
	}
}


int checkcollision(int thecase, struct cshape* allshapes){
	if(current==0){ return 0; }
	for(int i=0;i<current;i++){
		switch(thecase){
			case 0:
				if(((((allshapes+i)->xval < (allshapes+current)->xval && (allshapes+current)->xval < (allshapes+i)->xval + (allshapes+i)->xdiff) ||
				((allshapes+current)->xval < (allshapes+i)->xval && (allshapes+i)->xval < (allshapes+current)->xval + (allshapes+current)->xdiff)) ||
				((allshapes+current)->xval == (allshapes+i)->xval)) &&
				((allshapes+i)->yval == (allshapes+current)->yval+(allshapes+current)->ydiff))
				{ return 1; }
				break;
			case 1:
				if(((allshapes+i)->xval < (allshapes+current)->xval + (allshapes+current)->xdiff) &&
				((allshapes+current)->xval + (allshapes+current)->xdiff < (allshapes+i)->xval + (allshapes+i)->xdiff) &&	
				(allshapes+i)->yval == (allshapes+current)->yval)
				{ return 1; }
				break;
			case 2:
				if(((allshapes+i)->xval < (allshapes+current)->xval - (allshapes+current)->xdiff && 
				(allshapes+current)->xval - (allshapes+current)->xdiff < (allshapes+i)->xval + (allshapes+i)->xdiff) && 
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
			if((allshapes+current)->xval<MAXX && (allshapes+current)->movdown && checkcollision(1,allshapes)==0){
				(allshapes+current)->xval += GENDIFF;
			}
		}else if(event->key.key == SDLK_LEFT){
			if((allshapes+current)->xval>0 && (allshapes+current)->movdown && checkcollision(2,allshapes)==0){
				(allshapes+current)->xval -= GENDIFF;
			}
		}else if(event->key.key == SDLK_DOWN){
			while((allshapes+current)->yval + (allshapes+current)->ydiff < MAXY && checkcollision(0,allshapes)==0){
				(allshapes+current)->yval += GENDIFF;
			}
			(allshapes+current)->movdown = false;
			current++;
			
		}
	}
	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){
	SDL_RenderClear(renderer);

	struct cshape* allshapes = (struct cshape*)appstate;

	for(int i = 0; i < SHAPECOUNT; i++){
		SDL_Vertex rectvert[7] = {
		{{(allshapes+i)->xval,(allshapes+i)->yval}, {255,0,0,0}, {0, 0} },
		{{(allshapes+i)->xval + (allshapes+i)->xdiff, (allshapes+i)->yval}, {255,0,0,0}, {1, 0} },
		{{(allshapes+i)->xval + (allshapes+i)->xdiff, (allshapes+i)->yval + (allshapes+i)->ydiff}, {255,0,0,0}, {1, 1} },
		{{(allshapes+i)->xval, (allshapes+i)-> yval + (allshapes+i)->ydiff}, {255,0,0,0}, {0, 1}},
		{{(allshapes+i)->xval, (allshapes+i)->yval + (allshapes+i)->ydiff}, {255,0,0,0}, {1, 1}},
		{{(allshapes+i)->xval + (allshapes+i)->xdiff, (allshapes+i)->yval + (allshapes+i)->ydiff}, {255,0,0,0}, {1, 0}},
		{{(allshapes+i)->xval + (allshapes+i)->xdiff, (allshapes+i)->yval}, {255,0,0,0}, {0, 1}}
		};
		SDL_RenderGeometry(renderer,NULL,rectvert,7,(allshapes+i)->indices,6);
	}

	SDL_RenderPresent(renderer);
	if((allshapes+current)->movdown && frames++>MAXFRAMES){
		(allshapes+current)->yval += GENDIFF;
		frames=0;
		if((allshapes+current)->yval + (allshapes+current)->ydiff >= MAXY || checkcollision(0,allshapes)==1){
			(allshapes+current)->movdown=false;
			current++;
		}
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
