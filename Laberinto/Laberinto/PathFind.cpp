#include "PathFind.h"

PathFind::PathFind() {

}

vector<Nodo> PathFind::aStar(Nodo &src, Nodo& dest)
{
	pffile.open("pfout.txt");
	nodosAbiertos.push_back(&src);

	pffile << "Coordenadas dest: " << dest.x << " " << dest.y << endl;
	
	while (nodosAbiertos.size() != 0) {
		Nodo* act = nodoMenorF();

		pffile << "Coordenadas act: " << act->x << " " << act->y << endl;
		pffile << "Nodos abiertos: " << nodosAbiertos.size() << endl;
		pffile << "Nodos cerrados: " << nodosCerrados.size() << endl;

		int indAc = find(nodosAbiertos.begin(), nodosAbiertos.end(), act) - nodosAbiertos.begin();
		nodosAbiertos.erase(nodosAbiertos.begin() + indAc);
		nodosCerrados.push_back(act);
		pffile << "Ind: " << indAc << endl;
		pffile << "Nodos abiertos: " << nodosAbiertos.size() << endl;
		pffile << "Nodos cerrados: " << nodosCerrados.size() << endl;

		if (act->x == dest.x && act->y == dest.y) {
			return recrearCamino(src, dest);
		}
			

		vector<Nodo *> vecinos = calcularVecinos(*act);

		pffile << "N vecinos: " << vecinos.size() << endl << "-------" << endl;

		//Para cada vecino de act
		for (Nodo *vecino : vecinos) {
			//Si el vecino no es accesible o esta cerrado, pasamos al siguiente
			if (vecino->muro || find(nodosCerrados.begin(), nodosCerrados.end(), vecino) != nodosCerrados.end()) {
				//pffile << "Muro" << vecino->x << vecino->y << endl;
				continue;
			}
			int costeVecino = act->g + getDist(*act, *vecino);
			//Si un nuevo camino a un vecino es menor o el vecino no esta abierto
			if (costeVecino < vecino->g || find(nodosAbiertos.begin(), nodosAbiertos.end(), vecino) == nodosAbiertos.end()) {
				//calculamos f del vecino
				calcularF(*vecino, src, dest);
				//Padre del vecino = act
				vecino->padre = act;	//MUY POSIBLEMENTE ESTO ME ESTE FALLANDO
				//Si el vecino no esta abierto
				if (find(nodosAbiertos.begin(), nodosAbiertos.end(), vecino) == nodosAbiertos.end()) {
					//Añadimos vecino a abiertos
					nodosAbiertos.push_back(vecino);
				}
			}
		}
		pffile << "*******" << endl;
	}



	return vector<Nodo>();
}

int PathFind::getDist(Nodo &A, Nodo &B) {
	int distX = abs(A.x - B.x);
	int distY = abs(A.y - B.y);

	if (distX > distY)
		return 14 * distY + 10 * (distX - distY);
	return 14 * distX + 10 * (distY - distX);
}

void PathFind::inicializarNodos(wchar_t* screen, Generador &gen)
{
	for (int i = 0; i < gen.nScreenHeight; i++) {
		for (int j = 0; j < gen.nScreenWidth; j++) {
			nodos.push_back(new Nodo(screen[j + i * gen.nScreenWidth], j, i));
		}
	}
}

Nodo* PathFind::nodoMenorF()
{
	int cuenta = 0;
	for (int i = 0; i < nodosAbiertos.size(); i++) {
		for (int j = 0; j < nodosAbiertos.size(); j++) {
			if (nodosAbiertos.at(i) <= nodosAbiertos.at(j)) {
				cuenta++;
			}
			if (cuenta == nodosAbiertos.size()) {
				return nodosAbiertos.at(i);
			}
		}
	}
}

void PathFind::calcularF(Nodo& vecino, Nodo& org, Nodo& dest)
{
	vecino.g = 10 * sqrt(pow(vecino.x - org.x, 2) + pow(vecino.y - org.y, 2));
	vecino.h = 10 * sqrt(pow(vecino.x - dest.x, 2) + pow(vecino.y - dest.y, 2));
	vecino.f = vecino.g + vecino.h;
}

vector<Nodo> PathFind::recrearCamino(Nodo& org, Nodo& dest)
{
	Nodo *act = &dest;

	while (dest.x != org.x && dest.y != org.y && act->padre != nullptr) {
		camino.push_back(*act);
		act = act->padre;
	}

	reverse(camino.begin(), camino.end());
	return camino;
}

int PathFind::distNodos(Nodo& n1, Nodo& n2) {
	return 10 * sqrt(pow(n2.x - n1.x, 2) + pow(n2.y - n1.y, 2));
}

vector<Nodo *> PathFind::calcularVecinos(Nodo &act)
{
	vector<Nodo *> vecinos;
	for (Nodo* nodo : nodos) {
		if (distNodos(act, *nodo) == 10 || distNodos(act, *nodo) == 14) {
			vecinos.push_back(nodo);
		}
		if (vecinos.size() == 8) {
			return vecinos;
		}
	}
	return vecinos;
}

void PathFind::impNodos(vector<Nodo*>& nodos)
{
	pffile << nodos.size() << "s";
	for (int i = 0; i < nodos.size(); i++) {
		if (nodos.at(i)->padre != nullptr)
			pffile << nodos.at(i)->x << nodos.at(i)->y << " " << nodos.at(i)->padre->x << "," << nodos.at(i)->padre->y << endl << endl;
		else
			pffile << nodos.at(i)->x << nodos.at(i)->y << " " << "Null" << endl;
	}
}
