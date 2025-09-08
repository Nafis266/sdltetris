#include "game.h"
#include "config.h"

static int Iindex[] = {0,4,8,12};
static int Tindex[] = {1,4,5,6};
static int Lindex[] = {0,4,8,9,10};
static int Sindex[] = {0,4,5,9};
static int Xindex[] = {1,4,5,6,9};


void setarray(int *setarr, int* arr, int length){
	for(int i=0; i<length; i++){
		*(setarr+i) = *(arr+i);
	}
}

void setshape(struct cshape* allshapes, int i){
	int random = rand()%1;
	int *randind;
	int length;
	switch(random){
		case 0:
			randind = Iindex;
			length = sizeof(Iindex)/sizeof(Iindex[0]);
			break;
		case 1:
			randind = Tindex;
			length = sizeof(Tindex)/sizeof(Tindex[0]);
			break;
		case 2:
			randind = Lindex;
			length = sizeof(Lindex)/sizeof(Lindex[0]);
			break;
		case 3:
			randind = Sindex;
			length = sizeof(Sindex)/sizeof(Sindex[0]);
			break;
		case 4:
			randind = Xindex;
			length = sizeof(Xindex)/sizeof(Xindex[0]);
			break;
	}
	(allshapes+i)->indcount = length;
	(allshapes+i)->indices = malloc(length * sizeof(int));
	setarray((allshapes+i)->indices, randind, length);
}

void initializearr(struct cshape* allshapes){
	for(int i=0;i<SHAPECOUNT;i++){	
		(allshapes+i)->xval = (MAXX/2)-2*GENDIFF;
		(allshapes+i)->yval = -MAXROWS*GENDIFF; 
		(allshapes+i)->movdown = true;
		setshape(allshapes,i);
	}
}
