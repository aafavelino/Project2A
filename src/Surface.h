/**
 * \file Surface.h
 *
 * \brief This  file contains the  definition of an abstract  class to
 * represent parametric surfaces in  3D space defined over rectangular
 * domains.
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
	 * \class Surface
	 *
	 * \brief  This  class  is   an  abstract  class  that  represents
	 * parametric  surfaces  in  3D  space  defined  over  rectangular
	 * domains.
	 *
	 */
	class Surface
	{
	public:

		// -----------------------------------------------------------
		//
		// Public methods
		//
		// -----------------------------------------------------------

		/**
		 * \fn Surface()
		 *
		 * \brief Creates an instance of this class.
		 *
		 */
		Surface() 
		{
		}

		
		/**
		 * \fn virtual ~Surface()
		 *
		 * \brief  Releases the  memory held  by an  instance of  this
		 * class.
		 */
		virtual ~Surface()
		{
		}


		/**
		 * \fn virtual double GetUMin() const = 0
		 *
		 * \brief Returns the lower bound  for the first coordinate of
		 * a domain point.
		 *
		 * \return  The lower  bound  for the  first  coordinate of  a
		 * domain point.
		 * 
		 */
		virtual double GetUMin() const = 0;


		/**
		 * \fn virtual double GetUMax() const = 0
		 *
		 * \brief Returns the upper bound  for the first coordinate of
		 * a domain point.
		 *
		 * \return  The lower  bound  for the  first  coordinate of  a
		 * domain point.
		 * 
		 */
		virtual double GetUMax() const = 0;


		/**
		 * \fn virtual double GetVMin() const = 0
		 *
		 * \brief Returns the lower bound  for the second coordinate of
		 * a domain point.
		 *
		 * \return  The lower  bound for  the second  coordinate of  a
		 * domain point.
		 * 
		 */
		virtual double GetVMin() const = 0;


		/**
		 * \fn virtual double GetVMax() const = 0
		 *
		 * \brief Returns the upper bound for the second coordinate of
		 * a domain point.
		 *
		 * \return  The lower  bound for  the second  coordinate of  a
		 * domain point.
		 * 
		 */
		virtual double GetVMax() const = 0;


		/**
		 * \fn virtual void GetPoint(double u, double v, double& x, double& y, double& z) const = 0
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
		virtual void
		    GetPoint(
				double u,
				double v,
				double& x,
				double& y,
				double& z
			)
		    const = 0 ;


		/**
		 * \fn virtual void GetDu(double u, double v, double& x, double& y, double& z) const = 0
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
		virtual void
		    GetDu(
				double u,
				double v,
				double& x,
				double& y,
				double& z
			)
		    const = 0 ;



	    /**
		 * \fn virtual void GetDv(double u, double v, double& x, double& y, double& z) const = 0
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
		virtual void
		    GetDv(
				double u,
				double v,
				double& x,
				double& y,
				double& z
			)
		    const = 0 ;

	};

}

/** @} */ //end of group class.
