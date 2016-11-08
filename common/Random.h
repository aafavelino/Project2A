/** 
 * \file Random.h
 * 
 * \brief  Definition and  implementation of  the class  Random, which
 * represents  a pseudo-random  generator of  real values  in a  given
 * interval.
 *
 * \author
 * Marcelo Ferreira Siqueira \n
 * Universidade Federal do Rio Grande do Norte, \n
 * Departamento de Matem&aacute;tica, \n
 * mfsiqueira at mat (dot) ufrn (dot) br
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

#include <cstdlib>    // rand(), srand(), RAND_MAX
#include <ctime>      // time()


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
	 * \class Random
	 *
	 * \brief This  class represents a pseudo-random  generator of
	 * real values in a given interval.
	 *
	 */
	 class Random {
	 public:

		// -------------------------------------------------------
		//
		// Public methods
		//
		// -------------------------------------------------------

		/**
		 * \fn Random()
		 *
		 * \brief Creates an instance of this class.
		 *
		 */
		Random()
		{
			srand((unsigned)time(0));
		}


		/**
		 * \fn double draw() const
		 *
		 * \brief Draws a pseudo random  number from the real interval
		 * \c [0,1],  where \c was  given during the  instantiation of
		 * this generator.
		 *
		 * \return A  pseudo random number  from the real  interval \c
		 * [0,1].
		 *
		 */
		double draw()
		{
			return double(rand()) / double(RAND_MAX);
		}


		/**
		 * \fn double operator()() const
		 *
		 * \brief Overloads  the operator ()  to draw a  pseudo random
		 * number from this generator.
		 *
		 * \return A  pseudo random number  from the real  interval \c
		 * [0,1].
		 *
		 */
		double operator()()
		{
			return draw();
		}

	};

}

/** @} */ //end of group class.

