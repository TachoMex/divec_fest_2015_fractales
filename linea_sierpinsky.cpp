#include "2D/linea.h"
#include "imagen/imagen.h"
#include <queue>
using namespace std;
using namespace _2D;

const int TAM_X = 800;
const int TAM_Y = 800;
const int MARGEN_X = 10;
const int MARGEN_Y = 10;
const int ITERACIONES = 10;

int main(){
	Imagen mat(TAM_X,TAM_Y,Color::negro);
	queue<Linea<double>> fractal;
	fractal.push(Linea<double>(Punto<double>(MARGEN_X,MARGEN_Y	), Punto<double>(TAM_X - MARGEN_X, MARGEN_Y)));

	for(int k=0;k<ITERACIONES;k++){
		int n = fractal.size();
		for(int i=0;i<n;i++){
			Linea<double> l = fractal.front();
			fractal.pop();

			Linea<double> l1 = (l.rotar(M_PI/3.0)*0.5).invierte();
			Linea<double> l2 = l1.rotar(M_PI*2.0/3.0);
			Linea<double> l3 = l2.invierte().rotar(M_PI*2.0/3.0).invierte();

			fractal.push(l1);
			fractal.push(l2);
			fractal.push(l3);
		}
	}

	while(not fractal.empty()){
		Linea<double> l = fractal.front();
		fractal.pop();
		dibujaLinea(mat,(int)l.inicio.x, (int)l.inicio.y, (int)l.fin.x,(int)l.fin.y,Color::blanco);
		/*
		for(int i=-3;i<=3;i++)
			for(int j=-3;j<=3;j++)
				mat.en((int)l.inicio.y+i, (int)l.inicio.x+j) = Color::verde;
		*/
	}

	mat.guardaBMP("sierpinsky.bmp");

	return 0;
}