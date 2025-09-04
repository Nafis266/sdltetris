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
#define MAXCOLS 4
#define MAXROWS 4

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static unsigned int prevt = 0;
static int current = 0;

static int squarep[] = {0,1,2,0,3,2};
static int Iindex[] = {0,1,2,3};
static int Tindex[] = {0,1,2,5};
static int Lindex[] = {0,1,2,6,10,14};
static int Sindex[] = {1,5,6,10};

struct cshape{
	int xval;
	int yval;
	bool movdown;
	int* indices;
	int indcount;
};

void setshape(struct cshape* allshapes, int i){
	int random = rand()%4;
	switch(random){
		case 0:
			(allshapes+i)->indices = Sindex;
			(allshapes+i)->indcount = sizeof(Sindex)/sizeof(Sindex[0]);
			break;
		case 1:
			(allshapes+i)->indices = Tindex;
			(allshapes+i)->indcount = sizeof(Tindex)/sizeof(Tindex[0]);
			break;
		case 2:
			(allshapes+i)->indices = Lindex;
			(allshapes+i)->indcount = sizeof(Lindex)/sizeof(Lindex[0]);
			break;
		case 3:
			(allshapes+i)->indices = Iindex;
			(allshapes+i)->indcount = sizeof(Iindex)/sizeof(Iindex[0]);
			break;
	}
}

void initializearr(struct cshape* allshapes){
	for(int i=0;i<SHAPECOUNT;i++){	
		(allshapes+i)->xval = MAXX/2;
		(allshapes+i)->yval = -100; 
		(allshapes+i)->movdown = true;
		setshape(allshapes,i);
	}
}


int checkcollision(int thecase, struct cshape* allshapes){
	if(current==0){ return 0; }
	return 0;
	/*
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
	*/
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
	}
	/*
	if(event->type == SDL_EVENT_KEY_DOWN){
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
	*/
	return SDL_APP_CONTINUE;
}


SDL_AppResult SDL_AppIterate(void *appstate){
	SDL_RenderClear(renderer);

	struct cshape* allshapes = (struct cshape*)appstate;

	for(int i = 0; i < SHAPECOUNT; i++){
		int index = 0;
		for(int j=0; j<MAXCOLS*MAXROWS && index < (allshapes+i)->indcount; j++){	
			if((allshapes+i)->indices[index] != j){
				continue;
			}
			int xdiff = j % MAXCOLS;
			int ydiff = j / MAXCOLS;
			SDL_Vertex rectvert[4] = {
			{{(allshapes+i)->xval + xdiff*GENDIFF, (allshapes+i)->yval + ydiff*GENDIFF}, {255,0,0,255}, {0, 0} },
			{{(allshapes+i)->xval + (xdiff+1)*GENDIFF, (allshapes+i)->yval + ydiff*GENDIFF}, {255,0,0,255}, {1, 0} },
			{{(allshapes+i)->xval + (xdiff+1)*GENDIFF, (allshapes+i)->yval + (ydiff+1)*GENDIFF}, {255,0,0,255}, {1, 1} },
			{{(allshapes+i)->xval + xdiff*GENDIFF, (allshapes+i)-> yval + (ydiff+1)*GENDIFF}, {255,0,0,255}, {0, 1}},	
			};
			SDL_RenderGeometry(renderer,NULL,rectvert,4,squarep,6);
			index++;
		}
	}

	SDL_RenderPresent(renderer);
	unsigned int currt = SDL_GetTicks();
	if((allshapes+current)->movdown && (currt > prevt + MAXTICKS)){
		(allshapes+current)->yval += GENDIFF;
		if((allshapes+current)->yval >= MAXY || checkcollision(0,allshapes)==1){
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
