#include "game.h"
#include "config.h"

void deleteindex(int* arr, int* len, int index){
	for(int i=index; i < *(len)-1; i++){
		*(arr+i) = *(arr+i+1);
	}
	*(len) -= 1;
}

void deleterow(struct cshape* allshapes, int row, int count, int toDel[][2]){
	for(int i=count-1; i>=0; i--){
		deleteindex((allshapes + toDel[i][0])->indices,&((allshapes + toDel[i][0])->indcount), toDel[i][1]);
	}
}

void checkrows(struct cshape* allshapes){
	for(int row=0; row< MAXY/GENDIFF; row++){
		int count = 0;
		int toDelete[MAXY/GENDIFF][2];
		for(int j=0; j<current; j++){
			for(int k=0; k<(allshapes+j)->indcount; k++){
				int ydiff = (allshapes+j)->indices[k] / MAXCOLS;
				int xdiff = (allshapes+j)->indices[k] % MAXCOLS;
				if((allshapes+j)->yval + ydiff*GENDIFF == row*GENDIFF){
					toDelete[count][0] = j;
					toDelete[count++][1] = k;
				}
			}
		}
		if(count == MAXX/GENDIFF){
			deleterow(allshapes,row,count,toDelete);
		}
	}
}
