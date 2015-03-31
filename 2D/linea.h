#ifndef LINEA2D_H
#define LINEA2D_H

#include "punto.h"
#include <cmath>

namespace _2D{
	template<class T>
	class Linea{
		public:
			Punto<T> inicio;
			Punto<T> fin;
			Linea<T>();
			Linea<T>(const Punto<T>& a,const Punto<T>& b);
			Linea<T>(const Punto<T>& ini, T r, T a);
			T angulo() const;
			Linea<T> rotar(T ang)const ;		
			T longitud() const;
			T longitud2() const;
			Linea invierte()const;
			Linea operator*(T d) const;
			Linea operator*=(T d);
			Linea operator/(T d) const;
			Linea operator/=(T d);
			std::string toString()const{
				return inicio.toString()+std::string("->")+fin.toString();
			}
			
		public:
			bool colisiona(const Linea<T>& b)const {
				return (ccw(inicio,fin,b.inicio) * ccw(inicio, fin, b.fin) < 0) and (ccw(b.inicio, b.fin, inicio)*ccw(b.inicio, b.fin, fin)<0);
			}

	};

	template<class T>
	Punto<T> interseccion(const Linea<T>& a,const Linea<T>& b);
	
	template<class T>
	Linea<T>::Linea():inicio(),fin(){
	}

	template<class T>
	Linea<T>::Linea(const Punto<T>& a,const Punto<T>& b){
		inicio=a;
		fin=b;
	}
	
	template<class T>
	Linea<T>::Linea(const Punto<T>& ini, T r, T a){
		inicio=ini;
		fin=Punto<T>(ini.x+r*cos(a), ini.y+r*sin(a));
	}

	template<class T>
	T Linea<T>::angulo()const {
		return (fin - inicio).angulo();
	}

	template<class T>
	Linea<T> Linea<T>::rotar(T ang)const {
		return Linea(inicio, longitud(), angulo()+ang); 
	}


	template<class T>
	T Linea<T>::longitud()const{
		return (fin-inicio).norma();
	}
	template<class T>
	T Linea<T>::longitud2()const{
		return (fin-inicio).norma();
	}

	template<class T>
	Linea<T> Linea<T>::operator*(T d)const{
		return Linea(inicio, longitud()*d, angulo());
	}
	template<class T>
	Linea<T> Linea<T>::operator*=(T d){
		*this=*this*d;
		return *this;
	}
	template<class T>
	Linea<T> Linea<T>::invierte()const{
		return Linea<T>(fin,inicio);
	}

	template<class T>
	Linea<T> Linea<T>::operator/(T d) const{
		return Linea(inicio/d,fin/d);
	}

	template<class T>
	Linea<T> Linea<T>::operator/=(T d){
		*this=*this/d;
		return *this;		
	}
}
#endif
