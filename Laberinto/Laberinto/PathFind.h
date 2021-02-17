#pragma once

#include<vector>
#include "Nodo.h"
#include "Generador.h"

using namespace std;

class PathFind
{
public:
	ofstream pffile;
	vector<Nodo*> nodos;
	vector<Nodo*> nodosAbiertos;
	vector<Nodo*> nodosCerrados;
	vector<Nodo> camino;
	PathFind();
	vector<Nodo> aStar(Nodo &src, Nodo &dest);
	void inicializarNodos(wchar_t *screen, Generador &gen);
	Nodo* nodoMenorF();
	void calcularF(Nodo& vecino, Nodo &org, Nodo &dest);
	int getDist(Nodo& A, Nodo& B);
	int distNodos(Nodo& n1, Nodo& n2);
	vector<Nodo> recrearCamino(Nodo& org, Nodo& dest);
	vector<Nodo *> calcularVecinos(Nodo& act);

	void impNodos(vector<Nodo*> &nodos);
};

