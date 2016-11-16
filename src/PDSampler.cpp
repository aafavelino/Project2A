/**
 * \file PDSampler.cpp
 *
 * \brief bla bla bla
 *
 * \author
 * Fulano e Beltrano \n
 * Universidade Federal do Rio Grande do Norte, \n
 * Instituto Metr&oacute;pole Digital, \n
 * fulano at imd (dot) ufrn (dot) br, \n
 * beltrano at imd (dot) ufrn (dot) br
 *
 * \version 1.0
 * \date May 2016
 *
 * \attention This program is distributed WITHOUT ANY WARRANTY, and it
 *            may be freely redistributed under the condition that the
 *            copyright notices  are not removed,  and no compensation
 *            is received. Private, research, and institutional use is
 *            free. Distribution of this  code as part of a commercial
 *            system  is permissible ONLY  BY DIRECT  ARRANGEMENT WITH
 *            THE AUTHOR.
 */

#include "PDSampler.h"     // PDSampler
#include "Random.h"        // Random
#include <cmath>

/**
 * \defgroup MAT309NameSpace Namespace MAT309.
 * @{
 */

/**
 * \namespace MAT309
 *
 * \brief Global namespace for MAT309 software.
 */

namespace MAT309
{

    /**
	 * \fn void PDSampler::sample(const spSurface& surface, size_t& numberOfPoints, std::vector<spPoint>& points)
	 *
	 * \brief bla bla bla
	 *
	 * \param surface A pointer to the surface to be sampled.
	 * \param numberOfPoints A reference to the number of sample points.
	 * \param points A reference to an array of sample points.
	 * 
	 */
	void
	    PDSampler::sample(
		    const spSurface& surface,
			size_t& numberOfPoints,
			std::vector<spPoint>& points
		)
	{

		this->surface = surface;

		points.clear(); 

		SamplePoint::Point pos, pos1, pos2, pos3;

		surface->GetPoint(surface->GetUMin(), surface->GetVMin(), pos._x, pos._y, pos._z );
		spSamplePoint Min0 = std::make_shared<SamplePoint>(SamplePoint(surface->GetUMin(), surface->GetVMin(), pos));

		surface->GetPoint(surface->GetUMax(), surface->GetVMin(), pos1._x, pos1._y, pos1._z );
		spSamplePoint Min1 = std::make_shared<SamplePoint>(SamplePoint(surface->GetUMax(), surface->GetVMin(), pos1) );

		surface->GetPoint(surface->GetUMin(), surface->GetVMax(), pos2._x, pos2._y, pos2._z );
		spSamplePoint Max0 = std::make_shared<SamplePoint>(SamplePoint(surface->GetUMin(), surface->GetVMax(), pos2));

		surface->GetPoint(surface->GetUMax(), surface->GetVMax(), pos3._x, pos3._y, pos3._z );
		spSamplePoint Max1 = std::make_shared<SamplePoint>(SamplePoint(surface->GetUMax(), surface->GetVMax(), pos3) );

		double trials = 0;

		bool conflicted;

		points.push_back(Min0);
		points.push_back(Min1);
		points.push_back(Max0);
		points.push_back(Max1);
		

		

		while (trials < getMT()){

			SamplePoint p;

			GerarAleatorio(p);

			spSamplePoint pontoAleatorioF = std::make_shared<SamplePoint>(p);
			
			conflicted = false;

			for (unsigned int i = 0; i < points.size(); i++){
				if ((calcularDistancia(*pontoAleatorioF,*points[i]) < 2*getAlpha()) or (calcularDistancia(*points[i],*pontoAleatorioF) < 2*getAlpha())){
					conflicted = true;
				}
			}
			if (!conflicted){

				//points.push_back(pontoAleatorio);
				points.push_back(pontoAleatorioF);
				//std::cout << "entrou"<< points.size();	
				trials = 0;

			}else {

				trials++;
			}
			
		}

	    // INSIRA CÓDIGO AQUI!

		// -----------------------------------------------------------
		//
		// Observe que a lista "points" é uma lista de shared pointers
		// para objectos da classe cdt::DtPoint.,  mas você vai ter de
		// criar pontos que são objetos  da classe SamplePoint. Isto é
		// feito assim:
		//
		// spSamplePoint myPoint = std::make_shared<SamplePoint>( SamplePoint( ARGUMENTOS DO CONSTRUTOR ) );
		//
		// Quando você  for inserir myPoint  na lista de  pontos, você
		// vai simplesmente fazer isso como se myPoint fosse um shared
		// pointer para  instâncias da  classe cdt::DtPoint  (isso vai
		// funcionar graças ao mecanismo de polimorfismo do C++):
		//
		// points.push_back( myPoint );
		//
		// A manipulação de shared_pointer é idêntica a de apontadores
		// comuns:
		//
		// Quero saber se o apontador é NULL:
		//
		// myPoint == nullptr
		//
		// Quero acessar o objeto apontador por myPOint:
		//
		// *myPoint  OU myPoint->ALGUM MEMBRO DA CLASSE cdt::DtPoint
		//
		// Você não  deve ter  nenhuma necessidade  para isso,  mas se
		// você quiser, poe fazer um cast dinâmico para obter o objeto
		// SamplePoint apontado por myPoint. Em geral, se você precisa
		// disso, há  algo não  muito adequado com  a sua  solução. De
		// todo jeito,
		//
		// spSamplePoint p = std::dynamic_pointer_cast<SamplePoint>(myPoint);
		//
		// Eu  mesmo  fiz  uso  disso   na  função  que  escreve  para
		// arquivo. Eu poderia ter evitado se tivesse sobrecarregado o
		// operador << de SamplePoint,  mas preferi terminar logo este
		// código.
		//
		// -----------------------------------------------------------

		// REMOVE A LINHA ABAIXO:
		
		numberOfPoints = points.size();

	}
	void PDSampler::GerarAleatorio(MAT309::SamplePoint &p){

			MAT309::Random *r = nullptr;
			double x = getSurface()->GetUMin() + (r->draw()*(getSurface()->GetUMax()-getSurface()->GetUMin()));
			double y = getSurface()->GetVMin() + (r->draw()*(getSurface()->GetVMax()-getSurface()->GetVMin()));
			getSurface()->GetPoint(x,y, p._pos._x,p._pos._y, p._pos._z);
			p.SetParameterCoordinates(x, y);

			
			//delete r;
		
		}
		
