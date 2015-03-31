#ifndef RECTANGULO2D_H
#define RECTANGULO2D_H
#include "../macros.h"
#include "linea.h"
	namespace _2D{

	template<class T>
	class Rectangulo : public Linea<T> {
		public:
			Rectangulo():Linea<T>(){}
			Rectangulo(Punto<T> a, Punto<T> b):Linea<T>(a, b){}
			Rectangulo(Punto<T> a, T b, T c): Linea<T>(a,a+Punto<T>(b,c)){}
			T base() const{
				return this->fin.x-this->inicio.x;
			}
			T altura() const{
				return this->fin.y-this->inicio.y;
			}
			T area() const{
				return base()*altura();
			}
			Punto<T> centro()const{
				return Punto<T>((this->inicio.x+this->fin.x)/2,(this->inicio.y+this->fin.y)/2);
			}
			Rectangulo<T> interseccion(Rectangulo<T> o)const{
				Punto<T> a(MAYOR(this->inicio.x, o.inicio.x), MAYOR(this->inicio.y, o.inicio.y));
				Punto<T> b(MENOR(this->fin.x, o.fin.x), MENOR(this->fin.y, o.fin.y));
				if(a.x > b.x or a.y > b.y)
					return Rectangulo<T>();
				else 
					return Rectangulo<T>(a, b);
			}
	};
}
#endif