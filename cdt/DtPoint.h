/**
 * \file DtPoint.h
 *
 * \brief Definition of a class to represent the geometric information
 * of a vertex of the  constrained Delaunay triangulation (CDT) in the
 * quad-edge data structure. Any derived  class from this class can be
 * given to the constructor of the vertex class. The CDT will be built
 * normally.
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

 // ------------------------------------------------------------------
 //
 // For more details  regarding the quad-edge data  structure, see the
 // paper
 //
 // "Primitives for  the manipulation of general  subdivisions and the
 // computation  of Voronoi  Diagrams", by  Leonidas Guibas  and Jorge
 // Stolfi,  published  in the  ACM  Transactions  on Graphics,  4(2),
 // p. 74-123, 1985.
 //
 // Jorge Stolfi  made available his  own code for the  quad-edge data
 // structure:
 //
 // http://www.ic.unicamp.br/~stolfi/EXPORT/software/c/Index.html
 //
 // ------------------------------------------------------------------

#pragma once

#include <sstream>     // std::stringstream
#include <iostream>    // std::iostream, std::fixed
#include <iomanip>     // std::setprecision()
#include <memory>      // std::shared_ptr


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
	 * \defgroup CDTNameSpace Namespace cdt.
	 * @{
	 */

	 /**
	  * \namespace cdt
	  *
	  * \brief  The   namespace  cdt   contains  the   definition  and
	  * implementation   of  classes   to  build   and  manipulate   a
	  * constrained Delaunay triangulation (CDT) using an augmentation
	  * of the quad-edge data structure.
	  */

	namespace cdt {


		/**
		 * \class DtPoint
		 *
		 * \brief This class represents the geometric information of a
		 * point in \f$E^2\f$.
		 */
		class DtPoint
		{
		public:

			// -------------------------------------------------------
			//
			// Type definitions
			//
			// -------------------------------------------------------

			/**
			 * \typedef spPoint
			 *
			 * \brief Alias for a shared pointer to DtPoint objects.
			 *
			 */
			typedef std::shared_ptr<DtPoint> spPoint;


		protected:

			// -------------------------------------------------------
			//
			// Protected data members
			//
			// -------------------------------------------------------

			double _u;    ///< The first Cartesian coordinate of the vertex.
			double _v;    ///< The second Cartesian coordinate of the vertex.   

		public:

			// -------------------------------------------------------
			//
			// Public methods
			//
			// -------------------------------------------------------

			/**
			 * \fn DtPoint()
			 *
			 * \brief Creates an instance of this class.
			 *
			 */
			DtPoint() : _u(0), _v(0)
			{
			}


			/**
			 * \fn DtPoint(double u, double v)
			 *
			 * \brief Creates an instance of this class.
			 *
			 * \param u The first Cartesian coordinate of this point.
			 * \param v The second Cartesian coordinate of this point.
			 *
			 */
			DtPoint(
				double u,
				double v
			) 
				:
				_u(u),
				_v(v)
			{
			}


			/**
			 * \fn DtPoint(const DtPoint& p)
			 *
			 * \brief Creates  an instance of this  class from another
			 * instance of this class.
			 *
			 * \param p An instance of this class.
			 *
			 */
			DtPoint(
				const DtPoint& p
			) 
				: 
				_u(p._u),
				_v(p._v)
			{
			}


			/**
			 * \fn virtual ~DtPoint()
			 *
			 * \brief Releases the memory held  by an instance of this
			 * class.
			 *
			 */
			virtual ~DtPoint()
			{
			}


			/**
			 * \fn double GetU() const
			 *
			 * \brief Returns  the first Cartesian coordinate  of this
			 * point.
			 *
			 * \return The first Cartesian coordinate of this point.
			 *
			 */
			double GetU() const
			{
				return _u;
			}


			/**
			 * \fn double GetV() const
			 *
			 * \brief Returns the second  Cartesian coordinate of this
			 * point.
			 *
			 * \return The second Cartesian coordinate of this point.
			 *
			 */
			double GetV() const
			{
				return _v;
			}


			/**
			 * \fn void SetParameterCoordinates(double u, double v)
			 *
			 * \brief Assign  values to  the parameter  coordinates of
			 * the point.
			 *
			 * \param u The value to be assigned to the u-coordinate.
			 * \param v The value to be assigned to the v-coordinate.
			 *
			 */
			void
				SetParameterCoordinates(
					double u,
					double v
				)
			{
				_u = u;
				_v = v;

				return;
			}


			/**
			 * \fn DtPoint& operator=(const DtPoint& p)
			 *
			 * \brief Makes this point the same as a given point.
			 *
			 * \param p A given point.
			 *
			 */
			DtPoint&
				operator=(
					const DtPoint& p
				)
			{
				_u = p.GetU();
				_v = p.GetV();

				return *this;
			}


			/**
			 * \fn bool operator==(const DtPoint& p) const
			 *
			 * \brief Returns the Boolean value  true if, and only if,
			 * this point is the same as a given one.
			 *
			 * \param p A given point.
			 *
			 * \return The  Boolean value  true if  this point  is the
			 * same  as  a given  one,  and  the Boolean  value  false
			 * otherwise.
			 *
			 */
			bool
				operator==(
					const DtPoint& p
				) 
				const
			{
				return (GetU() == p.GetU()) && (GetV() == p.GetV());
			}


			/**
			 * \fn bool operator!=(const DtPoint& p) const
			 *
			 * \brief Returns the Boolean value  true if, and only if,
			 * this point is not the same as a given one.
			 *
			 * \param p A given point.
			 *
			 * \return The Boolean value true if this point is not the
			 * same  as  a given  one,  and  the Boolean  value  false
			 * otherwise.
			 *
			 */
			bool 
				operator!=(
					const DtPoint& p
				) 
				const
			{
				return !(*this == p);
			}


			/**
			 * \fn virtual void GetFormattedData(std::stringstream& formattedData) const
			 *
			 * \brief Creates an string  with the data associated with
			 * this point.
			 *
			 * \param formattedData  A reference to a  string with the
			 * data associated with this point.
			 *
			 */
			virtual 
			void
				GetFormattedData(
					std::stringstream& formattedData
				) 
				const
			{
				formattedData	<< std::setprecision(18)
								<< std::fixed;

				formattedData	<< GetU()
								<< '\t'
								<< GetV();
			}

		};

	}

}

/** @} */ //end of group class.
