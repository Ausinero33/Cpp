#include<windows.h>
#include "Generador.h"
#include "PathFind.h"
#include <iostream>
#include<conio.h>

int main() {
	Generador gen;
	gen.generarInicial();
	string algo;

	cout << "El tamaño de la consola para este laberinto deberia ser " << gen.celdasW * 2 + 1 << "x" << gen.celdasH * 2 + 1 << "\nPulsa cualquier tecla para continuar" << endl;
	_getch();

	wchar_t* screen = gen.screen;
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD escrito = 0;

	//for (int i = 0; i < gen.nScreenHeight; i++) {
	//	for (int j = 0; j < gen.nScreenWidth; j++) {
	//		screen[i * gen.nScreenWidth + j] = ' ';
	//	}
	//}

	gen.backtracking(0, 0);
	gen.imprimirLaberinto();

	screen = gen.screen;

	gen.file << "Eres el puto amo Gonzalo" << endl << "------+" << endl; //Esto se va a quedar porque es verdad

	
	

	PathFind *ptf = new PathFind();
	ptf->inicializarNodos(screen, gen);

	vector<Nodo> camino = ptf->aStar(*ptf->nodos[gen.nScreenWidth * 1 + 1], *ptf->nodos[gen.nScreenWidth * (gen.nScreenHeight - 2) + (gen.nScreenWidth - 2)]);

	for (int i = 0; i < ptf->camino.size(); i++) {
		screen[camino.at(i).x + camino.at(i).y * gen.nScreenWidth] = 0x2592;
		gen.file << "A";
	}

	screen[gen.nScreenWidth * 1 + 1] = 'I';
	screen[gen.nScreenWidth * (gen.nScreenHeight - 2) + (gen.nScreenWidth - 2)] = 'D';

	gen.file.close();
	ptf->pffile.close();

	while (1) {
		WriteConsoleOutputCharacter(hConsole, screen, gen.nScreenWidth * gen.nScreenHeight, { 0, 0 }, &escrito);
	}


	return 0;
}
