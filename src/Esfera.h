/**
 * \file MyCylinder.h
 *
 * \brief This file contains the definition of a class to represent an
 * open cylinder defined  by a parametric equation  over a rectangular
 * domain.
 *
 * \author
 * Marcelo Ferreira Siqueira \n
 * Universidade Federal do Rio Grande do Norte, \n
 * Departamento de Matem&aacute;tica, \n
 * mfsiqueira at mat (dot) ufrn (dot) br
 *
 * \version 1.0
 * \date October 2016
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

#include "Surface.h"       // Surface

#include <cmath>           // acos
#include <stdexcept>       // std::runtime_error
#include <cmath>

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
	 * \class MyCylinder
	 *
	 * \brief This  class is  an abstract  class that  represents open
	 * cylinder defined  by a  parametric equation over  a rectangular
	 * domain.
	 *
	 */
	class Esfera : public Surface
	{
	private:

		// -----------------------------------------------------------
		//
		// Private data members
		//
		// -----------------------------------------------------------

	    const double _u0;          ///< Lower bound for the first coordinate of a point in the parameter domain.
		const double _u1;          ///< Upper bound for the first coordinate of a point in the parameter domain.
		const double _v0;          ///< Lower bound for the second coordinate of a point in the parameter domain.
		const double _v1;          ///< Upper bound for the second coordinate of a point in the parameter domain.
		const double _radius;      ///< Radius of the cylinder.
		
	  
	public:

		// -----------------------------------------------------------
		//
		// Public methods
		//
		// -----------------------------------------------------------

		/**
		 * \fn MyCylinder()
		 *
		 * \brief Creates an instance of this class.
		 *
		 */
	    Esfera() : _u0(0), _u1( 2*acos(-1)), _v0(0), _v1(10) , _radius(5)
		{
		}

		
		/**
		 * \fn virtual ~MyCylinder()
		 *
		 * \brief  Releases the  memory held  by an  instance of  this
		 * class.
		 */
		virtual ~Esfera()
		{
		}


		/**
		 * \fn double GetUMin() const
		 *
		 * \brief Returns the lower bound  for the first coordinate of
		 * a domain point.
		 *
		 * \return  The lower  bound  for the  first  coordinate of  a
		 * domain point.
		 * 
		 */
		double GetUMin() const
		{
			return _u0;
		}


		/**
		 * \fn double GetUMax() const
		 *
		 * \brief Returns the upper bound  for the first coordinate of
		 * a domain point.
		 *
		 * \return  The lower  bound  for the  first  coordinate of  a
		 * domain point.
		 * 
		 */
		double GetUMax() const
		{
			return _u1;
		}


		/**
		 * \fn double GetVMin() const
		 *
		 * \brief Returns the lower bound  for the second coordinate of
		 * a domain point.
		 *
		 * \return  The lower  bound for  the second  coordinate of  a
		 * domain point.
		 * 
		 */
		double GetVMin() const
		{
			return _v0;
		}


		/**
		 * \fn double GetVMax() const
		 *
		 * \brief Returns the upper bound for the second coordinate of
		 * a domain point.
		 *
		 * \return  The lower  bound for  the second  coordinate of  a
		 * domain point.
		 * 
		 */
		double GetVMax() const
		{
			return _v1;
		}


		/**
		 * \fn void GetPoint(double u, double v, double& x, double& y, double& z) const
		 *
		 * \brief Computes a point on the surface at a given parameter
		 * point.
		 *
		 * \param u First coordinate of a parameter point.
		 * \param v Second coordinate of a parameter point.
		 * \param x A  reference to the first coordinate  of the point
		 * on the surface.
		 * \param y A reference to  the second coordinate of the point
		 * on the surface.
		 * \param z A  reference to the third coordinate  of the point
		 * on the surface.
		 * 
		 */
		void
		    GetPoint(
				double u,
				double v,
				double& x,
				double& y,
				double& z
			)
		    const
		{
			if ((u < GetUMin()) || (u > GetUMax()) || (v < GetVMin()) || (v > GetVMax()))
			{
				throw std::runtime_error("The given parameter point is outside the parameter domain.");
			}

			x = _radius * cos( u ) * sin( v );
			y = _radius * sin( u ) * sin( v ) ;
			z = _radius * cos( v );

			return;
		}


		/**
		 * \fn void GetDu(double u, double v, double& x, double& y, double& z) const
		 *
		 * \brief  Computes the  first  derivative vector  at a  given
		 * parameter point in direction u.
		 *
		 * \param u First coordinate of a parameter point.
		 * \param v Second coordinate of a parameter point.
		 * \param x A  reference to the first coordinate  of the first
		 * derivative vector in direction \c u.
		 * \param y A reference to  the second coordinate of the first 
		 * derivative vector in direction \c u.
		 * \param z A  reference to the third coordinate  of the first
		 * derivative vector in direction \c u.
		 * 
		 */
		void
		    GetDu(
				double u,
				double v,
				double& x,
				double& y,
				double& z
			)
		    const
		{
			if ((u < GetUMin()) || (u > GetUMax()) || (v < GetVMin()) || (v > GetVMax()))
			{
				throw std::runtime_error("The given parameter point is outside the parameter domain.");
			}

			x = -_radius * sin( u ) * sin( v );
			y =  _radius * cos( u ) * sin( v );
			z = 0 ;
			return;
		}



	    /**
		 * \fn void GetDv(double u, double v, double& x, double& y, double& z) const
		 *
		 * \brief  Computes the  first  derivative vector  at a  given
		 * parameter point in direction v.
		 *
		 * \param u First coordinate of a parameter point.
		 * \param v Second coordinate of a parameter point.
		 * \param x A  reference to the first coordinate  of the first
		 * derivative vector in direction \c v.
		 * \param y A reference to  the second coordinate of the first 
		 * derivative vector in direction \c v.
		 * \param z A  reference to the third coordinate  of the first
		 * derivative vector in direction \c v.
		 * 
		 */
		void
		    GetDv(
				double u,
				double v,
				double& x,
				double& y,
				double& z
			)
		    const
		{
			if ((u < GetUMin()) || (u > GetUMax()) || (v < GetVMin()) || (v > GetVMax()))
			{
				throw std::runtime_error("The given parameter point is outside the parameter domain.");
			}
			
			x = _radius * cos( u ) * cos( v );
			y = _radius * sin( u ) * cos( v ) ;
			z = -_radius * sin( v );

			return;
		}

	};

}

/** @} */ //end of group class.
