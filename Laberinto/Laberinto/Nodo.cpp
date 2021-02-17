#include "Nodo.h"

Nodo::Nodo(wchar_t caracter, int x, int y)
{
	if (caracter == ' ')
		muro = true;
	else
		muro = false;

	this->x = x;
	this->y = y;
 	g = 0;
	h = 0;
	f = g + h;
	padre = nullptr;
}