	double PDSampler::calcularDistancia(MAT309::cdt::DtPoint &p, MAT309::cdt::DtPoint &p1){

			double matrizJ[3][2];
			double matrizJt[2][3];
			double pp1[1][2];
			double pp1t[2][1];
			double result[1][3];
			double result1[3][1];
			double resultado_final;
			double x,y,z,x1,y1,z1;

			pp1[0][0] = p1.GetU()- p.GetU();
			pp1[0][1] = p1.GetV() - p.GetV();

			pp1t[0][0] = p1.GetU()- p.GetU();
			pp1t[1][0] = p1.GetV() - p.GetV();


			getSurface()->GetDu(p.GetU(),p.GetV(),x,y,z);
			getSurface()->GetDv(p.GetU(),p.GetV(),x1,y1,z1);

			//std::cout << x << "  " << x1 << " " << y <<" "<<y1<<" "<< z << "  "<<z1<<"\n";
			//std::cout << pp1[0][0] << "  "<<pp1[0][1]<<"\n";

			matrizJ[0][0] = x;
			matrizJ[0][1] = x1;
			matrizJ[1][0] = y;
			matrizJ[1][1] = y1;
			matrizJ[2][0] = z;
			matrizJ[2][1] = z1;


			matrizJt[0][0] = x;
			matrizJt[0][1] = y;
			matrizJt[0][2] = z;
			matrizJt[1][0] = x1;
			matrizJt[1][1] = y1;
			matrizJt[1][2] = z1;


			result[0][0] = (pp1[0][0] * matrizJt[0][0]) + (pp1[0][1] * matrizJt[1][0]);
			result[0][1] = (pp1[0][0] * matrizJt[0][1]) + (pp1[0][1] * matrizJt[1][1]);
			result[0][2] = (pp1[0][0] * matrizJt[0][2]) + (pp1[0][1] * matrizJt[1][2]);


			result1[0][0] = (pp1t[0][0] * matrizJ[0][0]) + (pp1t[1][0] * matrizJ[0][1]);
			result1[1][0] = (pp1t[0][0] * matrizJ[1][0]) + (pp1t[1][0] * matrizJ[1][1]);
			result1[2][0] = (pp1t[0][0] * matrizJ[2][0]) + (pp1t[1][0] * matrizJ[2][1]);
			
			
			resultado_final = (result[0][0] * result1[0][0]) + (result[0][1] * result1[1][0]) + (result[0][2] * result1[2][0]);
			
			return sqrt(resultado_final);
			

		}
}
/** @} */ //end of group class.
