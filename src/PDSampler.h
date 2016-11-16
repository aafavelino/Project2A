/**
 * \file PDSampler.h
 *
 *
 * \author
 * Adelino Afonso e Irene Ginani \n
 * Universidade Federal do Rio Grande do Norte, \n
 * Instituto Metr&oacute;pole Digital, \n
 * Adelino Afonso at imd (dot) ufrn (dot) br, \n
 * Irene Ginani at imd (dot) ufrn (dot) br
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

#pragma once

#include "Surface.h"           // Surface
#include "DtPoint.h"           // cdt::DtPoint
#include "SamplePoint.h"       // SamplePoint

#include <vector>              // std::vector


/**
 * \defgroup MAT309NameSpace Namespace MAT309.
 * @{
 */

/**
 * \namespace MAT309
 *
 * \brief Global namespace for MAT309 software.
 *
 */

namespace MAT309
{

	/**
	 * \class PDSampler
	 *
	 * \brief This class ...
	 *
	 */
	class PDSampler
	{
	public:

		// -----------------------------------------------------------
		//
		// Type definition
		//
		// -----------------------------------------------------------

		/**
		 * \typedef spPoint
		 *
		 * \brief Alias for a shared pointer to DtPoint objects.
		 *
		 */
		typedef cdt::DtPoint::spPoint spPoint;


		/**
		 * \typedef spSamplePoint
		 *
		 * \brief Alias for a shared pointer to SamplePoint objects.
		 *
		 */
		typedef SamplePoint::spSamplePoint spSamplePoint;


		/**
		 * \typedef spSurface
		 *
		 * \brief Alias for a shared pointer to Surface objects.
		 *
		 */
		typedef std::shared_ptr<Surface> spSurface;


	private:

		// -----------------------------------------------------------
		//
		// Private data members
		//
		// -----------------------------------------------------------

	    //
	    // DEFINA OS DADOS PRIVADOS AQUI
			double alpha = 0.1;
	    	double MT = 100;
	     	spSurface surface;
	
	    //

	public:

		// -----------------------------------------------------------
		//
		// Public methods
		//
		// -----------------------------------------------------------

		/**
		 * \fn PDSampler()
		 *
		 * \brief Creates an instance of this class.
		 *
		 */
		PDSampler() 
		{
		    // Do whatever you want.
		}

		
		/**
		 * \fn PDSampler(const PDSampler& s)
		 *
		 * \brief  Creates  an instance  of  this  class from  another
		 * instance of this class.
		 *
		 * \param s An instance of this class.
		 *
		 */
		PDSampler(const PDSampler& s)
		{
		    // Do whatever you want
		}


		/**
		 * \fn virtual ~PDSampler()
		 *
		 * \brief  Releases the  memory held  by an  instance of  this
		 * class.
		 */
		virtual ~PDSampler()
		{
		    // Do whatever you want
		}


		/**
		 * \fn void sample(const spSurface& surface, size_t& numberOfPoints, std::vector<spPoint>& points)
		 *
		 * \brief Função para espalhar a amostragem de pontos
		 *
		 * \param surface A pointer to the surface to be sampled.
		 * \param numberOfPoints A reference to the number of sample points.
		 * \param points A reference to an array of sample points.
		 * 
		 */
		void sample(
			const spSurface& surface,
			size_t& numberOfPoints,
			std::vector<spPoint>& points
		);


	
		spSurface getSurface(){
			return surface;
		}

		double getAlpha(){
			return alpha;
		}

		double getMT(){
			return MT;
		}

	private:

		// ---------------------------------------------------------------
		//
		// Private methods
		//
		// ---------------------------------------------------------------

		/**
		 * \fn GerarAleatorio(MAT309::SamplePoint &p);
		 *
		 * \brief Função para gerar os pontos aleatórios na malha
		 *
		 * \param p Malha para gerar pontos
		 * 
		 */
		void GerarAleatorio(MAT309::SamplePoint &p);

		/**
		 * \fn double calcularDistancia(MAT309::cdt::DtPoint &p, MAT309::cdt::DtPoint &p1);
		 *
		 * \brief Função destinada a calcular a distância entre pontos
		 *
		 * \param p Ponto de cordenadas
		 * \param p1 Ponto de cordenadas 
		 * 
		 */
		double calcularDistancia(MAT309::cdt::DtPoint &p, MAT309::cdt::DtPoint &p1);
	};

}

/** @} */ //end of group class.
