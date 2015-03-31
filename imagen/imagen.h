#ifndef IMAGEN_H
#define IMAGEN_H

#include <cstring>
#include "color.h"
#include <algorithm>
class Imagen{
	private:
		Color*pixels;
		int x, y;
	public:
		Imagen(int a, int b){
			x=a;
			y=b;
			pixels= new Color[x*y];
		}
		
		Imagen(){
			pixels=NULL;
			x=y=0;
		}
		
		Imagen(int a, int b, Color c){
			x=a;
			y=b;
			pixels = new Color[x*y];
			for(int i=0;i<x*y;i++)
				pixels[i]=c;
		}

		Imagen(const Imagen &I){
			x=I.x;
			y=I.y;
			pixels=new Color[x*y];
			for(int i=0;i<x*y;i++)
				pixels[i]=I.pixels[i];
		}

		
		/*
		Imagen(Imagen&& I){
			x=I.x;
			y=I.y;
			pixels = I.pixels;
		}
		Imagen operator=(Imagen&& I){
			x=I.x;
			y=I.y;
			pixels = I.pixels;
			return *this;
		}
		*/
		int filas()const{
			return y;
		}
		
		int columnas()const{
			return x;
		}
		
		Color& en(int i, int j){
			if(i < 0 or i >= y or j < 0 or j >= x )
				throw 0;
			//std::cout<<i*x+j<<std::endl;
			return pixels[i*x+j];
		}

		Color en(int i, int j)const{
			if(i < 0 or i >= y or j < 0 or j >= x )
				throw 0;
			//std::cout<<i*x+j<<std::endl;
			return pixels[i*x+j];
		}
		
