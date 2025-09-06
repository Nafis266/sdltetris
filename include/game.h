#ifndef GAME_H
#define GAME_H

#include<stdbool.h>

extern int current;

struct cshape{
	int xval;
	int yval;
	bool movdown;
	int *indices;
	int indcount;
};


int checkcollision(int thecase, struct cshape* allshapes);


#endif
