#include "Celda.h"

Celda::Celda(int coordX, int coordY, bool visto) {
	cX = coordX;
	cY = coordY;
	this->visto = visto;
	for (int i = 0; i < 4; i++) {
		muros[i] = 1;
		vecinos[i] = 0;
	}
}

int Celda::obtenerIndiceMuro()
{
	int num = muros[0] * pow(2, 3) + muros[1] * pow(2, 2) + muros[2] * pow(2, 1) + muros[3] * pow(2, 0);
	return num;
}