		Imagen escalaDeGrises()const{
			Imagen ret(x,y);
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++)
					ret.en(i,j) = en(i,j).aGris();
			return ret;
		}

		Imagen derivadax()const {
			Imagen ret(x,y);
			for(int i=0;i<y;i++){
				ret.en(i,0)=en(i,0);
				for(int j=1;j<x;j++)
					ret.en(i,j) =maxDif(en(i,j),en(i,j-1));
			}
			return ret;
		}

		Imagen derivaday()const{
			Imagen ret(x,y);
			for(int i=0;i<x;i++){
				ret.en(0,i)=en(0,i);
				for(int j=1;j<y;j++)
					ret.en(j,i) = maxDif(en(j,i),en(j-1,i));
			}
			return ret;
		}

		Imagen derivada()const{
			Imagen dx=derivadax();
			Imagen dy=derivaday();
			Imagen ret(x,y);

			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++)
					ret.en(i,j) =max(dx.en(i,j),dy.en(i,j));
			}
			return ret;
		}


		Imagen umbral(unsigned char min=128, Color down=Color())const{
			Imagen ret(x,y);
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++)
					ret.en(i,j) = (en(i,j).luz()>min?en(i,j):down);
			return ret;
		}

		Imagen umbral(unsigned char min, const Color& down,const Color& up)const{
			Imagen ret(x,y);
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++)
					ret.en(i,j) = (en(i,j).luz()>min?up:down);
			return ret;
		}

		void copiar(int h, int k, Imagen &o)const{
			for(int i=0;i<o.y;i++)
				for(int j=0;j<o.x;j++){
					int p,q;
					p=h+i;
					q=k+j;
					try{
						en(p,q) = o.en(i,j);
					}catch(int e){
					
					}
				}
		}
	
		~Imagen(){
			delete[] pixels;
		}
		
		Imagen operator=(const Imagen& I){
			delete pixels;
			x=I.x;
			y=I.y;
			pixels = new Color[x*y];
			for(int i=0;i<x*y;i++)
				pixels[i]=I.pixels[i];
			return *this;
		}

		
		Imagen filtroMatriz(double coeficientes[3][3])const {
			Imagen ret(x,y);
			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++){
					double sr=0;
					double sg=0;
					double sb=0;
					for(int k=-1;k<2;k++)
						for(int l=-1;l<2;l++){
							try{
								Color aux=en(i+k,j+l);
								sr+=(double)aux.r*coeficientes[k+1][l+1];
								sg+=(double)aux.g*coeficientes[k+1][l+1];
								sb+=(double)aux.b*coeficientes[k+1][l+1];
							}catch(int e){
							}
						}

					if(sr>255)
						sr=255;
					if(sr<0)
						sr=0;
					if(sg>255)
						sg=255;
					if(sg<0)
						sg=0;
					if(sb>255)
						sb=255;
					if(sb<0)
						sb=0;
					ret.en(i,j) = Color(sr,sg,sb);
				}
				//ret.guardaBMP("filtro.bmp");
			}
			return ret;
		}

		Imagen filtroGauss()const{
			double coeficientes[3][3]={
				{1.0/21,1.0/7,1.0/21},
				{1.0/7,5.0/21,1.0/7},
				{1.0/21,1.0/7,1.0/21}
			};
			return filtroMatriz(coeficientes);	
		}

		Imagen filtroAfilador()const{
			double coeficientes[3][3]={
			 {  0, -1,  0},
    		 {-1,  5, -1},
     		 {	0, -1,  0 }
     		};
     		return filtroMatriz(coeficientes);
		}

		Imagen filtroMediana()const{
			double coeficientes[3][3]={
				{1.0/9.0,1.0/9.0,1.0/9.0},
				{1.0/9.0,1.0/9.0,1.0/9.0},
				{1.0/9.0,1.0/9.0,1.0/9.0}
			};
			return filtroMatriz(coeficientes);
		}

		Imagen erosionar()const{
			Imagen ret(x,y);
			int dx[] = {1,-1,0,0};
			int dy[] = {0,0,1,-1};
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++){
					Color c=en(i,j);
					for(int k=0;k<4;k++){
						try{
							Color z=en(i+dy[k],j+dx[k]);
							c=min(c,z); 
						}catch(int e){

						}
					}
					ret.en(i,j) = c;
				}
			return ret;
		}

		Imagen dilatar()const{
			Imagen ret(x,y);
			int dx[] = {1,-1,0,0};
			int dy[] = {0,0,1,-1};
			for(int i=0;i<y;i++)
				for(int j=0;j<x;j++){
					Color c=en(i,j);
					for(int k=0;k<4;k++){
						try{
							Color z=en(i+dy[k],j+dx[k]);
							c=max(c,z); 
						}catch(int e){

						}
					}
					ret.en(i,j) = c;
				}
			return ret;
		}

		Imagen laplace()const{
			double coeficientes[3][3]={
			 {  -1, -1,  -1},
    		 {-1,  8, -1},
     		 {	-1, -1,  -1 }
     		};
     		return filtroMatriz(coeficientes);
		}

		void guardaBMP(const char *nombre){
			std::ofstream f(nombre);
			f.put('B'); f.put('M'); //Tipo
			int tam=54+x*y;
			f.write((char*)&tam,sizeof(int)); 
			int reservado=0;
			f.write((char*)&reservado,sizeof(int));  
			int offset=0x36;
			f.write((char*)&offset,sizeof(int));  
			int tamCabeceras=40;
			f.write((char*)&tamCabeceras,sizeof(int));
			f.write((char*)&(x),sizeof(int)); //ancho
			f.write((char*)&(y),sizeof(int)); //alto
			short planos=1;
			f.write((char*)&planos,sizeof(short));
			short bits=24;
			f.write((char*)&bits,sizeof(short));
			int compresion=0;
			f.write((char*)&compresion,sizeof(int));
			f.put(0); f.put(0); f.put(0); f.put(0); //Tama�o Paleta
			f.put(0); f.put(0); f.put(0); f.put(0); //BitsPorMetroX
			f.put(0); f.put(0); f.put(0); f.put(0); //BitsPorMetroY
			f.put(0); f.put(0); f.put(0); f.put(0); //Colores Usados
			f.put(0); f.put(0); f.put(0); f.put(0); //Colores Importantes
			int ajuste=(4-(x*3)%4)%4;
			//std::cout<<f.tellp()<<std::endl;
			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++)
					try{f<<en(i,j);}catch(int t){f<<Color();}
				for(int j=0;j<ajuste;j++)
					f<<'\0';
			}
			f.close();
		}
		
		void leerBMP(const char *nombre){
			delete pixels;
			std::ifstream f(nombre);
			if(f.get()!='B' or f.get()!='M'){
				std::cout<<"No es BMP"<<std::endl;
			}
			int tam;
			f.read((char*)&tam,sizeof(int));
			f.get(); f.get(); f.get(); f.get(); //Reservado
			f.get(); f.get(); f.get(); f.get(); //Offset
			f.get(); f.get(); f.get(); f.get(); //Tama�o cabecera
			f.read((char*)&x,sizeof(int)); //x++;
			f.read((char*)&y,sizeof(int)); //y++;
			f.get(); f.get(); //Planos
			short bits;
			f.read((char*)&bits,sizeof(short));
			f.get(); f.get(); f.get(); f.get(); //Compresion
			f.get(); f.get(); f.get(); f.get(); //Tama�o Paleta
			f.get(); f.get(); f.get(); f.get(); //BitsPorMetroX
			f.get(); f.get(); f.get(); f.get(); //BitsPorMetroY
			f.get(); f.get(); f.get(); f.get(); //Colores Usados
			f.get(); f.get(); f.get(); f.get(); //Colores Importantes
			pixels=new Color[x*y];
			int ajuste=(4-(x*3)%4)%4;
			f.seekg(-((x+ajuste)*y*3),f.end);
			for(int i=0;i<y;i++){
				for(int j=0;j<x;j++){
					//std::cout<<x-j<<std::endl;
					f>>en(i,j);
				}
				for(int j=0;j<ajuste;j++)
					f.get();
			}
			f.close();
		}
		
		Imagen region(int p, int q, int h, int k, Imagen& r)const{
			delete r.pixels;
			r.x=h;
			r.y=k;
			r.pixels = new Color[h*k];
			
			for(int i=0;i<k;i++)
				for(int j=0;j<h;j++)
					try{
						r.en(i,j) = en(q+i,p+j);
					}catch(int e){
					}
			
			
			return r;
		}

		
		Imagen region(int p, int q, int h, int k)const{
			Imagen r;
			r.x=h;
			r.y=k;
			r.pixels = new Color[h*k];
			
			for(int i=0;i<k;i++)
				for(int j=0;j<h;j++)
					try{
						r.en(i,j) = en(q+i,p+j);
					}catch(int e){
					}
			
			
			return r;
		}



		Color& operator()(int x, int y){
			return this->en(x,y);
		}


	friend void drawAt(const Imagen& M, int h, int k);
	friend inline void glReadPixels(Imagen &M);
	friend inline void glReadPixels(Imagen &M ,int x2, int y2);	
	friend inline void glDrawPixels(const Imagen &M);

};



