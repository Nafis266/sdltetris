#include "game.h"
#include "config.h"

int checkcollision(int thecase, struct cshape* allshapes){
	if(thecase == -1){
		if((allshapes+current)->yval + (((allshapes+current)->indices[((allshapes+current)->indcount)-1])/MAXCOLS)*(GENDIFF) + GENDIFF >= MAXY){
			return 1;
		}
		return 0;
	}
	if(current == 0){ return 0; }

	for(int i=0; i<(allshapes+current)->indcount; i++){
		for(int j=0; j<current; j++){
			for(int k=0; k<(allshapes+j)->indcount; k++){
				int yidiff = (allshapes+current)->indices[i] / MAXCOLS;
				int yjdiff = (allshapes+j)->indices[k] / MAXCOLS;

				int xidiff = (allshapes+current)->indices[i] % MAXCOLS;
				int xjdiff = (allshapes+j)->indices[k] % MAXCOLS;
				switch(thecase){
					case 0:
						if(((allshapes+current)->yval + yidiff*GENDIFF + GENDIFF == (allshapes+j)->yval + yjdiff*GENDIFF) &&
						((allshapes+current)->xval + xidiff*GENDIFF == (allshapes+j)->xval + xjdiff*GENDIFF)){
							return 1;
						}
						break;
					case 1:
						if(((allshapes+current)->xval + xidiff*GENDIFF + GENDIFF == (allshapes+j)->xval + xjdiff*GENDIFF) &&
						((allshapes+current)->yval + yidiff*GENDIFF == (allshapes+j)->yval + yjdiff*GENDIFF)){
							return 1;
						}
						break;
					case 2:
						if((allshapes+current)->xval + xidiff*GENDIFF - GENDIFF == ((allshapes+j)->xval + xjdiff*GENDIFF) &&
						((allshapes+current)->yval + yidiff*GENDIFF == (allshapes+j)->yval + yjdiff*GENDIFF)){
							return 1;
						}
						break;
				}
			}
		}
	}
	return 0;
}
