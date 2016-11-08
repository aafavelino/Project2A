/**
 * \file SamplePoint.h
 *
 * \brief This  file contains the  definition of a class  to represent
 * the space  and the parameter  coordinates of  a point on  a surface
 * patch.
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

#include "DtPoint.h"           // cdt::DtPoint

#include <sstream>             // std::stringstream
#include <iostream>            // std::iostream, std::fixed
#include <iomanip>             // std::setprecision()


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
	 * \class SamplePoint
	 *
	 * \brief  This   class  represents  the  3D   and  the  parameter
	 * coordinates of a point on a surface patch.
	 *
	 */
	class SamplePoint : public cdt::DtPoint
	{
	public:

		// -----------------------------------------------------------
		//
		// Type definition
		//
		// -----------------------------------------------------------


		/**
		 * \typedef spSamplePoint
		 *
		 * \brief Alias for a shared pointer to SamplePoint objects.
		 *
		 */
		typedef std::shared_ptr<SamplePoint> spSamplePoint;


		/**
		 * \struct Point
		 *
		 * \brief This struct represents the coordinates of a point in
		 * 3D.
		 *
		 */
		struct Point
		{
			double _x;    ///< First Cartesian coordinate of a point in 3D.
			double _y;    ///< Second Cartesian coordinate of a point in 3D.
			double _z;    ///< Third Cartesian coordinate of a point in 3D.

			/**
			 * \fn Point()
			 *
			 * \brief Creates an instance of this struct.
			 *
			 */
			Point() : _x(0), _y(0), _z(0)
			{
			}

			/**
			 * \fn Point(double x, double y, double z)
			 *
			 * \brief Creates an instance of this struct.
			 *
			 * \param x Value of the first Cartesian coordinate.
			 * \param y Value of the second Cartesian coordinate.
			 * \param z Value of the third Cartesian coordinate.
			 *
			 */
			Point(double x, double y, double z) : _x(x), _y(y), _z(z)
			{
			}


			/**
			 * \fn Point(const Point& pt)
			 *
			 * \brief Creates an instance  of this struct from another
			 * instance of this struct.
			 *
			 * \param pt A given instance of this struct.
			 *
			 */
			Point(const Point& pt) : _x(pt._x), _y(pt._y), _z(pt._z)
			{
			}


			/**
			 * \fn bool operator==(const Point& pt) const
			 *
			 * \brief Returns  the Boolean value  true if and  only if
			 * this point has the same coordinates as a given point.
			 *
			 * \param pt A given point.
			 *
			 * \return The  Boolean value true  if this point  has the
			 * same Cartesian  coordinates as  a given point,  and the
			 * Boolean value false otherwise.
			 *
			 */
			bool operator==(const Point& pt) const
			{
				return (_x == pt._x) && (_y == pt._y) && (_z == pt._z);
			}


			/**
			 * \fn Point& operator=(const Point& pt)
			 *
			 * \brief Makes this point the same as a given point.
			 *
			 * \param pt A given point.
			 *
			 */
			Point& operator=(const Point& pt)
			{
				_x = pt._x;
				_y = pt._y;
				_z = pt._z;

				return *this;
			}

		};


	public:

		// ------------------------------------------------------------
		//
		// Protected data members
		//
		// ------------------------------------------------------------

		Point  _pos;   ///< 3D Cartesian coordinates of a point on the surface.

	public:

		// ---------------------------------------------------------------
		//
		// Public methods
		//
		// ---------------------------------------------------------------

		/**
		 * \fn SamplePoint()
		 *
		 * \brief Creates an instance of this class.
		 *
		 */
		SamplePoint() : cdt::DtPoint(), _pos(Point())
		{
		}


		/**
		 * \fn SamplePoint(double u, double v)
		 *
		 * \brief Creates an instance of this class.
		 *
		 * \param u The first Cartesian coordinate of this point.
		 * \param v The second Cartesian coordinate of this point.
		 *
		 */
		SamplePoint(double u, double v) : cdt::DtPoint(u, v), _pos(Point())
		{
		}


		/**
		 * \fn SamplePoint(double u, double v, const Point& pos)
		 *
		 * \brief Creates an instance of this class.
		 *
		 * \param u The first Cartesian coordinate of this point.
		 * \param v The second Cartesian coordinate of this point.
		 * \param pos The  3D Cartesian coordinates of a  point on the
		 * surface.
		 *
		 */
		SamplePoint(
			double u,
			double v,
			const Point& pos
		) :
			cdt::DtPoint(u, v),
			_pos(pos)
		{
		}


		/**
		 * \fn SamplePoint(const SamplePoint& p)
		 *
		 * \brief  Creates  an instance  of  this  class from  another
		 * instance of this class.
		 *
		 * \param p An instance of this class.
		 *
		 */
		SamplePoint(const SamplePoint& p) : cdt::DtPoint(p.GetU(), p.GetV()), _pos(p._pos)
		{
		}


		/**
		 * \fn virtual ~SamplePoint()
		 *
		 * \brief  Releases the  memory held  by an  instance of  this
		 * class.
		 */
		virtual ~SamplePoint()
		{
		}


		/**
		 * \fn virtual double GetU() const
		 *
		 * \brief  Returns  the  first parameter  coordinate  of  this
		 * point.
		 *
		 * \return The first parameter coordinate of this point.
		 *
		 */
		virtual double GetU() const
		{
			return _u;
		}


		/**
		 * \fn virtual double& GetV()
		 *
		 * \brief  Returns the  second  parameter  coordinate of  this
		 * point.
		 *
		 * \return The second parameter coordinate of this point.
		 *
		 */
		virtual double GetV() const
		{
			return _v;
		}


		/**
		 * \fn Point GetPosition() const
		 *
		 * \brief Return the 3D coordinates of this point.
		 *
		 * \return The 3D coordinates of this point.
		 *
		 */
		Point GetPosition() const
		{
			return _pos;
		}


		/**
		 * \fn SamplePoint& operator=(const SamplePoint& p)
		 *
		 * \brief Makes this point the same as a given point.
		 *
		 * \param p A given point.
		 *
		 */
		SamplePoint& operator=(const SamplePoint& p)
		{
			SetParameterCoordinates(p.GetU(), p.GetV());
			Set3DCoordinates(p.GetPosition());

			return *this;
		}


		/**
		 * \fn bool operator==(const SamplePoint& p) const
		 *
		 * \brief Returns the  Boolean value true if and  only if this
		 * point is the same as a given one.
		 *
		 * \param p A given point.
		 *
		 * \return The Boolean value true if this point is the same as
		 * a given one, and the Boolean value false otherwise.
		 *
		 */
		bool operator==(const SamplePoint& p) const
		{
			return  (GetU() == p.GetU()) &&
				(GetV() == p.GetV()) &&
				(GetPosition() == p.GetPosition());
		}


		/**
		 * \fn bool operator!=(const SamplePoint& p) const
		 *
		 * \brief Returns the Boolean value true if, and only if, this
		 * point is not the same as a given one.
		 *
		 * \param p A given point.
		 *
		 * \return The  Boolean value  true if this  point is  not the
		 * same as a given one, and the Boolean value false otherwise.
		 *
		 */
		bool operator!=(const SamplePoint& p) const
		{
			return !(*this == p);
		}


		/**
		 * \fn void SetParameterCoordinates(double u, double v)
		 *
		 * \brief Assign  values to  the parameter coordinates  of the
		 * point.
		 *
		 * \param u The value to be assigned to the u-coordinate.
		 * \param v The value to be assigned to the v-coordinate.
		 *
		 */
		void SetParameterCoordinates(double u, double v)
		{
			_u = u;
			_v = v;

			return;
		}


		/**
		 * \fn void Set3DCoordinates(const Point& pos)
		 *
		 * \brief Assign values to the 3D coordinates of this point.
		 *
		 * \param pos The 3D coordinates to be assigned to this point.
		 *
		 */
		void Set3DCoordinates(const Point& pos)
		{
			_pos = pos;

			return;
		}

	};

}

/** @} */ //end of group class.
