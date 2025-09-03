#define SDL_MAIN_USE_CALLBACKS 1 
#include<SDL3/SDL.h>
#include<SDL3/SDL_main.h>
#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>

#define SHAPECOUNT 100
#define MAXX 600
#define MAXY 700
#define MAXTICKS 500
#define GENDIFF 50

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static unsigned int prevt = 0;
static int current = 0;

static int squarep[] = {0,1,2,0,3,2};
static int tp[] = {0,1,2,0,3,2,4,5,6,4,7,6};

struct cshape{
	int xval;
	int yval;
	int xdiff;
	int ydiff;
	bool movdown;
	int* indices;
	int moreshapes;
};

void setpoints(struct cshape* allshapes, int i){
	int random = rand()%4;
	(allshapes+i)->moreshapes = 0;
	switch(random){
		case 0:
			(allshapes+i)->xdiff = 3*GENDIFF;
			(allshapes+i)->ydiff = GENDIFF;
			(allshapes+i)->indices = squarep;
			break;
		case 1:
			(allshapes+i)->xdiff = GENDIFF;
			(allshapes+i)->ydiff = 3*GENDIFF;
			(allshapes+i)->indices = squarep;
			break;
		case 2:
			(allshapes+i)->xdiff = 2*GENDIFF;
			(allshapes+i)->ydiff = 2*GENDIFF;
			(allshapes+i)->indices = squarep;
			break;
		case 3:
			(allshapes+i)->xdiff = 3*GENDIFF;
			(allshapes+i)->ydiff = GENDIFF;
			(allshapes+i)->indices = tp;
			(allshapes+i)->moreshapes = 1;
			break;
	}

}

void initializearr(struct cshape* allshapes){
	for(int i=0;i<SHAPECOUNT;i++){	
		setpoints(allshapes,i);
		(allshapes+i)->xval = MAXX/2;
		(allshapes+i)->yval = -((allshapes+i)->ydiff)*2; 
		(allshapes+i)->movdown = true;
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
			int totaly = ((allshapes+current)->yval + ((allshapes+current)->moreshapes == 0 ? 0 : GENDIFF));
			while(totaly + (allshapes+current)->ydiff < MAXY && checkcollision(0,allshapes)==0){
				totaly += GENDIFF;
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
		SDL_Vertex rectvert[8] = {
		{{(allshapes+i)->xval,(allshapes+i)->yval}, {255,0,0,0}, {0, 0} },
		{{(allshapes+i)->xval + (allshapes+i)->xdiff, (allshapes+i)->yval}, {255,0,0,0}, {1, 0} },
		{{(allshapes+i)->xval + (allshapes+i)->xdiff, (allshapes+i)->yval + (allshapes+i)->ydiff}, {255,0,0,0}, {1, 1} },
		{{(allshapes+i)->xval, (allshapes+i)-> yval + (allshapes+i)->ydiff}, {255,0,0,0}, {0, 1}},
		
		{{(allshapes+i)->xval + GENDIFF, (allshapes+i)->yval+GENDIFF}, {255,0,0,0}, {0, 0} },
		{{(allshapes+i)->xval + GENDIFF*2, (allshapes+i)->yval+GENDIFF}, {255,0,0,0}, {0, 1} },
		{{(allshapes+i)->xval + GENDIFF*2, (allshapes+i)->yval + GENDIFF*2}, {255,0,0,0}, {1, 1} },
		{{(allshapes+i)->xval + GENDIFF, (allshapes+i)-> yval + GENDIFF*2}, {255,0,0,0}, {0, 1}},
		};
		SDL_RenderGeometry(renderer,NULL,rectvert,8,(allshapes+i)->indices,12);
	}

	SDL_RenderPresent(renderer);
	unsigned int currt = SDL_GetTicks();
	if((allshapes+current)->movdown && (currt > prevt + MAXTICKS)){
		(allshapes+current)->yval += GENDIFF;
		int totaly = (allshapes+current)->yval + ((allshapes+current)->moreshapes == 0 ? 0 : GENDIFF);
		if(totaly + (allshapes+current)->ydiff >= MAXY || checkcollision(0,allshapes)==1){
			(allshapes+current)->movdown=false;
			current++;
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
