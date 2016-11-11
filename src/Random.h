#include <iostream>
#include "../common/Random.h"
using namespace std;
struct par
{
	double x;
	double y;
};

class RandomPar{

private:

	par u;
	par v;
	par m;

public:

	RandomPar(par &u, par &v){
		this->u.x = u.x;
		this->u.y = u.y;
		this->v.x = v.x;
		this->v.y = v.y;
		
	}
	par getU(){
		return u;
	}
	par getV(){
		return v;
	}
	par GerarAleatorio(){
		MAT309::Random r;
		double menorX, maiorX;
		double menorY,maiorY;

		if(u.x < v.x){
			menorX=u.x;
			maiorX=v.x;
		}
		else {
			menorX=v.x;
			maiorX=u.x;
		}
		if(u.y < v.y){
			menorY=u.y;
			maiorY=v.y;
		}else{
			menorY=v.y;
			maiorY=u.y;
		}

		m.x = menorX + (r.draw()*(maiorX-menorX));
		m.y = menorY + (r.draw()*(maiorY-menorY));

		return m;
	}
};
