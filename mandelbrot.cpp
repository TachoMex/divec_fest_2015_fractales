#include "imagen/imagen.h"
#include <complex>
using namespace std;

const int TAM_X = 800;
const int TAM_Y = 800;
const int iteraciones = 100;

const double Xi = -2.0;
const double Yi = -2.0;
const double Xf =  2.0;
const double Yf =  2.0;
const double limite = 2.0;

complex<double> itera(complex<double> z, complex<double> c){
	return z*z+c;
	//return pow(z,6)+pow(c,-1);
	//return cos(z)+pow(c,-1);
}

int main(){
	Imagen mat(TAM_X,TAM_Y);
	double dx = (Xf-Xi)/TAM_X;
	double dy = (Yf-Yi)/TAM_Y;

	for(int i=0;i<TAM_X;i++){
		for(int j=0;j<TAM_Y;j++){
			double x = Xi + dx*i;
			double y = Yi + dy*j;
			complex<double> z, c(x,y);
			int k;
			for(k = 0; k < iteraciones; k++){
				if(abs(z)>=limite){
					break;
				}
				z = itera(z,c);
			}
			mat.en(j,i) = Color::hsl(arg(z)*180/M_PI)*((double)k/iteraciones);

		}
	}

	
	mat.guardaBMP("mandelbrot.bmp");

	return 0;
}