void dibujaLinea(Imagen &M, int y1,int  x1,int y2, int x2,const Color& c){
		int dx=abs(x2-x1);
		int dy=abs(y2-y1);
		if(dy==0){
			if(x2<x1)
			std::swap(x1,x2);
			for(;x1<=x2;x1++)
			try{M(x1,y1)=c;}catch(int n){}
		}else if(dx==0){
			if(y1>y2)
			std::swap(y1,y2);
			for(;y1<=y2;y1++)
			try{M(x1,y1)=c;}catch(int n){}
			
		}else if(dx==dy){
			if(x1>x2){
				std::swap(x1,x2);
				std::swap(y1, y2);
			}
			int incremento=(y1<y2?1:-1);
			for(;x1<=x2;x1++,y1+=incremento){
				try{M(x1,y1)=c;}catch(int n){}
			}
		}
		else if(dy<dx){
			if(x1>x2){
				std::swap(x1,x2);
				std::swap(y1, y2);
			}
			int x=x1, y=y1;
			int incremento=(y1<y2?1:-1);
			int p=0;
			p=2*dy-dx;
			while(x!=x2 or y!=y2){
				try{M(x,y)=c;}catch(int n){}
				if(p>=0){
					x++;
					y+=incremento;
					p=p+2*dy-2*dx;
				}else{
					x++;
					p=p+2*dy;
				}
			}
		}else{	
			if(y1>y2){
				std::swap(x1,x2);
				std::swap(y1, y2);
			}
			int y=y1, x=x1;
			int incremento=(x1<x2?1:-1);
			int p=0;
			p=2*dx-dy;
			while(y!=y2 or x!=x2){
				try{M(x,y)=c;}catch(int n){}
				if(p>0){
					y++;
					x+=incremento;
					p=p+2*dx-2*dy;
				}else{
					y++;
					p=p+2*dx;
				}
			}
		}
		try{M(x2,y2)=c;}catch(int n){}
	}

#endif