#pragma once
#include<cmath>

class Celda
{
public:
	int cX;
	int cY;
	bool visto;
	int muros[4]; //N S E O
	int vecinos[4];

	Celda(int coordX, int coordY, bool visto);
	int obtenerIndiceMuro();
};

