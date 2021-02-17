#include "Generador.h"

void Generador::generarInicial() {
	file.open("log.txt");
	for (int i = 0; i < celdasH; i++) {
		for (int j = 0; j < celdasW; j++) {
			celdas.push_back(new Celda(j, i, false));
			//screen[i * nScreenWidth + j] = L"0123456789ABCDEF"[celdas.back()->obtenerIndiceMuro()];
		}
	}
}

int Generador::contarVecinos(Celda &celda, int *pos) {
	int vecinos = 0;
	for (int i = -1; i < 2; i++) {
		if (i != 0 && celda.cY + i >= 0 && celda.cY + i < celdasH && !celdas.at(celda.cY * celdasW + celda.cX + celdasW * i)->visto) {
			vecinos++;
			if (i == -1)
				pos[0] = 1;
			else
				pos[1] = 1;
		}
		if (i != 0 && celda.cX + i >= 0 && celda.cX + i < celdasW && !celdas.at(celda.cY * celdasW + (celda.cX + i))->visto) {
			vecinos++;
			if (i == -1)
				pos[3] = 1;
			else
				pos[2] = 1;
		}
	}

	return vecinos;
}

Celda* Generador::escogerVecino(Celda celda, int *pos, int &sel)
{
	int numRand;
	do {
		numRand = rand() % 4;
	} while (pos[numRand] == 0);

	sel = numRand;

	switch (numRand)
	{
	case 0: //N
		return celdas.at((celda.cX + celda.cY * celdasW) - celdasW);
		break;
	case 1: //S
		return celdas.at((celda.cX + celda.cY * celdasW) + celdasW);
		break;
	case 2: //E
		return celdas.at((celda.cX + celda.cY * celdasW) + 1);
		break;
	case 3: //O
		return celdas.at((celda.cX + celda.cY * celdasW) - 1);
		break;
	}
}

void Generador::quitarMuro(Celda *celda, Celda *vecino, int sel)
{
	switch (sel)
	{
	case 0: //N
		celda->muros[0] = 0;
		vecino->muros[1] = 0;
		break;
	case 1: //S
		celda->muros[1] = 0;
		vecino->muros[0] = 0;
		break;
	case 2: //E
		celda->muros[2] = 0;
		vecino->muros[3] = 0;
		break;
	case 3: //O
		celda->muros[3] = 0;
		vecino->muros[2] = 0;
		break;
	}
}

void Generador::imprimirLaberinto()
{
	for (int i = 0; i < nScreenHeight; i++) {
		for (int j = 0; j < nScreenWidth; j++) {
			screen[i * nScreenWidth + j] = ' ';
			if (i % 2 != 0 && j % 2 != 0) {
				screen[i * nScreenWidth + j] = 0x2588;
				for (int k = 0; k < 4; k++) {
					if (celdas.at((i / 2) * celdasW + (j / 2))->muros[k] == 0) {
						switch (k)
						{
						case 0:
							screen[(i - 1) * nScreenWidth + j] = 0x2588;
							break;
						case 1:
							screen[(i + 1) * nScreenWidth + j] = 0x2588;
							break;
						case 2:
							screen[i * nScreenWidth + (j + 1)] = 0x2588;
							break;
						case 3:
							screen[i * nScreenWidth + (j - 1)] = 0x2588;
							break;
						}
					}
				}
			}
		}
	}
}

void Generador::backtracking(int x, int y)
{
	srand((unsigned int)time(NULL));
	int sel; //Guarda la posicion del vector de la posicion de los vecinos. Guarda el vecino.
	celdas.at(y * celdasW + x)->visto = true; //Seleccionar celda como vista
	stack.push_back(celdas.at(y * celdasW + x)); //Llevar celda al stack

	while (stack.size() != 0) {
		//Sacar celda del stack y hacerla la actual
		Celda* act = stack.back();
		stack.pop_back();

		//Contar vecinos sin visitar
		int vecinos = contarVecinos(*act, act->vecinos);
		if (vecinos > 0) {
			stack.push_back(act);
			Celda* vecino = escogerVecino(*act, act->vecinos, sel);
			quitarMuro(act, vecino, sel);
			vecino->visto = true;
			stack.push_back(vecino);

			file << act->vecinos[0] << act->vecinos[1] << act->vecinos[2] << act->vecinos[3] << " " << vecinos << " " << act->cX << " " << act->cY << endl;
			file << vecino->cX << " " << vecino->cY << " " << sel << endl;
			file << act->obtenerIndiceMuro() << " " << vecino->obtenerIndiceMuro() << " " << act->muros[0] << act->muros[1] << act->muros[2] << act->muros[3] << " " << vecino->muros[0] << vecino->muros[1] << vecino->muros[2] << vecino->muros[3] << endl;
		}
	}
}
