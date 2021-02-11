#include<windows.h>
#include<math.h>
#include<vector>
#include <string>
#include <iostream>

using namespace std;

const int nScreenWidth = 120;
const int nScreenHeight = 40;

struct Esfera {
	double radio;// = 20.25f;
	double centroX;// = nScreenWidth / 2;
	double centroY;// = nScreenHeight / 2;
	double centroZ;// = 0;
};

double calcularDistancia(double eX, double eY, double eZ, int pX, int pY, int pZ) {
	return sqrt(pow(pX - eX, 2) + pow((pY - eY) * 2, 2) + pow(pZ - eZ, 2)); //Multiplicado por 2 la Y porque un caracter es el 2 de alto que de ancho
}

vector<double> calcularNormal(double eX, double eY, double eZ, int pX, int pY, int pZ) { //La normal es la recta desde el centro hasta el punto de la superficie de la esfera
	vector<double> vect;
	//Normal
	vect.push_back(pX - eX);
	vect.push_back(pY - eY);
	vect.push_back(pZ - eZ);

	//Normal normalizada
	double magnitud = sqrt(pow(vect[0], 2) + pow(vect[1], 2) + pow(vect[2], 2));

	vect[0] /= magnitud;
	vect[1] /= magnitud;
	vect[2] /= magnitud;

	return vect;
}

int calcularLuminiscencia(vector<double> normal, vector<double> lum) {
	int num = (normal[0] * lum[0] + normal[1] * lum[1] + normal[2] * lum[2]) * 8;
	return num;
}

char obtenerCaracter(int luminiscencia) { //Luminiscencia = [-11, 11]
	char c;
	switch (luminiscencia)
	{
	case 0:
		c = '.';
		break;
	case 1:
		c = ',';
		break;
	case 2:
		c = '-';
		break;
	case 3:
		c = '~';
		break;
	case 4:
		c = ':';
		break;
	case 5:
		c = ';';
		break;
	case 6:
		c = '=';
		break;
	case 7:
		c = '!';
		break;
	case 8:
		c = '*';
		break;
	case 9:
		c = '#';
		break;
	case 10:
		c = '$';
		break;
	case 11:
		c = '@';
		break;
	default:
		c = 'o';
		break;
	}
	return c;
}

vector<double> rotarVector(vector<double> vect, double angulo, char eje) {
	vector<double> rot;
	switch (eje) {
	case 'X':
		rot.push_back(vect[0]);
		rot.push_back(vect[1] * cos(angulo) - vect[2] * sin(angulo));
		rot.push_back(vect[1] * sin(angulo) + vect[2] * cos(angulo));
		break;
	case 'Y':
		rot.push_back(vect[0] * cos(angulo) + vect[2] * sin(angulo));
		rot.push_back(vect[1]);
		rot.push_back(-vect[0] * sin(angulo) + vect[2] * cos(angulo));
		break;
	case 'Z':
		rot.push_back(vect[0] * cos(angulo) - vect[1] * sin(angulo));
		rot.push_back(vect[0] * sin(angulo) + vect[1] * cos(angulo));
		rot.push_back(vect[2]);
		break;
	}
	return rot;
}

void dibujarEsfera(Esfera esfera, vector<double> rotado, wchar_t *screen) {
	for (int z = esfera.centroZ - esfera.radio; z < esfera.centroZ + esfera.radio; z++) {
		for (int x = 0; x < nScreenWidth; x++) {
			for (int y = 0; y < nScreenHeight; y++) {
				//Si es un punto de la esfera o menor, calcular la normal y dibujar segun la normal
				if (calcularDistancia(esfera.centroX, esfera.centroY, esfera.centroZ, x, y, z) <= esfera.radio) {
					//Calculamos la normal de ese punto.
					vector<double> normal = calcularNormal(esfera.centroX, esfera.centroY, esfera.centroZ, x, y, z);
					//Dibujamos 1 caracter segun su normal.
					//screen[y * nScreenWidth + x] = obtenerCaracter(calcularLuminiscencia(normal, rotado));
					if (calcularLuminiscencia(normal, rotado) < -5) {
						screen[y * nScreenWidth + x] = 0x2588;
					}
					else if (calcularLuminiscencia(normal, rotado) < 0) {
						screen[y * nScreenWidth + x] = 0x2593;
					}
					else if (calcularLuminiscencia(normal, rotado) < 5) {
						screen[y * nScreenWidth + x] = 0x2592;
					}
					else {
						screen[y * nScreenWidth + x] = 0x2591;
					}
				}
			}
		}
	}
}

int main() {
	wchar_t *screen = new wchar_t[nScreenWidth * nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD escrito = 0;
	
	char c;
	int cont = 0;
	Esfera esfera, esfera2;
	double luz = -1;
	int contr = 0;
	vector<double> lum;
	lum.push_back(0);
	lum.push_back(1);
	lum.push_back(-1);
	float rotacion = 0.05f;
	vector<double> rotado = lum;

	esfera.radio = 20.25F;
	esfera.centroX = nScreenWidth / 2;
	esfera.centroY = nScreenHeight / 2;
	esfera.centroZ = 0;

	esfera2.radio = 5.25F;
	esfera2.centroX = nScreenWidth / 4;
	esfera2.centroY = nScreenHeight / 4;
	esfera2.centroZ = -3;

	for (int x = 0; x < nScreenWidth; x++) {
		for (int y = 0; y < nScreenHeight; y++) {
			screen[y * nScreenWidth + x] = ' ';
		}
	}

	while (1) {
		
		dibujarEsfera(esfera, rotado, screen);
		dibujarEsfera(esfera2, rotado, screen);

		rotado = rotarVector(lum, rotacion, 'Y');
		rotacion += 0.05f;

		string num1 = to_string(rotado[0]);
		string num2 = to_string(rotado[1]);
		string num3 = to_string(rotado[2]);

		string num4 = to_string(sqrt(pow(rotado[0], 2) + pow(rotado[1], 2) + pow(rotado[2], 2)));
		//string num5 = to_string(cos(3));

		for (int i = 1; i < 5; i++) {
			screen[nScreenWidth * nScreenHeight - i] = num3[4 - i];
			screen[nScreenWidth * nScreenHeight - i - 5] = num2[4 - i];
			screen[nScreenWidth * nScreenHeight - i - 10] = num1[4 - i];
			screen[nScreenWidth * nScreenHeight - i - 15] = num4[4 - i];
		}
		
		//for (int i = 0; i < sizeof(num5); i++) {
		//	screen[nScreenWidth * nScreenHeight - 25] = num5[sizeof(num5) - 1];
		//}

		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0, 0 }, &escrito);
	}

	
	return 0;
}