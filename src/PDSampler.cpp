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
		


	}

}
/** @} */ //end of group class.
