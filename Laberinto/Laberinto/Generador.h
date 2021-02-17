#pragma once
#include <vector>
#include <sstream>
#include "Celda.h"
#include <cstdlib>
#include <Windows.h>
#include<fstream>
#include<iostream>
#include <time.h>

using namespace std;

class Generador
{
public:
	ofstream file;
	int celdasW = 100;
	int celdasH = 20;
	int nScreenWidth = celdasW * 2 + 1;
	int nScreenHeight = celdasH * 2 + 1;
	int inicialX = 1;
	int inicialY = 1;
	vector<Celda*> celdas;
	vector<Celda*> stack;
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	void generarInicial();
	void backtracking(int x, int y);
	int contarVecinos(Celda &celda, int *pos);
	Celda* escogerVecino(Celda celda, int *pos, int &sel);
	void quitarMuro(Celda *celda, Celda *vecino, int sel);
	void imprimirLaberinto();
};

