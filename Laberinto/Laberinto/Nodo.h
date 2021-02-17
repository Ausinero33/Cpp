#pragma once
class Nodo
{
public:
	int x;
	int y;
	bool muro;
	int g;
	int h;
	int f;
	Nodo* padre;

	Nodo(wchar_t caracter, int x, int y);
};

