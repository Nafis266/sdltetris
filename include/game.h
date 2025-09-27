#ifndef GAME_H
#define GAME_H

#include<stdbool.h>
#include<time.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

extern int current;

struct cshape{
	int xval;
	int yval;
	bool movdown;
	int *indices;
	int indcount;
};


int checkcollision(int thecase, struct cshape* allshapes);
void setshape(struct cshape* allshapes, int i);
void initializearr(struct cshape* allshapes);
void checkrows(struct cshape* allshapes);

#endif
