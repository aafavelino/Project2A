/**
 * \file Predicates.h
 *
 * \brief Definition and implementation of the class Predicates, which
 * represents a set of robust geometric predicates.
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
 // For more details regarding this class, see the paper
 //
 // "Adaptive  Precision  Floating-Point  Arithmetic and  Fast  Robust
 // Geometric  Predicates", by  Jonathan  Shewchuk,  published in  the
 // Discrete & Computational  Geometry journal, 18(3):305–363, October
 // 1997,
 //
 // Jonathan Shewchuk made available his own code for the predicates:
 //
 // https://www.cs.cmu.edu/afs/cs/project/quake/public/www/robust.html
 //
 // ------------------------------------------------------------------

#pragma once


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
	 * \brief   The  namespace   cdt  contains   the  definition   and
	 * implementation of classes to build and manipulate a constrained
	 * Delaunay  triangulation  (CDT)  using an  augmentation  of  the
	 * quad-edge data structure.
	 */

	namespace cdt
	{


		/**
		 * \class Predicates
		 *
		 * \brief This class represents a set of geometric predicates.
		 */
		template <typename real>
		class Predicates
		{
		private:

			real _splitter;       ///< 2^ceiling( p / 2 ) + 1, where p is the precision.
			real _epsilon;        ///< 2^( -p ), where p is the precision. This is the machine epsilon.
			real _resulterrbound; ///< error bound for orientation and incircle test.
			real _ccwerrboundA;   ///< error bound for orientation test.
			real _ccwerrboundB;   ///< error bound for orientation test.
			real _ccwerrboundC;   ///< error bound for orientation test.
			real _iccerrboundA;   ///< error bound for incircle test.
			real _iccerrboundB;   ///< error bound for incircle test.
			real _iccerrboundC;   ///< error bound for incircle test.

			real _bvirt;          ///< used as temporary varible - declared here to speed up code.
			real _avirt;          ///< used as temporary varible - declared here to speed up code.
			real _bround;         ///< used as temporary varible - declared here to speed up code.
			real _around;         ///< used as temporary varible - declared here to speed up code.

			real _abig;           ///< used as temporary varible - declared here to speed up code.
			real _ahi;            ///< used as temporary varible - declared here to speed up code.
			real _alo;            ///< used as temporary varible - declared here to speed up code.
			real _bhi;            ///< used as temporary varible - declared here to speed up code.
			real _blo;            ///< used as temporary varible - declared here to speed up code.
			real _c;              ///< used as temporary varible - declared here to speed up code.
			real _err1;           ///< used as temporary varible - declared here to speed up code.
			real _err2;           ///< used as temporary varible - declared here to speed up code.
			real _err3;           ///< used as temporary varible - declared here to speed up code.
			real _i;              ///< used as temporary varible - declared here to speed up code.
			real _j;              ///< used as temporary varible - declared here to speed up code.
			real _k;              ///< used as temporary varible - declared here to speed up code.

		public:

			// -------------------------------------------------------
			//
			// Type definitions
			//
			// -------------------------------------------------------

			/**
			 * \enum Orientation
			 *
			 * \brief A class for all possible orientation results.
			 *
			 */
			enum class Orientation : std::int8_t 
			{ 
			    Origin,       ///< indicates that a point coincides with the origin vertex of an oriented line segment.
				Destination,  ///< indicates that a point coincides with the destination vertex of an oriented line segment.
				Left,         ///< indicates that a point is on the left side of the supporting line of an oriented line segment. 
				Right,        ///< indicates that a point is on the right side of the supporting line of an oriented line segment. 
				Beyond,       ///< indicates that a point is beyond the destination vertex of an oriented line segment w.r.t its supporting line. 
				Behind,       ///< indicates that a point is behind the origin vertex of an oriented line segment  w.r.t its supporting line. 
				Between       ///< indicates that a point is in between the origin and destination vertices of an oriented line segment w.r.t its supporting line.
			};


			// -------------------------------------------------------
			//
			// Public methods
			//
			// -------------------------------------------------------

			/**
			 * \fn Predicates()
			 *
			 * \brief Creates an instance of this class.
			 *
			 */
			Predicates();


			/**
			 * \fn ~Predicates()
			 *
			 * \brief Releases the memory held by an instance of this class.
			 *
			 */
			~Predicates() {}


			/**
			 * \fn inline real Absolute( real a ) const
			 *
			 * \brief Returns the absolute value of a given number.
			 *
			 * \param a A floating-point number
			 *
			 * \return The absolute value of the given number.
			 *
			 */
			inline 
			real
				Absolute(
					real a
				) 
				const
			{
				return (a >= real(0)) ? a : -a;
			}


			/**
			 * \fn inline real Orient2D(real ax, real ay, real bx, real by, real cx, real cy)
			 *
			 * \brief  Returns a  number  with the  same  sign of  the
			 * signed  area  of  a  triangle defined  by  three  given
			 * vertices,  and  the  value  zero if  the  vertices  are
			 * collinear.  This  method is numerically robust,  and it
			 * is  typically  much faster  than  the  exact orient  2d
			 * predicate.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 *
			 * \return a number with the  same sign of the signed area
			 * of a triangle defined by  three given vertices, and the
			 * value zero if the vertices are collinear.
			 *
			 */
			inline
			real
				Orient2D(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy
				)
			{
				return Orient2DAdapt(ax, ay, bx, by, cx, cy);
			}


			/**
			 * \fn inline bool InCircle(double ax, double ay, double bx, double by, double cx, double cy, double dx, double dy)
			 *
			 * \brief Returns the Boolean value  true if a given point
			 * is in the interior of the circle defined by three other
			 * given points, and false otherwise.
			 *
			 * \param ax First Cartesian coordinate of the first point
			 * defining the circle.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point defining the circle.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point defining the circle.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point defining the circle.
			 * \param cx First Cartesian coordinate of the third point
			 * defining the circle.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point defining the circle.
			 * \param dx First Cartesian coordinate of the point to be
			 * tested.
			 * \param dy  Second Cartesian coordinate of  the point to
			 * be tested.
			 *
			 * \return The Boolean  value true if a given  point is in
			 * the interior of the circle defined by three other given
			 * points, and false otherwise.
			 *
			 */
			inline
				bool 
				InCircle(
					double ax,
					double ay,
					double bx,
					double by,
					double cx,
					double cy,
					double dx,
					double dy
				)
			{
				return InCircleAdapt(ax, ay, bx, by, cx, cy, dx, dy) > 0;
			}


			/**
			 * \fn inline bool Left(real ax, real ay, real bx, real by, real cx, real cy)
			 *
			 * \brief Returns  the Boolean  value true if  three given
			 * points in \f$E^2\f$ define a positively oriented basis,
			 * and false otherwise.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 *
			 * \return The Boolean value true if three given points in
			 * \f$E^2\f$ define  a positively oriented basis,  and the
			 * Boolean value false otherwise.
			 *
			 */
			inline 
			bool
				Left(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy
				)
			{
				return Orient2D(ax, ay, bx, by, cx, cy) > 0;
			}


			/**
			 * \fn inline bool LeftOn(real ax, real ay, real bx, real by, real cx, real cy)
			 *
			 * \brief Returns  the Boolean  value true if  three given
			 * points  in   \f$E^2\f$  are   collinear  or   define  a
			 * positively oriented basis, and false otherwise.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 *
			 * \return The Boolean value true if three given points in
			 * \f$E^2\f$ are collinear or define a positively oriented
			 * basis, and the Boolean value false otherwise.
			 *
			 */
			inline 
			bool
				LeftOn(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy
				)
			{
				return Orient2D(ax, ay, bx, by, cx, cy) >= 0;
			}


			/**
			 * \fn inline bool Collinear(real ax, real ay, real bx, real by, real cx, real cy)
			 *
			 * \brief Returns  the Boolean  value true if  three given
			 * points  in \f$E^2\f$  are  collinear,  and the  Boolean
			 * value false otherwise.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 *
			 * \return The Boolean value true if three given points in
			 * \f$E^2\f$ are  collinear, and  the Boolean  value false
			 * otherwise.
			 */
			inline
			bool
				Collinear(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy
				)
			{
				return Orient2D(ax, ay, bx, by, cx, cy) == 0;
			}


			/**
			 * \fn Orientation Classify(real ax, real ay, real bx, real by, real cx, real cy)
			 *
			 * \brief Returns the orientation  of a point with respect
			 * to a line segment.
			 *
			 * \param  ax First  Cartesian  coordinate  of the  origin
			 * point of the line segment.
			 * \param  ay Second  Cartesian coordinate  of the  origin
			 * point of the line segment.
			 * \param bx First Cartesian coordinate of the destination
			 * point of the line segment.
			 * \param   by   Second   Cartesian  coordinate   of   the
			 * destination point of the line segment.
			 * \param cx First Cartesian coordinate of the point to be
			 * tested.
			 * \param cy  Second Cartesian coordinate of  the point to
			 * be tested.
			 *
			 * \return The  orientation of a  point with respect  to a
			 * line segment.
			 *
			 */
			Orientation
				Classify(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy
				);


		private:

			// -------------------------------------------------------
			//
			// Private methods
			//
			// -------------------------------------------------------


			/**
			 * \fn inline void TwoSumTail(real a, real b, real x, real& y)
			 *
			 * \brief Compute the tail y of the  expansion a + b = x +
			 * y.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param x A floating-point number corresponding to a + b.
			 * \param y A reference to the  tail of an expansion for a
			 * + b.
			 *
			 */
			inline 
			void
				TwoSumTail(
					real a,
					real b,
					real x,
					real& y
				)
			{
				_bvirt = x - a;
				_avirt = x - _bvirt;
				_bround = b - _bvirt;
				_around = a - _avirt;
				y = _around + _bround;
			}


			/**
			 * \fn inline void TwoSum(real a, real b, real& x, real& y)
			 *
			 * \brief Compute an expansion x + y for a + b.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param   x   A    reference   to   the   floating-point
			 * corresponding to a + b.
			 * \param y A reference to the  tail of an expansion for a
			 * + b.
			 *
			 */
			inline
			void 
				TwoSum(
					real a,
					real b,
					real& x,
					real& y
				)
			{
				x = a + b;
				TwoSumTail(a, b, x, y);
			}


			/**
			 * \fn inline void FastTwoSumTail(real a, real b, real x, real& y)
			 *
			 * \brief Compute the tail y of the  expansion a + b = x +
			 * y.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param x  A floating-point number corresponding  to a +
			 * b.
			 * \param y A reference to the  tail of an expansion for a
			 * + b.
			 *
			 */
			inline
			void
				FastTwoSumTail(
					real a,
					real b,
					real x,
					real& y
				)
			{
				_bvirt = x - a;
				y = b - _bvirt;
			}


			/**
			 * \fn inline void FastTwoSum(real a, real b, real& x, real& y)
			 *
			 * \brief Compute an expansion x + y for a + b.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param   x   A    reference   to   the   floating-point
			 * corresponding to a + b.
			 * \param y A reference to the  tail of an expansion for a
			 * + b.
			 *
			 */
			inline
			void
				FastTwoSum(
					real a,
					real b,
					real& x,
					real& y
				)
			{
				x = a + b;
				FastTwoSumTail(a, b, x, y);
			}


			/**
			 * \fn inline void TwoDiffTail(real a, real b, real x, real& y)
			 *
			 * \brief Compute the tail y of the  expansion a - b = x +
			 * y.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param x  A floating-point number corresponding  to a -
			 * b.
			 * \param y A reference to the  tail of an expansion for a
			 * - b.
			 *
			 */
			inline
			void
				TwoDiffTail(
					real a,
					real b,
					real x,
					real& y
				)
			{
				_bvirt = a - x;
				_avirt = x + _bvirt;
				_bround = _bvirt - b;
				_around = a - _avirt;
				y = _around + _bround;
			}


			/**
			 * \fn inline void TwoDiff(real a, real b, real& x, real& y)
			 *
			 * \brief Compute an expansion x + y for a - b.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param   x   A    reference   to   the   floating-point
			 * corresponding to a - b.
			 * \param y A reference to the  tail of an expansion for a
			 * - b.
			 *
			 */
			inline
			void
				TwoDiff(
					real a,
					real b,
					real& x,
					real& y
				)
			{
				x = a - b;
				TwoDiffTail(a, b, x, y);
			}


			/**
			 * \fn inline void Split(real a, real& ah, real& al)
			 *
			 * \brief Compute an expansion ahi + alo for a * ( 2^s + 1
			 * ), where s is the _splitter constant.
			 *
			 * \param a A floating-point number.
			 * \param   ah   A   reference   to   the   floating-point
			 * corresponding to a * ( 2^s + 1 ).
			 * \param   al   A   reference   to   the   floating-point
			 * corresponding to the tail of an expansion ah + al for a
			 * * ( 2^s + 1 ).
			 *
			 */
			inline
			void
				Split(
					real a,
					real& ah,
					real& al
				)
			{
				_c = _splitter * a;
				_abig = _c - a;
				ah = _c - _abig;
				al = a - ah;
			}


			/**
			 * \fn inline void TwoProductTail(real a, real b, real x, real& y)
			 *
			 * \brief Compute an expansion x + y for a * b.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param x A floating-point corresponding  to a * b.
			 * \param y A reference to the  tail of an expansion for a
			 * * b.
			 *
			 */
			inline
			void
				TwoProductTail(
					real a,
					real b,
					real x,
					real& y
				)
			{
				Split(a, _ahi, _alo);
				Split(b, _bhi, _blo);

				_err1 = x - (_ahi * _bhi);
				_err2 = _err1 - (_alo * _bhi);
				_err3 = _err2 - (_ahi * _blo);

				y = (_alo * _blo) - _err3;
			}


			/**
			 * \fn inline void TwoProduct(real a, real b, real& x, real& y)
			 *
			 * \brief Compute an expansion x + y for a * b.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param   x   A    reference   to   the   floating-point
			 * corresponding to a * b.
			 * \param y A reference to the  tail of an expansion for a
			 * * b.
			 *
			 */
			inline
			void
				TwoProduct(
					real a,
					real b,
					real& x,
					real& y
				)
			{
				x = a * b;
				TwoProductTail(a, b, x, y);
			}


			/**
			 * \fn inline void TwoProduct(real a, real b, real bh, real bl, real& x, real& y)
			 *
			 * \brief Compute  an expansion x +  y for a *  b, where b
			 * has already been split.
			 *
			 * \param a A floating-point number.
			 * \param b A floating-point number.
			 * \param bh A floating-point corresponding  to b * (2^s +
			 * 1).
			 * \param bl A floating-point corresponding to the tail of
			 * an expansion for b * (2^s + 1).
			 * \param   x   A    reference   to   the   floating-point
			 * corresponding to a * b.
			 * \param y A reference to the  tail of an expansion for a
			 * * b.
			 *
			 */
			inline
			void
				TwoProduct(
					real a,
					real b,
					real bh,
					real bl,
					real& x,
					real& y
				)
			{
				x = a * b;

				Split(a, _ahi, _alo);

				_err1 = x - (_ahi * bh);
				_err2 = _err1 - (_alo * bh);
				_err3 = _err2 - (_ahi * bl);

				y = (_alo * bl) - _err3;
			}


			/**
			 * \fn inline void SquareTail(real a, real x, real& y)
			 *
			 * \brief Compute the  tail of an expansion x +  y for the
			 * square os a number a.
			 *
			 * \param a A floating-point number.
			 * \param x A floating-point corresponding to a * a.
			 * \param y A reference to the  tail of an expansion for a
			 * * a.
			 *
			 */
			inline
			void
				SquareTail(
					real a,
					real x,
					real& y
				)
			{
				Split(a, _ahi, _alo);

				_err1 = x - (_ahi * _ahi);
				_err3 = _err1 - ((_ahi + _ahi) * _alo);

				y = (_alo * _alo) - _err3;
			}


			/**
			 * \fn inline void Square(real a, real& x, real& y)
			 *
			 * \brief Compute an  expansion x + y for the  square os a
			 * number a.
			 *
			 * \param a A floating-point number.
			 * \param x A floating-point corresponding  to a * a.
			 * \param y A reference to the  tail of an expansion for a
			 * * a.
			 *
			 */
			inline
			void
				Square(
					real a,
					real& x,
					real& y
				)
			{
				x = a * a;
				SquareTail(a, x, y);
			}


			/**
			* \fn inline void TwoOneSum(real a1, real a0, real b, real& x2, real& x1, real& x0)
			*
			* \brief  Compute  an  expansion  x2  + x1  +  x0  of  the
			* expression a1 + ( a0 + b ).
			*
			* \param a1 A floating-point number.
			* \param a0 A floating-point number.
			* \param b A floating-point number.
			* \param x2 A reference to the last term of the expansion.
			* \param  x1  A  reference  to  the  middle  term  of  the
			* expansion.
			* \param  x0  A  reference  to   the  first  term  of  the
			* expansion.
			*
			*/
			inline
			void
				TwoOneSum(
					real a1,
					real a0,
					real b,
					real& x2,
					real& x1,
					real& x0
				)
			{
				TwoSum(a0, b, _i, x0);
				TwoSum(a1, _i, x2, x1);
			}


			/**
			 * \fn inline void TwoTwoSum(real a1, real a0, real b1, real b0, real& x3, real& x2, real& x1, real& x0)
			 *
			 * \brief Compute  an expansion x3 +  x2 + x1 +  x0 of the
			 * expression ( a1 + a0 ) + ( b1 + b0 ).
			 *
			 * \param a1 A floating-point number.
			 * \param a0 A floating-point number.
			 * \param b1 A floating-point number.
			 * \param b0 A floating-point number.
			 * \param  x3  A  reference  to the  fourth  term  of  the
			 * expansion.
			 * \param  x2  A  reference  to  the  third  term  of  the
			 * expansion.
			 * \param  x1  A  reference  to the  second  term  of  the
			 * expansion.
			 * \param  x0  A  reference  to  the  first  term  of  the
			 * expansion.
			 *
			 */
			inline
			void
				TwoTwoSum(
					real a1,
					real a0,
					real b1,
					real b0,
					real& x3,
					real& x2,
					real& x1,
					real& x0
				)
			{
				TwoOneSum(a1, a0, b0, _j, _k, x0);
				TwoOneSum(_j, _k, b1, x3, x2, x1);
			}


			/**
			 * \fn inline void TwoOneDiff(real a1, real a0, real b, real& x2, real& x1, real& x0)
			 *
			 * \brief  Compute  an expansion  x2  +  x1  + x0  of  the
			 * expression a1 + ( a0 - b ).
			 *
			 * \param a1 A floating-point number.
			 * \param a0 A floating-point number.
			 * \param b A floating-point number.
			 * \param  x2  A  reference  to   the  last  term  of  the
			 * expansion.
			 * \param  x1  A  reference  to the  middle  term  of  the
			 * expansion.
			 * \param  x0  A  reference  to  the  first  term  of  the
			 * expansion.
			 *
			 */
			inline
			void
				TwoOneDiff(
					real a1,
					real a0,
					real b,
					real& x2,
					real& x1,
					real& x0
				)
			{
				TwoDiff(a0, b, _i, x0);
				TwoSum(a1, _i, x2, x1);
			}


			/**
			 * \fn inline void TwoTwoDiff(real a1, real a0, real b1, real b0, real& x3, real& x2, real& x1, real& x0)
			 *
			 * \brief Compute  an expansion x3 +  x2 + x1 +  x0 of the
			 * expression ( a1 + a0 ) - ( b1 + b0 ).
			 *
			 * \param a1 A floating-point number.
			 * \param a0 A floating-point number.
			 * \param b1 A floating-point number.
			 * \param b0 A floating-point number.
			 * \param  x3  A  reference  to the  fourth  term  of  the
			 * expansion.
			 * \param  x2  A  reference  to  the  third  term  of  the
			 * expansion.
			 * \param  x1  A  reference  to the  second  term  of  the
			 * expansion.
			 * \param  x0  A  reference  to  the  first  term  of  the
			 * expansion.
			 *
			 */
			inline
			void
				TwoTwoDiff(
					real a1,
					real a0,
					real b1,
					real b0,
					real& x3,
					real& x2,
					real& x1,
					real& x0
				)
			{
				TwoOneDiff(a1, a0, b0, _j, _k, x0);
				TwoOneDiff(_j, _k, b1, x3, x2, x1);
			}


			/**
			 * \fn int FastExpansionSumZeroElim(int elen, real e[], int flen, real f[], real h[])
			 *
			 * \brief Compute  the sum of two  expansions, eliminating
			 * zero components from the output expansion.
			 *
			 * \param elen Number of components of the first expansion.
			 * \param  e An  array  containing the  components of  the
			 * first expansion.
			 * \param  flen   Number  of  components  of   the  second
			 * expansion.
			 * \param  f An  array  containing the  components of  the
			 * second expansion.
			 * \param h An array to store the components of the output
			 * expansion.
			 *
			 * \return  The   number  of  components  of   the  output
			 * expansion.
			 *
			 */
			int 
				FastExpansionSumZeroElim(
					int elen,
					real e[],
					int flen,
					real f[],
					real h[]
				);


			/**
			 * \fn int ScaleExpansionZeroElim(int elen, real e[], real b, real h[])
			 *
			 * \brief Multiply  an expansion by a  scalar, eliminating
			 * zero components from the output expansion.
			 *
			 * \param elen Number of components of the expansion.
			 * \param e An array containing the expansion components.
			 * \param b The scalar to be multiplied by the expansion.
			 * \param h An array to store the components of the output
			 * expansion.
			 *
			 * \return  The   number  of  components  of   the  output
			 * expansion.
			 *
			 */
			int 
				ScaleExpansionZeroElim(
					int elen,
					real e[],
					real b,
					real h[]
				);


			/**
			 * \fn real Estimate(int elen, real e[])
			 *
			 * \brief Compute  a one-word approximation for  the value
			 * of an expansion.
			 *
			 * \param elen Number of components of the expansion.

			 * \param e An array containing the expansion components.
			 *
			 * \return  A one-word  approximation for  the expansion's
			 * value.
			 *
			 */
			real
				Estimate(
					int elen,
					real e[]
				);


			/**
			 * \fn real Orient2DFast(real ax, real ay, real bx, real by, real cx, real cy)
			 *
			 * \brief Computes  an approximation for twice  the signed
			 * area of a triangle defined by three given vertices, and
			 * the  value zero  if  the vertices  are collinear.  This
			 * approximation may have the wrong sign.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 *
			 * \return An approximation for twice the signed area of a
			 * triangle defined by three given vertices, and the value
			 * zero if the vertices are  collinear. The output of this
			 * method may be wrong due to roundoff errors.
			 *
			 */
			real
				Orient2DFast(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy
				);


			/**
			 * \fn real Orient2DExact(real ax, real ay, real bx, real by, real cx, real cy)
			 *
			 * \brief  Returns a  number  with the  same  sign of  the
			 * signed  area  of  a  triangle defined  by  three  given
			 * vertices,  and  the  value  zero if  the  vertices  are
			 * collinear.   This  method  is numerically  robust,  but
			 * slow.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 *
			 * \return a number with the  same sign of the signed area
			 * of a triangle defined by  three given vertices, and the
			 * value zero if the vertices are collinear.
			 *
			 */
			real 
				Orient2DExact(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy
				);


			/**
			 * \fn real Orient2DAdapt(real ax, real ay, real bx, real by, real cx, real cy)
			 *
			 * \brief  Returns a  number  with the  same  sign of  the
			 * signed  area  of  a  triangle defined  by  three  given
			 * vertices,  and  the  value  zero if  the  vertices  are
			 * collinear.  This  method is numerically robust,  and it
			 * is  typically  much faster  than  the  exact orient  2d
			 * predicate.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 *
			 * \return a number with the  same sign of the signed area
			 * of a triangle defined by  three given vertices, and the
			 * value zero if the vertices are collinear.
			 *
			 */
			real 
				Orient2DAdapt(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy
				);


			/**
			 * \fn real Orient2DAdapt(real ax, real ay, real bx, real by, real cx, real cy, real detsum)
			 *
			 * \brief  Computes a  number with  the same  sign of  the
			 * signed  area  of  a  triangle defined  by  three  given
			 * points, and the value zero if the points are collinear.
			 * The  number   is  adaptively  computed  from   a  given
			 * approximation for  the signed area, which  may not have
			 * the same sign of the signed area.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 * \param detsum  An approximation for the  signed area of
			 * the triangle defined by three given points.
			 *
			 * \return A number with the  same sign of the signed area
			 * of a triangle defined by three given points.
			 *
			 */
			real 
				Orient2DAdapt(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy,
					real detsum
				);


			/**
			 * \fn real InCircleFast(real ax, real ay, real bx, real by, real cx, real cy, real dx, real dy)
			 *
			 * \brief  Returns   a  positive   value  if   point  with
			 * coordinates ( dx , dy  ) lies inside the circle passing
			 * through points of coordinates  ( ax , ay ) ,  ( bx , by
			 * ),  and (  cx  , cy  );  a negative  value  if it  lies
			 * outside,   and   zero   if    the   four   points   are
			 * cocircular. The  points a,  b, and c  must be  given in
			 * counterclockwise order, or the  sign of the result will
			 * be reversed. This method is not robust.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 * \param dx First Cartesian coordinate of the point to be
			 * tested.
			 * \param dy  Second Cartesian coordinate of  the third to
			 * be tested.
			 *
			 * \return A positive value if point with coordinates ( dx
			 * , dy ) lies inside the circle passing through points of
			 * coordinates ( ax , ay ) , ( bx , by ), and ( cx , cy );
			 * a negative  value if it  lies outside, and zero  if the
			 * four points belong to the same circumference.
			 *
			 */
			real 
				InCircleFast(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy,
					real dx,
					real dy
				);


			/**
			 * \fn real InCircleExact(real ax, real ay, real bx, real by, real cx, real cy, real dx, real dy)
			 *
			 * \brief  Returns   a  positive   value  if   point  with
			 * coordinates ( dx , dy  ) lies inside the circle passing
			 * through points of coordinates  ( ax , ay ) ,  ( bx , by
			 * ),  and (  cx  , cy  );  a negative  value  if it  lies
			 * outside,   and   zero   if    the   four   points   are
			 * cocircular. The  points a,  b, and c  must be  given in
			 * counterclockwise order, or the  sign of the result will
			 * be reversed.  This method is numerically robust.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 * \param dx First Cartesian coordinate of the point to be
			 * tested.
			 * \param dy  Second Cartesian coordinate of  the third to
			 * be tested.
			 *
			 * \return A positive value if point with coordinates ( dx
			 * , dy ) lies inside the circle passing through points of
			 * coordinates ( ax , ay ) , ( bx , by ), and ( cx , cy );
			 * a negative  value if it  lies outside, and zero  if the
			 * four points belong to the same circumference.
			 *
			 */
			real 
				InCircleExact(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy,
					real dx,
					real dy
				);


			/**
			 * \fn real InCircleAdapt(real ax, real ay, real bx, real by, real cx, real cy, real dx, real dy, real permanent)
			 *
			 * \brief  Returns   a  positive   value  if   point  with
			 * coordinates ( dx , dy  ) lies inside the circle passing
			 * through points of coordinates  ( ax , ay ) ,  ( bx , by
			 * ),  and (  cx  , cy  );  a negative  value  if it  lies
			 * outside,   and   zero   if    the   four   points   are
			 * cocircular. The  points a,  b, and c  must be  given in
			 * counterclockwise order, or the  sign of the result will
			 * be reversed.  This method is numerically robust and the
			 * result   is   adaptively    computed   from   a   given
			 * approximation.
			 *
			 * \param  ax  First  Cartesian coordinate  of  the  first
			 * point.
			 * \param  ay Second  Cartesian  coordinate  of the  first
			 * point.
			 * \param  bx First  Cartesian  coordinate  of the  second
			 * point.
			 * \param  by Second  Cartesian coordinate  of the  second
			 * point.
			 * \param  cx  First  Cartesian coordinate  of  the  third
			 * point.
			 * \param  cy Second  Cartesian  coordinate  of the  third
			 * point.
			 * \param dx First Cartesian coordinate of the point to be
			 * tested.
			 * \param dy  Second Cartesian coordinate of  the third to
			 * be tested.
			 * \param permanent An approximation to the exact result.
			 *
			 * \return A positive value if point with coordinates ( dx
			 * , dy ) lies inside the circle passing through points of
			 * coordinates ( ax , ay ) , ( bx , by ), and ( cx , cy );
			 * a negative  value if it  lies outside, and zero  if the
			 * four points belong to the same circumference.
			 *
			 */
			real 
				InCircleAdapt(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy,
					real dx,
					real dy,
					real permanent
				);


			/**
			* \fn real InCircleAdapt(real ax, real ay, real bx, real by, real cx, real cy, real dx, real dy)
			*
			* \brief   Returns  a   positive  value   if  point   with
			* coordinates ( dx  , dy ) lies inside  the circle passing
			* through points of coordinates ( ax , ay ) , ( bx , by ),
			* and (  cx , cy );  a negative value if  it lies outside,
			* and zero if  the four points are  cocircular. The points
			* a, b, and c must  be given in counterclockwise order, or
			* the sign of the result will be reversed.  This method is
			* numerically robust.
			*
			* \param ax First Cartesian coordinate of the first point.
			* \param  ay  Second  Cartesian coordinate  of  the  first
			* point.
			* \param  bx  First  Cartesian coordinate  of  the  second
			* point.
			* \param  by Second  Cartesian  coordinate  of the  second
			* point.
			* \param cx First Cartesian coordinate of the third point.
			* \param  cy  Second  Cartesian coordinate  of  the  third
			* point.
			* \param dx First Cartesian coordinate  of the point to be
			* tested.
			* \param dy Second Cartesian coordinate of the third to be
			* tested.
			*
			* \return A positive value if  point with coordinates ( dx
			* , dy ) lies inside  the circle passing through points of
			* coordinates ( ax , ay ) , ( bx  , by ), and ( cx , cy );
			* a negative  value if  it lies outside,  and zero  if the
			* four points belong to the same circumference.
			*
			*/
			real 
				InCircleAdapt(
					real ax,
					real ay,
					real bx,
					real by,
					real cx,
					real cy,
					real dx,
					real dy
				);

		};


		// -----------------------------------------------------------
		//
		// Implementation of public methods
		//
		// -----------------------------------------------------------


		/// Default constructor.
		template <typename real>
		Predicates<real>::Predicates()
		{
			real half = (real) 0.5;

			_epsilon = (real) 1.0;
			_splitter = (real) 1.0;

			bool every_other = true;


			// Repeatedly divide _epsilon by two until it is too small
			// to  add  to one  without  causing  roundoff. This  will
			// happen when _epsilon is equal to 2^(-p), where p is the
			// precision of the number type real.
			//
			// Here, we assume that exact rounding is used.

			do
			{
				// Divide _epsilon by two
				_epsilon *= half;

				// Multiply _splitter by two  every other iteration of
				// this loop.
				if (every_other)
				{
					_splitter *= (real) 2.0;
				}
				every_other = !every_other;

				// Add _epsilon to one. If  the result is one, then we
				// are done.
			} while (((real) 1.0 + _epsilon) != (real) 1.0);

			// Add _splitter to one, so  that _splitter = 2^ceiling( p
			// / 2 ) + 1.
			_splitter += (real) 1.0;

			// Compute the  error bounds for orientation  and incircle
			// tests.
			_resulterrbound = (3.0 + 8.0 * _epsilon) * _epsilon;
			_ccwerrboundA = (3.0 + 16.0 * _epsilon) * _epsilon;
			_ccwerrboundB = (2.0 + 12.0 * _epsilon) * _epsilon;
			_ccwerrboundC = (9.0 + 64.0 * _epsilon) * _epsilon * _epsilon;
			_iccerrboundA = (10.0 + 96.0 * _epsilon) * _epsilon;
			_iccerrboundB = (4.0 + 48.0 * _epsilon) * _epsilon;
			_iccerrboundC = (44.0 + 576.0 * _epsilon) * _epsilon * _epsilon;

			return;
		}


		/**
		 * \fn template <typename real> typename Predicates<real>::Orientation Predicates<real>::Classify(real ax, real ay, real bx, real by, real cx, real cy) const
		 *
		 * \brief Returns the orientation of a point with respect to a
		 * line segment.
		 *
		 * \param ax First Cartesian coordinate of the origin point of
		 * the line segment.
		 * \param ay  Second Cartesian coordinate of  the origin point
		 * of the line segment.
		 * \param  bx First  Cartesian coordinate  of the  destination
		 * point of the line segment.
		 * \param by  Second Cartesian  coordinate of  the destination
		 * point of the line segment.
		 * \param cx  First Cartesian  coordinate of  the point  to be
		 * tested.
		 * \param cy  Second Cartesian coordinate  of the point  to be
		 * tested.
		 *
		 * \return The orientation  of a point with respect  to a line
		 * segment.
		 *
		 */
		template <typename real>
		typename Predicates<real>::Orientation
			Predicates<real>::Classify(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy
			)
		{
			real res = Orient2D(
								ax,
								ay,
								bx,
								by,
								cx,
								cy
							   );

			if (res > 0)
			{
				return Orientation::Left;
			}
			else if (res < 0)
			{
				return Orientation::Right;
			}

			if ((ax == cx) && (ay == cy))
			{
				return Orientation::Origin;
			}

			if ((cx == bx) && (cy == by))
			{
				return Orientation::Destination;
			}

			if (ax < bx)
			{
				if ((cx > ax) && (cx < bx))
				{
					return Orientation::Between;
				}
				else if (cx < ax)
				{
					return Orientation::Behind;
				}
				return Orientation::Beyond;
			}

			if (ax > bx)
			{
				if ((cx > bx) && (cx < ax))
				{
					return Orientation::Between;
				}
				else if (cx > ax)
				{
					return Orientation::Behind;
				}
				return Orientation::Beyond;
			}

			if (ay < by)
			{
				if ((cy > ay) && (cy < by))
				{
					return Orientation::Between;
				}
				else if (cy < ay) {
					return Orientation::Behind;
				}
				return Orientation::Beyond;
			}

			if ((cy < ay) && (cy > by))
			{
				return Orientation::Between;
			}
			else if (cy > ay)
			{
				return Orientation::Behind;
			}

			return Orientation::Beyond;
		}


		// -----------------------------------------------------------
		//
		// Implementation of private methods
		//
		// -----------------------------------------------------------

		/**
		 * \fn template <typename real> int Predicates<real>::FastExpansionSumZeroElim(int elen, real e[], int flen, real f[], real h[])
		 *
		 * \brief Compute the sum  of two expansions, eliminating zero
		 * components from the output expansion.
		 *
		 * \param elen Number of components of the first expansion.
		 * \param e  An array containing  the components of  the first
		 * expansion.
		 * \param flen Number of components of the second expansion.
		 * \param f An  array containing the components  of the second
		 * expansion.
		 * \param h  An array  to store the  components of  the output
		 * expansion.
		 *
		 * \return The number of components of the output expansion.
		 *
		 */
		template <typename real>
		int
			Predicates<real>::FastExpansionSumZeroElim(
				int elen,
				real e[],
				int flen,
				real f[],
				real h[]
			)
		{
			real Q;
			real enow = e[0];
			real fnow = f[0];
			int eindex = 0;
			int findex = 0;
			if ((fnow > enow) == (fnow > -enow))
			{
				Q = enow;
				enow = e[++eindex];
			}
			else
			{
				Q = fnow;
				fnow = f[++findex];
			}

			real Qnew;
			real hh;
			int hindex = 0;
			if ((eindex < elen) && (findex < flen))
			{
				if ((fnow > enow) == (fnow > -enow))
				{
					FastTwoSum(enow, Q, Qnew, hh);
					enow = e[++eindex];
				}
				else
				{
					FastTwoSum(fnow, Q, Qnew, hh);
					fnow = f[++findex];
				}
				Q = Qnew;
				if (hh != real(0))
				{
					h[hindex++] = hh;
				}
				while ((eindex < elen) && (findex < flen))
				{
					if ((fnow > enow) == (fnow > -enow))
					{
						TwoSum(Q, enow, Qnew, hh);
						enow = e[++eindex];
					}
					else
					{
						TwoSum(Q, fnow, Qnew, hh);
						fnow = f[++findex];
					}
					Q = Qnew;
					if (hh != real(0))
					{
						h[hindex++] = hh;
					}
				}
			}

			while (eindex < elen)
			{
				TwoSum(Q, enow, Qnew, hh);
				enow = e[++eindex];
				Q = Qnew;
				if (hh != real(0))
				{
					h[hindex++] = hh;
				}
			}

			while (findex < flen)
			{
				TwoSum(Q, fnow, Qnew, hh);
				fnow = f[++findex];
				Q = Qnew;
				if (hh != real(0))
				{
					h[hindex++] = hh;
				}
			}

			if ((Q != real(0)) || (hindex == 0))
			{
				h[hindex++] = Q;
			}

			return hindex;
		}


		/**
		 * \fn template <typename real> int Predicates<real>::ScaleExpansionZeroElim(int elen, real e[], real b, real h[])
		 *
		 * \brief Multiply an expansion  by a scalar, eliminating zero
		 * components from the output expansion.
		 *
		 * \param elen Number of components of the expansion.
		 * \param e An array containing the expansion components.
		 * \param b The scalar to be multiplied by the expansion.
		 * \param h  An array  to store the  components of  the output
		 * expansion.
		 *
		 * \return The number of components of the output expansion.
		 *
		 */
		template <typename real>
		int
			Predicates<real>::ScaleExpansionZeroElim(
				int elen,
				real e[],
				real b,
				real h[]
			)
		{
			Split(b, _bhi, _blo);

			real Q;
			real hh;
			TwoProduct(e[0], b, _bhi, _blo, Q, hh);

			int hindex = 0;
			if (hh != real(0)) {
				h[hindex++] = hh;
			}

			real enow;
			real product1;
			real product0;
			real sum;
			for (int eindex = 1; eindex < elen; eindex++) {
				enow = e[eindex];
				TwoProduct(enow, b, _bhi, _blo, product1, product0);
				TwoSum(Q, product0, sum, hh);
				if (hh != real(0)) {
					h[hindex++] = hh;
				}

				FastTwoSum(product1, sum, Q, hh);
				if (hh != real(0)) {
					h[hindex++] = hh;
				}
			}

			if ((Q != real(0)) || (hindex == 0)) {
				h[hindex++] = Q;
			}

			return hindex;
		}


		/**
		 * \fn template <typename real> real Predicates<real>::Estimate(int elen, real e[])
		 *
		 * \brief Compute a one-word approximation for the value of an
		 * expansion.
		 *
		 * \param elen Number of components of the expansion.
		 * \param e An  array  containing  the  expansion components.
		 *
		 * \return A one-word approximation for the expansion's value.
		 *
		 */
		template <typename real>
		real
			Predicates<real>::Estimate(
				int elen,
				real e[]
			)
		{
			real Q = e[0];
			for (int eindex = 1; eindex < elen; eindex++)
			{
				Q += e[eindex];
			}

			return Q;
		}


		/**
		 * \fn template <typename real> real Predicates<real>::Orient2DFast(real ax, real ay, real bx, real by, real cx, real cy)
		 *
		 * \brief Computes an approximation  for twice the signed area
		 * of  a triangle  defined by  three given  vertices, and  the
		 * value   zero   if   the  vertices   are   collinear.   This
		 * approximation may have the wrong sign.
		 *
		 * \param ax First Cartesian coordinate of the first point.
		 * \param ay Second Cartesian coordinate of the first point.
		 * \param bx First Cartesian coordinate of the second point.
		 * \param by Second Cartesian coordinate of the second point.
		 * \param cx First Cartesian coordinate of the third point.
		 * \param cy Second Cartesian coordinate of the third point.
		 *
		 * \return An  approximation for  twice the  signed area  of a
		 * triangle  defined by  three given  vertices, and  the value
		 * zero  if the  vertices are  collinear. The  output of  this
		 * method may be wrong due to roundoff errors.
		 *
		 */
		template <typename real>
		real
			Predicates<real>::Orient2DFast(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy
			)
		{
			real acx, bcx, acy, bcy;

			acx = ax - cx;
			bcx = bx - cx;
			acy = ay - cy;
			bcy = by - cy;

			return acx * bcy - acy * bcx;
		}


		/**
		 * \fn template <typename real> real Predicates<real>::Orient2DExact(real ax, real ay, real bx, real by, real cx, real cy)
		 *
		 * \brief Returns  a number with  the same sign of  the signed
		 * area of a triangle defined by three given vertices, and the
		 * value zero if  the vertices are collinear.   This method is
		 * numerically robust, but slow.
		 *
		 * \param ax First Cartesian coordinate of the first point.
		 * \param ay Second Cartesian coordinate of the first point.
		 * \param bx First Cartesian coordinate of the second point.
		 * \param by Second Cartesian coordinate of the second point.
		 * \param cx First Cartesian coordinate of the third point.
		 * \param cy Second Cartesian coordinate of the third point.
		 *
		 * \return a number with the same sign of the signed area of a
		 * triangle  defined by  three given  vertices, and  the value
		 * zero if the vertices are collinear.
		 *
		 */
		template <typename real>
		real
			Predicates<real>::Orient2DExact(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy
			)
		{
		    //
			// We want to compute
			//
			// ( ax * by ) + ( bx * cy ) + ( ay * cx ) - ( cx * by ) - ( ax * cy ) - ( bx * ay )
			//
			// which can be rewritten as
			//
			// ( ax * by - ax * cy ) + ( bx * cy - bx * ay ) + ( cx * ay - cx * by ).
			//
			// To do that exactly, we compute an expansion for each of
			// the three terms of the the  above sum, and then sum the
			// three expansions  using the fast expansion  sum method.
			// Finally, we return the  expansion term with the largest
			// magnitude, which is guaranteed to have the same sign of
			// the exact value of the  expansion (in addition to being
			// a good approximation to the exact value).

			// Computes the expansion axby1 + axby0 = ax * by.
			real axby1;
			real axby0;
			TwoProduct(ax, by, axby1, axby0);

			// Computes the expansion axcy1 + axcy0 = ax * cy.
			real axcy1;
			real axcy0;
			TwoProduct(ax, cy, axcy1, axcy0);

		    // Computes an expansion for ( axby1 + axby0 ) - ( axcy1 +
		    // axcy0 ).
			real aterms[4];
			TwoTwoDiff(
				axby1,
				axby0,
				axcy1,
				axcy0,
				aterms[3],
				aterms[2],
				aterms[1],
				aterms[0]
			);

			// Computes the expansion bxcy1 + bxcy0 = bx * cy.
			real bxcy1;
			real bxcy0;
			TwoProduct(bx, cy, bxcy1, bxcy0);

			// Computes the expansion bxay1 + bxay0 = bx * ay.
			real bxay1;
			real bxay0;
			TwoProduct(bx, ay, bxay1, bxay0);

			//
			real bterms[4];
			TwoTwoDiff(
				bxcy1,
				bxcy0,
				bxay1,
				bxay0,
				bterms[3],
				bterms[2],
				bterms[1],
				bterms[0]
			);

			// Computes the expansion cxay1 + cxay0 = cx * ay.
			real cxay1;
			real cxay0;
			TwoProduct(cx, ay, cxay1, cxay0);

			// Computes the expansion cxby1 + cxby0 = cx * by.
			real cxby1;
			real cxby0;
			TwoProduct(cx, by, cxby1, cxby0);

			// Computes an expansion for ( cxay1 + cxay0 ) - ( cxby1 +
			// cxby0 ).
			real cterms[4];
			TwoTwoDiff(
				cxay1,
				cxay0,
				cxby1,
				cxby0,
				cterms[3],
				cterms[2],
				cterms[1],
				cterms[0]
			);

			// Sum the expansions aterms and bterms.
			real v[8];
			int vlength = FastExpansionSumZeroElim(
				4,
				aterms,
				4,
				bterms,
				v
			);

			// Sum the expansions v and cterms.
			real w[12];
			int wlength = FastExpansionSumZeroElim(
				vlength,
				v,
				4,
				cterms,
				w
			);

			// Return the largest magnitude term of the resulting expansion.
			return w[wlength - 1];
		}


		/**
		 * \fn template <typename real> real Predicates<real>::Orient2DAdapt(real ax, real ay, real bx, real by, real cx, real cy)
		 *
		 * \brief Returns  a number with  the same sign of  the signed
		 * area of a triangle defined by three given vertices, and the
		 * value zero if  the vertices are collinear.   This method is
		 * numerically robust,  and it  is typically much  faster than
		 * the exact orient 2d predicate.
		 *
		 * \param ax First Cartesian coordinate of the first point.
		 * \param ay Second Cartesian coordinate of the first point.
		 * \param bx First Cartesian coordinate of the second point.
		 * \param by Second Cartesian coordinate of the second point.
		 * \param cx First Cartesian coordinate of the third point.
		 * \param cy Second Cartesian coordinate of the third point.
		 *
		 * \return a number with the same sign of the signed area of a
		 * triangle  defined by  three given  vertices, and  the value
		 * zero if the vertices are collinear.
		 *
		 */
		template <typename real>
		real
			Predicates<real>::Orient2DAdapt(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy
			)
		{
			real detlft = (ax - cx) * (by - cy);
			real detrgt = (ay - cy) * (bx - cx);

			real detval = detlft - detrgt;

			real detsum = real(0);

			if (detlft > real(0))
			{
				if (detrgt <= real(0))
				{
					return detval;
				}
				else {
					detsum = detlft + detrgt;
				}
			}
			else if (detlft < real(0))
			{
				if (detrgt >= real(0))
				{
					return detval;
				}
				else
				{
					detsum = -detlft - detrgt;
				}
			}
			else {
				return detval;
			}

			real errbound = _ccwerrboundA * detsum;
			if ((detval >= errbound) || (-detval >= errbound))
			{
				return detval;
			}

			return Orient2DAdapt(
				ax,
				ay,
				bx,
				by,
				cx,
				cy,
				detsum
			);
		}


		/**
		 * \fn template <typename real> real Predicates<real>::Orient2DAdapt(real ax, real ay, real bx, real by, real cx, real cy, real detsum)
		 *
		 * \brief Computes a  number with the same sign  of the signed
		 * area of a  triangle defined by three given  points, and the
		 * value  zero if  the points  are collinear.   The number  is
		 * adaptively  computed from  a  given  approximation for  the
		 * signed area, which may not have the same sign of the signed
		 * area.
		 *
		 * \param ax First Cartesian coordinate of the first point.
		 * \param ay Second Cartesian coordinate of the first point.
		 * \param bx First Cartesian coordinate of the second point.
		 * \param by Second Cartesian coordinate of the second point.
		 * \param cx First Cartesian coordinate of the third point.
		 * \param cy Second Cartesian coordinate of the third point.
		 * \param detsum An approximation for  the signed area of the triangle
		 * defined by three given points.
		 *
		 * \return A number with the same sign of the signed area of a
		 * triangle defined by three given points.
		 *
		 */
		template <typename real>
		real
			Predicates<real>::Orient2DAdapt(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy,
				real detsum
			)
		{
			real acx = ax - cx;
			real bcx = bx - cx;
			real acy = ay - cy;
			real bcy = by - cy;

			real detlft, detlfttail;
			TwoProduct(acx, bcy, detlft, detlfttail);

			real detrgt, detrgttail;
			TwoProduct(acy, bcx, detrgt, detrgttail);

			real B[4];
			TwoTwoDiff(
				detlft,
				detlfttail,
				detrgt,
				detrgttail,
				B[3],
				B[2],
				B[1],
				B[0]
			);

			real det = Estimate(4, B);

			real errbound = _ccwerrboundB * detsum;
			if ((det >= errbound) || (-det >= errbound)) {
				return det;
			}

			real acxtail;
			TwoDiffTail(ax, cx, acx, acxtail);

			real bcxtail;
			TwoDiffTail(bx, cx, bcx, bcxtail);

			real acytail;
			TwoDiffTail(ay, cy, acy, acytail);

			real bcytail;
			TwoDiffTail(by, cy, bcy, bcytail);

			if (
				(acxtail == real(0)) && (acytail == real(0)) &&
				(bcxtail == real(0)) && (bcytail == real(0))
				)
			{
				return det;
			}

			errbound = (_ccwerrboundC * detsum)
				+ (_resulterrbound * Absolute(det));

			det += ((acx * bcytail) + (bcy * acxtail))
				- ((acy * bcxtail) + (bcx * acytail));

			if ((det >= errbound) || (-det >= errbound))
			{
				return det;
			}

			real s1, s0;
			TwoProduct(acxtail, bcy, s1, s0);

			real t1, t0;
			TwoProduct(acytail, bcx, t1, t0);

			real u[4];
			TwoTwoDiff(
				s1,
				s0,
				t1,
				t0,
				u[3],
				u[2],
				u[1],
				u[0]
			);

			real C1[8];
			int C1length = FastExpansionSumZeroElim(
				4,
				B,
				4,
				u,
				C1
			);

			TwoProduct(acx, bcytail, s1, s0);
			TwoProduct(acy, bcxtail, t1, t0);
			TwoTwoDiff(
				s1,
				s0,
				t1,
				t0,
				u[3],
				u[2],
				u[1],
				u[0]
			);

			real C2[12];
			int C2length = FastExpansionSumZeroElim(
				C1length,
				C1,
				4,
				u,
				C2
			);

			TwoProduct(acxtail, bcytail, s1, s0);
			TwoProduct(acytail, bcxtail, t1, t0);
			TwoTwoDiff(
				s1,
				s0,
				t1,
				t0,
				u[3],
				u[2],
				u[1],
				u[0]
			);

			real D[16];
			int Dlength = FastExpansionSumZeroElim(
				C2length,
				C2,
				4,
				u,
				D
			);

			return D[Dlength - 1];
		}


		/**
		 * \fn template <typename real> real Predicates<real>::InCircleFast(real ax, real ay, real bx, real by, real cx, real cy, real dx, real dy)
		 *
		 * \brief Returns a positive value if point with coordinates (
		 * dx , dy ) lies inside  the circle passing through points of
		 * coordinates ( ax , ay  ) , ( bx , by ), and (  cx , cy ); a
		 * negative value  if it  lies outside, and  zero if  the four
		 * points are cocircular. The points a, b, and c must be given
		 * in counterclockwise order,  or the sign of  the result will
		 * be reversed. This method is not robust.
		 *
		 * \param ax First Cartesian coordinate of the first point.
		 * \param ay Second Cartesian coordinate of the first point.
		 * \param bx First Cartesian coordinate of the second point.
		 * \param by Second Cartesian coordinate of the second point.
		 * \param cx First Cartesian coordinate of the third point.
		 * \param cy Second Cartesian coordinate of the third point.
		 * \param dx  First Cartesian  coordinate of  the point  to be
		 * tested.
		 * \param dy  Second Cartesian coordinate  of the third  to be
		 * tested.
		 *
		 * \return A positive  value if point with coordinates  ( dx ,
		 * dy  ) lies  inside  the circle  passing  through points  of
		 * coordinates ( ax , ay  ) , ( bx , by ), and (  cx , cy ); a
		 * negative value  if it  lies outside, and  zero if  the four
		 * points belong to the same circumference.
		 *
		 */
		template <typename real>
		real
			Predicates<real>::InCircleFast(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy,
				real dx,
				real dy
			)
		{
			real adx = ax - dx;
			real ady = ay - dy;
			real bdx = bx - dx;
			real bdy = by - dy;
			real cdx = cx - dx;
			real cdy = cy - dy;

			real abdet = (adx * bdy) - (bdx * ady);
			real bcdet = (bdx * cdy) - (cdx * bdy);
			real cadet = (cdx * ady) - (adx * cdy);

			real alift = (adx * adx) + (ady * ady);
			real blift = (bdx * bdx) + (bdy * bdy);
			real clift = (cdx * cdx) + (cdy * cdy);

			return (alift * bcdet) + (blift * cadet) + (clift * abdet);
		}


		/**
		 * \fn template <typename real> real Predicates<real>::InCircleExact(real ax, real ay, real bx, real by, real cx, real cy, real dx, real dy)
		 *
		 * \brief Returns a positive value if point with coordinates (
		 * dx , dy ) lies inside  the circle passing through points of
		 * coordinates ( ax , ay  ) , ( bx , by ), and (  cx , cy ); a
		 * negative value  if it  lies outside, and  zero if  the four
		 * points are cocircular. The points a, b, and c must be given
		 * in counterclockwise order,  or the sign of  the result will
		 * be reversed.  This method is numerically robust.
		 *
		 * \param ax First Cartesian coordinate of the first point.
		 * \param ay Second Cartesian coordinate of the first point.
		 * \param bx First Cartesian coordinate of the second point.
		 * \param by Second Cartesian coordinate of the second point.
		 * \param cx First Cartesian coordinate of the third point.
		 * \param cy Second Cartesian coordinate of the third point.
		 * \param dx  First Cartesian  coordinate of  the point  to be
		 * tested.
		 * \param dy  Second Cartesian coordinate  of the third  to be
		 * tested.
		 *
		 * \return A positive  value if point with coordinates  ( dx ,
		 * dy  ) lies  inside  the circle  passing  through points  of
		 * coordinates ( ax , ay  ) , ( bx , by ), and (  cx , cy ); a
		 * negative value  if it  lies outside, and  zero if  the four
		 * points belong to the same circumference.
		 *
		 */
		template <typename real>
		real
			Predicates<real>::InCircleExact(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy,
				real dx,
				real dy
			)
		{
			// The idea  behind the  incircle test  is to  compute the
			// exact value of
			//
			// | ax ay ( ax^2 + ay^2 ) 1 |
			// | bx by ( bx^2 + by^2 ) 1 |
			// | cx ay ( cx^2 + cy^2 ) 1 |
			// | dx dy ( cx^2 + cy^2 ) 1 |
			//
			// The value  of this  determinat is  positive if  point d
			// lies inside the circle passing through points a, b, and
			// c; it is negative if it lies outside, and it is zero if
			// the four points are cocircular.  The exact value of the
			// determinant is  computed by the Laplace  method using a
			// dynamic programming based computation strategy.

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | ax ay |
			// |       | = ab[3] + ab[2] + ab[1] + ab[0]
			// | bx by |
			//
			// -------------------------------------------------------

		    // Computes the expansion axby1 +  axby0 of the product ax
		    // * by.
			real axby1;
			real axby0;
			TwoProduct(ax, by, axby1, axby0);

			// Computes the expansion bxay1 +  bxay0 of the product bx
			// * ay.
			real bxay1;
			real bxay0;
			TwoProduct(bx, ay, bxay1, bxay0);

			// Computes the expansion  ab of the expression  ( axby1 +
			// axby0 ) - ( bxay1 + bxay0 )
			real ab[4];
			TwoTwoDiff(
				axby1,
				axby0,
				bxay1,
				bxay0,
				ab[3],
				ab[2],
				ab[1],
				ab[0]
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of a 2x2 determinant:
			//
			// | bx by |
			// |       | = bc[3] + bc[2] + bc[1] + bc[0]
			// | cx cy |
			//
			// -------------------------------------------------------

			// Computes the expansion bxcy1 +  bxcy0 of the product bx
			// * cy.
			real bxcy1;
			real bxcy0;
			TwoProduct(bx, cy, bxcy1, bxcy0);

			// Computes the expansion cxby1 +  cxby0 of the product cx
			// * by.
			real cxby1;
			real cxby0;
			TwoProduct(cx, by, cxby1, cxby0);

			// Computes the expansion  bc of the expression  ( bxcy1 +
			// bxcy0 ) - ( cxby1 + cxby0 )
			real bc[4];
			TwoTwoDiff(
				bxcy1,
				bxcy0,
				cxby1,
				cxby0,
				bc[3],
				bc[2],
				bc[1],
				bc[0]
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | cx cy |
			// |       | = bc[3] + bc[2] + bc[1] + bc[0]
			// | dx dy |
			//
			// -------------------------------------------------------

			// Computes the expansion cxdy1 +  cxdy0 of the product cx
			// * dy.
			real cxdy1;
			real cxdy0;
			TwoProduct(cx, dy, cxdy1, cxdy0);

			// Computes the expansion dxcy1 +  dxcy0 of the product dx
			// * cy.
			real dxcy1;
			real dxcy0;
			TwoProduct(dx, cy, dxcy1, dxcy0);

			// Computes the expansion  cd of the expression  ( cxdy1 +
			// cxdy0 ) - ( dxcy1 + dxcy0 )
			real cd[4];
			TwoTwoDiff(
				cxdy1,
				cxdy0,
				dxcy1,
				dxcy0,
				cd[3],
				cd[2],
				cd[1],
				cd[0]
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | dx dy |
			// |       | = da[3] + da[2] + da[1] + da[0]
			// | ax ay |
			//
			// -------------------------------------------------------

			// Computes the expansion dxay1 +  dxay0 of the product dx
			// * ay.
			real dxay1;
			real dxay0;
			TwoProduct(dx, ay, dxay1, dxay0);

			// Computes the expansion axdy1 +  axdy0 of the product ax
			// * dy.
			real axdy1;
			real axdy0;
			TwoProduct(ax, dy, axdy1, axdy0);

			// Computes the expansion  da of the expression  ( dxay1 +
			// dxay0 ) - ( axdy1 + axdy0 )
			real da[4];
			TwoTwoDiff(
				dxay1,
				dxay0,
				axdy1,
				axdy0,
				da[3],
				da[2],
				da[1],
				da[0]
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | ax ay |
			// |       | = ac[3] + ac[2] + ac[1] + ac[0]
			// | cx cy |
			//
			// -------------------------------------------------------

			// Computes the expansion axcy1 +  axcy0 of the product ax
			// * cy.
			real axcy1;
			real axcy0;
			TwoProduct(ax, cy, axcy1, axcy0);

			// Computes the expansion cxay1 +  cxay0 of the product cx
			// * ay.
			real cxay1;
			real cxay0;
			TwoProduct(cx, ay, cxay1, cxay0);

			// Computes the expansion  ac of the expression  ( axcy1 +
			// axcy0 ) - ( cxay1 + cxay0 )
			real ac[4];
			TwoTwoDiff(
				axcy1,
				axcy0,
				cxay1,
				cxay0,
				ac[3],
				ac[2],
				ac[1],
				ac[0]
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | bx by |
			// |       | = bd[3] + bd[2] + bd[1] + bd[0]
			// | dx dy |
			//
			// -------------------------------------------------------

			// Computes the expansion bxdy1 +  bxdy0 of the product bx
			// * dy.
			real bxdy1;
			real bxdy0;
			TwoProduct(bx, dy, bxdy1, bxdy0);

			// Computes the expansion dxby1 +  dxby0 of the product dx
			// * by.
			real dxby1;
			real dxby0;
			TwoProduct(dx, by, dxby1, dxby0);

			// Computes the expansion  ac of the expression  ( dxby1 +
			// dxby0 ) - ( dxby1 + dxby0 )
			real bd[4];
			TwoTwoDiff(
				bxdy1,
				bxdy0,
				dxby1,
				dxby0,
				bd[3],
				bd[2],
				bd[1],
				bd[0]
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | ax ay 1 |
			// | cx cy 1 | = sum_i=1^12 ( cda[i] )
			// | dx dy 1 |
			//
			// -------------------------------------------------------

			// Computes the sum of expansions cd and da.
			real temp8[8];
			int templen = FastExpansionSumZeroElim(
				4,
				cd,
				4,
				da,
				temp8
			);

			// Computes the sum of expansions ( cd + da ) and ac.
			real cda[12];
			int cdalen = FastExpansionSumZeroElim(
				templen,
				temp8,
				4,
				ac,
				cda
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | ax ay 1 |
			// | bx by 1 | = sum_i=1^12 ( dab[i] )
			// | dx dy 1 |
			//
			// -------------------------------------------------------

			// Computes the sum of expansions da and ab.
			templen = FastExpansionSumZeroElim(
				4,
				da,
				4,
				ab,
				temp8
			);

			// Computes the sum of expansions ( da + ab ) and bd.
			real dab[12];
			int dablen = FastExpansionSumZeroElim(
				templen,
				temp8,
				4,
				bd,
				dab
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | ax ay 1 |
			// | bx by 1 | = sum_i=1^12 ( abc[i] )
			// | cx cy 1 |
			//
			// -------------------------------------------------------

			for (int i = 0; i < 4; i++) {
				bd[i] = -bd[i];
				ac[i] = -ac[i];
			}

			// Computes the sum of expansions ab and bc.
			templen = FastExpansionSumZeroElim(
				4,
				ab,
				4,
				bc,
				temp8
			);

			// Computes the sum of expansions ( ab + bc ) and -ac.
			real abc[12];
			int abclen = FastExpansionSumZeroElim(
				templen,
				temp8,
				4,
				ac,
				abc
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | bx by 1 |
			// | cx cy 1 | = sum_i=1^12 ( abc[i] )
			// | dx dy 1 |
			//
			// -------------------------------------------------------

			// Computes the sum of expansions bc and cd.
			templen = FastExpansionSumZeroElim(
				4,
				bc,
				4,
				cd,
				temp8
			);

			// Computes the sum of expansions ( bc + cd ) and -bd.
			real bcd[12];
			int bcdlen = FastExpansionSumZeroElim(
				templen,
				temp8,
				4,
				bd,
				bcd
			);

			// -------------------------------------------------------
			//
			// Compute an expansion for the value of the determinant:
			//
			// | ax ay ( ax^2 + ay^2 ) 1 |
			// | bx by ( bx^2 + by^2 ) 1 | = sum_i=1^384 ( deter )
			// | cx ay ( cx^2 + cy^2 ) 1 |
			// | dx dy ( cx^2 + cy^2 ) 1 | 
			//
			// -------------------------------------------------------

			// Compute an expansion for the product ( ax * ax ) * bcd.
			real det24x[24];
			int xlen = ScaleExpansionZeroElim(
				bcdlen,
				bcd,
				ax,
				det24x
			);

			real det48x[48];
			xlen = ScaleExpansionZeroElim(
				xlen,
				det24x,
				ax,
				det48x
			);

			// Compute an expansion for the product ( ay * ay ) * bcd.
			real det24y[24];
			int ylen = ScaleExpansionZeroElim(
				bcdlen,
				bcd,
				ay,
				det24y
			);

			real det48y[48];
			ylen = ScaleExpansionZeroElim(
				ylen,
				det24y,
				ay,
				det48y
			);

			// Compute an expansion for the sum of det48x and det48y.
			real adet[96];
			int alen = FastExpansionSumZeroElim(
				xlen,
				det48x,
				ylen,
				det48y,
				adet
			);

			// Compute an  expansion for the  product -(  bx * bx  ) *
			// cda.
			xlen = ScaleExpansionZeroElim(
				cdalen,
				cda,
				bx,
				det24x
			);

			xlen = ScaleExpansionZeroElim(
				xlen,
				det24x,
				-bx,
				det48x
			);

			// Compute an  expansion for the  product -(  by * by  ) *
			// cda.
			ylen = ScaleExpansionZeroElim(
				cdalen,
				cda,
				by,
				det24y
			);

			ylen = ScaleExpansionZeroElim(
				ylen,
				det24y,
				-by,
				det48y
			);

			// Compute an expansion for the sum of det48x and det48y.
			real bdet[96];
			int blen = FastExpansionSumZeroElim(
				xlen,
				det48x,
				ylen,
				det48y,
				bdet
			);

			// Compute an expansion for the product ( cx * cx ) * dab.
			xlen = ScaleExpansionZeroElim(
				dablen,
				dab,
				cx,
				det24x
			);


			xlen = ScaleExpansionZeroElim(
				xlen,
				det24x,
				cx,
				det48x
			);

			// Compute an expansion for the product ( cy * cy ) * dab.
			ylen = ScaleExpansionZeroElim(
				dablen,
				dab,
				cy,
				det24y
			);

			ylen = ScaleExpansionZeroElim(
				ylen,
				det24y,
				cy,
				det48y
			);

			// Compute an expansion for the sum of det48x and det48y.
			real cdet[96];
			int clen = FastExpansionSumZeroElim(
				xlen,
				det48x,
				ylen,
				det48y,
				cdet
			);

			// Compute an  expansion for the  product -(  dx * dx  ) *
			// abc.
			xlen = ScaleExpansionZeroElim(
				abclen,
				abc,
				dx,
				det24x
			);


			xlen = ScaleExpansionZeroElim(
				xlen,
				det24x,
				-dx,
				det48x
			);

			// Compute an  expansion for the  product -(  dy * dy  ) *
			// abc.
			ylen = ScaleExpansionZeroElim(
				abclen,
				abc,
				dy,
				det24y
			);

			ylen = ScaleExpansionZeroElim(
				ylen,
				det24y,
				-dy,
				det48y
			);

			// Compute an expansion for the sum of det48x and det48y.
			real ddet[96];
			int dlen = FastExpansionSumZeroElim(
				xlen,
				det48x,
				ylen,
				det48y,
				ddet
			);


			// Add up the  expansions adet, bdet, cdet,  and ddet. The
			// result is the  value of the 4x4  determinant whose sign
			// can be tested for the incircle criterion.
			real abdet[192];
			int ablen = FastExpansionSumZeroElim(
				alen,
				adet,
				blen,
				bdet,
				abdet
			);

			real cddet[192];
			int cdlen = FastExpansionSumZeroElim(
				clen,
				cdet,
				dlen,
				ddet,
				cddet
			);

			real deter[384];
			int deterlen = FastExpansionSumZeroElim(
				ablen,
				abdet,
				cdlen,
				cddet,
				deter
			);

			// Return the largest magnitude component of the expansion
			// deter. The  sign of this  component is the same  as the
			// sign of the determinant.
			return deter[deterlen - 1];
		}


		/**
		 * \fn template <typename real> real Predicates<real>::InCircleAdapt(real ax, real ay, real bx, real by, real cx, real cy, real dx, real dy, real permanent)
		 *
		 * \brief Returns a positive value if point with coordinates (
		 * dx , dy ) lies inside  the circle passing through points of
		 * coordinates ( ax , ay  ) , ( bx , by ), and (  cx , cy ); a
		 * negative value  if it  lies outside, and  zero if  the four
		 * points are cocircular. The points a, b, and c must be given
		 * in counterclockwise order,  or the sign of  the result will
		 * be  reversed.  This  method is  numerically robust  and the
		 * result is adaptively computed from a given approximation.
		 *
		 * \param ax First Cartesian coordinate of the first point.
		 * \param ay Second Cartesian coordinate of the first point.
		 * \param bx First Cartesian coordinate of the second point.
		 * \param by Second Cartesian coordinate of the second point.
		 * \param cx First Cartesian coordinate of the third point.
		 * \param cy Second Cartesian coordinate of the third point.
		 * \param dx  First Cartesian  coordinate of  the point  to be
		 * tested.
		 * \param dy  Second Cartesian coordinate  of the third  to be
		 * tested.
		 * \param permanent An approximation to the exact result.
		 *
		 * \return A positive  value if point with coordinates  ( dx ,
		 * dy  ) lies  inside  the circle  passing  through points  of
		 * coordinates ( ax , ay  ) , ( bx , by ), and (  cx , cy ); a
		 * negative value  if it  lies outside, and  zero if  the four
		 * points belong to the same circumference.
		 */
		template <typename real>
		real
			Predicates<real>::InCircleAdapt(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy,
				real dx,
				real dy,
				real permanent
			)
		{
			// This  incircle test  adaptively  computes an  expansion
			// which is guaranteed to have  the same sign of the value
			// of the determinant below:
			//
			// | (ax - dx) (ay - dy) (ax - dx)^2 + (ay - dy)^2 |
			// | (bx - dx) (by - dy) (bx - dx)^2 + (by - dy)^2 |
			// | (cx - dx) (cy - dy) (cx - dx)^2 + (cy - dy)^2 |

			// Computes the entries  of the first two  columns of the
			// matrix.
			real adx = ax - dx;
			real bdx = bx - dx;
			real cdx = cx - dx;

			real ady = ay - dy;
			real bdy = by - dy;
			real cdy = cy - dy;

			// Computes an expansion bdxcdy1 + bdxcdy0 for ( bx - dx )
			// * ( cy - dy ).
			real bdxcdy1;
			real bdxcdy0;
			TwoProduct(bdx, cdy, bdxcdy1, bdxcdy0);

			// Computes an expansion cdxbdy1+cdxbdy0 for ( cx - dx ) *
			// ( by - dy ).
			real cdxbdy1;
			real cdxbdy0;
			TwoProduct(cdx, bdy, cdxbdy1, cdxbdy0);

			// Computes an  expansion for  ( bdxcdy1 +  bdxcdy0 )  - (
			// cdxbdy1 + cdxbdy0 ).
			real bc[4];
			TwoTwoDiff(
				bdxcdy1,
				bdxcdy0,
				cdxbdy1,
				cdxbdy0,
				bc[3],
				bc[2],
				bc[1],
				bc[0]
			);

			// Computes an expansion adet for bc * (adx^2 + adyˆ2).
			real axbc[8];
			int axbclen = ScaleExpansionZeroElim(
				4,
				bc,
				adx,
				axbc
			);

			real axxbc[16];
			int axxbclen = ScaleExpansionZeroElim(
				axbclen,
				axbc,
				adx,
				axxbc
			);

			real aybc[8];
			int aybclen = ScaleExpansionZeroElim(
				4,
				bc,
				ady,
				aybc
			);

			real ayybc[16];
			int ayybclen = ScaleExpansionZeroElim(
				aybclen,
				aybc,
				ady,
				ayybc
			);

			real adet[32];
			int alen = FastExpansionSumZeroElim(
				axxbclen,
				axxbc,
				ayybclen,
				ayybc,
				adet
			);

			// Computes an expansion cdxady1 + cdxady0 for ( cx - dx )
			// * ( ay - dy ).
			real cdxady1;
			real cdxady0;
			TwoProduct(
				cdx,
				ady,
				cdxady1,
				cdxady0
			);

			// Computes an expansion adxcdy1 + adxcdy0 for ( ax - dx )
			// * ( cy - dy ).
			real adxcdy1;
			real adxcdy0;
			TwoProduct(
				adx,
				cdy,
				adxcdy1,
				adxcdy0
			);

			// Computes an  expansion for  ( cdxady1 +  cdxady0 )  - (
			// adxcdy1 + adxcdy0 ).
			real ca[4];
			TwoTwoDiff(
				cdxady1,
				cdxady0,
				adxcdy1,
				adxcdy0,
				ca[3],
				ca[2],
				ca[1],
				ca[0]
			);

			// Computes an expansion bdet for ca * ( bdx^2 + bdyˆ2 ).
			real bxca[8];
			int bxcalen = ScaleExpansionZeroElim(
				4,
				ca,
				bdx,
				bxca
			);

			real bxxca[16];
			int bxxcalen = ScaleExpansionZeroElim(
				bxcalen,
				bxca,
				bdx,
				bxxca
			);

			real byca[8];
			int bycalen = ScaleExpansionZeroElim(
				4,
				ca,
				bdy,
				byca
			);

			real byyca[16];
			int byycalen = ScaleExpansionZeroElim(
				bycalen,
				byca,
				bdy,
				byyca
			);

			real bdet[32];
			int blen = FastExpansionSumZeroElim(
				bxxcalen,
				bxxca,
				byycalen,
				byyca,
				bdet
			);

			// Computes an expansion adxbdy1+adxbdy0 for ( ax - dx ) *
			// ( by - dy ).
			real adxbdy1;
			real adxbdy0;
			TwoProduct(adx, bdy, adxbdy1, adxbdy0);

			// Computes an expansion bdxady1 + bdxady0 for ( bx - dx )
			// * ( ay - dy ).
			real bdxady1;
			real bdxady0;
			TwoProduct(bdx, ady, bdxady1, bdxady0);

			// Computes an  expansion for  ( adxbdy1 +  adxbdy0 )  - (
			// bdxady1 + bdxady0 ).
			real ab[4];
			TwoTwoDiff(
				adxbdy1,
				adxbdy0,
				bdxady1,
				bdxady0,
				ab[3],
				ab[2],
				ab[1],
				ab[0]
			);

			// Computes an expansion bdet for ab * (cdx^2 + cdyˆ2).
			real cxab[8];
			int cxablen = ScaleExpansionZeroElim(
				4,
				ab,
				cdx,
				cxab
			);

			real cxxab[16];
			int cxxablen = ScaleExpansionZeroElim(
				cxablen,
				cxab,
				cdx,
				cxxab
			);

			real cyab[8];
			int cyablen = ScaleExpansionZeroElim(
				4,
				ab,
				cdy,
				cyab
			);

			real cyyab[16];
			int cyyablen = ScaleExpansionZeroElim(
				cyablen,
				cyab,
				cdy,
				cyyab
			);

			real cdet[32];
			int clen = FastExpansionSumZeroElim(
				cxxablen,
				cxxab,
				cyyablen,
				cyyab,
				cdet
			);

			// Adds up expansions adet, bdet and cdet.
			real abdet[64];
			int ablen = FastExpansionSumZeroElim(
				alen,
				adet,
				blen,
				bdet,
				abdet
			);

			real fin1[1152];
			int finlength = FastExpansionSumZeroElim(
				ablen,
				abdet,
				clen,
				cdet,
				fin1
			);

			// Determines   whether   the   current   value   of   the
			// determinant, i.e.  \c fin1,  is guaranteed to  have the
			// same sign of the exact value of the determinant. If so,
			// return the  current value.  Otherwise, use  the current
			// value to improve the approximation for the exact value.
			real det = Estimate(finlength, fin1);
			real errbound = _iccerrboundB * permanent;
			if ((det >= errbound) || (-det >= errbound)) {
				return det;
			}

			// Improve  the  approximation  of the  determinant  exact
			// value.

			// Computes an expansion adx + adxtail for (ax - dx).
			real adxtail;
			TwoDiffTail(ax, dx, adx, adxtail);

			// Computes an expansion ady + adytail for (ay - dy).
			real adytail;
			TwoDiffTail(ay, dy, ady, adytail);

			// Computes an expansion ady + adytail for (bx - dx).
			real bdxtail;
			TwoDiffTail(bx, dx, bdx, bdxtail);

			// Computes an expansion bdy + bdytail for (by - dy).
			real bdytail;
			TwoDiffTail(by, dy, bdy, bdytail);

			//
			// Computes an expansion cdx + cdxtail for (cx - dx).
			//
			real cdxtail;
			TwoDiffTail(cx, dx, cdx, cdxtail);

			// Computes an expansion cdy + cdytail for (cy - dy).
			real cdytail;
			TwoDiffTail(cy, dy, cdy, cdytail);


			// Determines   whether   the   current   value   of   the
			// determinant, i.e. fin1, is  guaranteed to have the same
			// sign  of the  exact value  of the  determinant. If  so,
			// return the  current value.  Otherwise, use  the current
			// value to improve the approximation for the exact value.
			if (
				(adxtail == real(0)) && (bdxtail == real(0)) &&
				(cdxtail == real(0)) && (adytail == real(0)) &&
				(bdytail == real(0)) && (cdytail == real(0))
				)
			{
				return det;
			}

			errbound = _iccerrboundC * permanent
				+ _resulterrbound * Absolute(det);

			det += (
				(adx * adx + ady * ady) *
				(
				(bdx * cdytail + cdy * bdxtail) -
					(bdy * cdxtail + cdx * bdytail)
					)
				+
				2.0 * (adx * adxtail + ady * adytail) * (bdx * cdy - bdy * cdx)
				)
				+
				(
				(bdx * bdx + bdy * bdy) *
					(
					(cdx * adytail + ady * cdxtail) -
						(cdy * adxtail + adx * cdytail)
						)
					+ 2.0 * (bdx * bdxtail + bdy * bdytail) * (cdx * ady - cdy * adx)
					)
				+
				(
				(cdx * cdx + cdy * cdy) *
					(
					(adx * bdytail + bdy * adxtail) -
						(ady * bdxtail + bdx * adytail)
						)
					+ 2.0 * (cdx * cdxtail + cdy * cdytail) * (adx * bdy - ady * bdx)
					);

			if ((det >= errbound) || (-det >= errbound))
			{
				return det;
			}

			//
			// Compute the exact value of the determinant.
			//

			real aa[4];
			if (
				(bdxtail != real(0)) || (bdytail != real(0)) ||
				(cdxtail != real(0)) || (cdytail != real(0))
				)
			{
				// Computes an expansion for (ax - dx)^2
				real adxadx1;
				real adxadx0;
				Square(adx, adxadx1, adxadx0);

				// Computes an expansion for (ay - dy)^2
				real adyady1;
				real adyady0;
				Square(ady, adyady1, adyady0);

				// Computes an expansion for ( adxadx1 + adxadx0 ) + ( adyady1 + adyady0 ).
				TwoTwoSum(
					adxadx1,
					adxadx0,
					adyady1,
					adyady0,
					aa[3],
					aa[2],
					aa[1],
					aa[0]
				);
			}

			real bb[4];
			if (
				(cdxtail != real(0)) || (cdytail != real(0)) ||
				(adxtail != real(0)) || (adytail != real(0))
				)
			{
				// Computes an expansion for (bx - dx)^2
				real bdxbdx1;
				real bdxbdx0;
				Square(bdx, bdxbdx1, bdxbdx0);

				// Computes an expansion for (by - dy)^2
				real bdybdy1;
				real bdybdy0;
				Square(bdy, bdybdy1, bdybdy0);

				// Computes an expansion for ( bdxbdx1 + bdxbdx0 ) + (
				// bdybdy1 + bdybdy0 ).
				TwoTwoSum(
					bdxbdx1,
					bdxbdx0,
					bdybdy1,
					bdybdy0,
					bb[3],
					bb[2],
					bb[1],
					bb[0]
				);
			}

			real cc[4];
			if (
				(adxtail != real(0)) || (adytail != real(0)) ||
				(bdxtail != real(0)) || (bdytail != real(0))
				)
			{
				//
				// Computes an expansion for (cx - dx)^2
				//
				real cdxcdx1;
				real cdxcdx0;
				Square(cdx, cdxcdx1, cdxcdx0);

				real cdycdy1;
				real cdycdy0;
				Square(cdy, cdycdy1, cdycdy0);

				// Computes an expansion for ( cdxcdx1 + cdxcdx0 ) + (
				// cdycdy1 + cdycdy0 ).
				TwoTwoSum(
					cdxcdx1,
					cdxcdx0,
					cdycdy1,
					cdycdy0,
					cc[3],
					cc[2],
					cc[1],
					cc[0]
				);
			}

			real fin2[1152];

			real* finnow = &fin1[0];
			real* finother = &fin2[0];
			real* finswap = (real*)0;

			real axtbc[8], aytbc[8];
			real axtbb[8], aytbb[8];
			real axtcc[8], aytcc[8];
			real bxtca[8], bytca[8];
			real bxtaa[8], bytaa[8];
			real bxtcc[8], bytcc[8];
			real cxtab[8], cytab[8];
			real cxtaa[8], cytaa[8];
			real cxtbb[8], cytbb[8];

			int axtbclen, axtbblen, axtcclen;
			int aytbclen, aytbblen, aytcclen;
			int bxtcalen, bxtaalen, bxtcclen;
			int bytcalen, bytaalen, bytcclen;
			int cxtablen, cxtaalen, cxtbblen;
			int cytablen, cytaalen, cytbblen;

			real temp16a[16], temp16b[16], temp16c[16];
			real temp32a[32];
			real temp48[48];

			int temp16alen, temp16blen, temp16clen;
			int temp32alen;
			int temp48len;

			//
			// To avoid compiler warnings, initialize some variables:
			//
			axtbclen = 0;
			aytbclen = 0;
			bxtcalen = 0;
			bytcalen = 0;
			cxtablen = 0;
			cytablen = 0;

			if (adxtail != real(0))
			{
				axtbclen = ScaleExpansionZeroElim(
					4,
					bc,
					adxtail,
					axtbc
				);

				temp16alen = ScaleExpansionZeroElim(
					axtbclen,
					axtbc,
					2.0 * adx,
					temp16a
				);

				axtcclen = ScaleExpansionZeroElim(
					4,
					cc,
					adxtail,
					axtcc
				);

				temp16blen = ScaleExpansionZeroElim(
					axtcclen,
					axtcc,
					bdy,
					temp16b
				);

				axtbblen = ScaleExpansionZeroElim(
					4,
					bb,
					adxtail,
					axtbb
				);

				temp16clen = ScaleExpansionZeroElim(
					axtbblen,
					axtbb,
					-cdy,
					temp16c
				);

				temp32alen = FastExpansionSumZeroElim(
					temp16alen,
					temp16a,
					temp16blen,
					temp16b,
					temp32a
				);

				temp48len = FastExpansionSumZeroElim(
					temp16clen,
					temp16c,
					temp32alen,
					temp32a,
					temp48
				);

				finlength = FastExpansionSumZeroElim(
					finlength,
					finnow,
					temp48len,
					temp48,
					finother
				);

				finswap = finnow;
				finnow = finother;
				finother = finswap;
			}

			if (adytail != real(0))
			{
				aytbclen = ScaleExpansionZeroElim(
					4,
					bc,
					adytail,
					aytbc
				);

				temp16alen = ScaleExpansionZeroElim(
					aytbclen,
					aytbc,
					2.0 * ady,
					temp16a
				);

				aytbblen = ScaleExpansionZeroElim(
					4,
					bb,
					adytail,
					aytbb
				);

				temp16blen = ScaleExpansionZeroElim(
					aytbblen,
					aytbb,
					cdx,
					temp16b
				);

				aytcclen = ScaleExpansionZeroElim(
					4,
					cc,
					adytail,
					aytcc
				);

				temp16clen = ScaleExpansionZeroElim(
					aytcclen,
					aytcc,
					-bdx,
					temp16c
				);

				temp32alen = FastExpansionSumZeroElim(
					temp16alen,
					temp16a,
					temp16blen,
					temp16b,
					temp32a
				);

				temp48len = FastExpansionSumZeroElim(
					temp16clen,
					temp16c,
					temp32alen,
					temp32a,
					temp48
				);

				finlength = FastExpansionSumZeroElim(
					finlength,
					finnow,
					temp48len,
					temp48,
					finother
				);
				finswap = finnow;
				finnow = finother;
				finother = finswap;
			}

			if (bdxtail != real(0))
			{
				bxtcalen = ScaleExpansionZeroElim(
					4,
					ca,
					bdxtail,
					bxtca
				);

				temp16alen = ScaleExpansionZeroElim(
					bxtcalen,
					bxtca,
					2.0 * bdx,
					temp16a
				);

				bxtaalen = ScaleExpansionZeroElim(
					4,
					aa,
					bdxtail,
					bxtaa
				);


				temp16blen = ScaleExpansionZeroElim(
					bxtaalen,
					bxtaa,
					cdy,
					temp16b
				);

				bxtcclen = ScaleExpansionZeroElim(
					4,
					cc,
					bdxtail,
					bxtcc
				);


				temp16clen = ScaleExpansionZeroElim(
					bxtcclen,
					bxtcc,
					-ady,
					temp16c
				);

				temp32alen = FastExpansionSumZeroElim(
					temp16alen,
					temp16a,
					temp16blen,
					temp16b,
					temp32a
				);

				temp48len = FastExpansionSumZeroElim(
					temp16clen,
					temp16c,
					temp32alen,
					temp32a,
					temp48
				);

				finlength = FastExpansionSumZeroElim(
					finlength,
					finnow,
					temp48len,
					temp48,
					finother
				);

				finswap = finnow;
				finnow = finother;
				finother = finswap;
			}

			if (bdytail != real(0))
			{
				bytcalen = ScaleExpansionZeroElim(
					4,
					ca,
					bdytail,
					bytca
				);

				temp16alen = ScaleExpansionZeroElim(
					bytcalen,
					bytca,
					2.0 * bdy,
					temp16a
				);

				bytcclen = ScaleExpansionZeroElim(
					4,
					cc,
					bdytail,
					bytcc
				);

				temp16blen = ScaleExpansionZeroElim(
					bytcclen,
					bytcc,
					adx,
					temp16b
				);

				bytaalen = ScaleExpansionZeroElim(
					4,
					aa,
					bdytail,
					bytaa
				);

				temp16clen = ScaleExpansionZeroElim(
					bytaalen,
					bytaa,
					-cdx,
					temp16c
				);

				temp32alen = FastExpansionSumZeroElim(
					temp16alen,
					temp16a,
					temp16blen,
					temp16b,
					temp32a
				);

				temp48len = FastExpansionSumZeroElim(
					temp16clen,
					temp16c,
					temp32alen,
					temp32a,
					temp48
				);

				finlength = FastExpansionSumZeroElim(
					finlength,
					finnow,
					temp48len,
					temp48,
					finother
				);

				finswap = finnow;
				finnow = finother;
				finother = finswap;
			}

			if (cdxtail != real(0))
			{
				cxtablen = ScaleExpansionZeroElim(
					4,
					ab,
					cdxtail,
					cxtab
				);

				temp16alen = ScaleExpansionZeroElim(
					cxtablen,
					cxtab,
					2.0 * cdx,
					temp16a
				);

				cxtbblen = ScaleExpansionZeroElim(
					4,
					bb,
					cdxtail,
					cxtbb
				);

				temp16blen = ScaleExpansionZeroElim(
					cxtbblen,
					cxtbb,
					ady,
					temp16b
				);

				cxtaalen = ScaleExpansionZeroElim(
					4,
					aa,
					cdxtail,
					cxtaa
				);

				temp16clen = ScaleExpansionZeroElim(
					cxtaalen,
					cxtaa,
					-bdy,
					temp16c
				);

				temp32alen = FastExpansionSumZeroElim(
					temp16alen,
					temp16a,
					temp16blen,
					temp16b,
					temp32a
				);

				temp48len = FastExpansionSumZeroElim(
					temp16clen,
					temp16c,
					temp32alen,
					temp32a,
					temp48
				);

				finlength = FastExpansionSumZeroElim(
					finlength,
					finnow,
					temp48len,
					temp48,
					finother
				);

				finswap = finnow;
				finnow = finother;
				finother = finswap;
			}

			if (cdytail != real(0))
			{
				cytablen = ScaleExpansionZeroElim(
					4,
					ab,
					cdytail,
					cytab
				);

				temp16alen = ScaleExpansionZeroElim(
					cytablen,
					cytab,
					2.0 * cdy,
					temp16a
				);

				cytaalen = ScaleExpansionZeroElim(
					4,
					aa,
					cdytail,
					cytaa
				);

				temp16blen = ScaleExpansionZeroElim(
					cytaalen,
					cytaa,
					bdx,
					temp16b
				);

				cytbblen = ScaleExpansionZeroElim(
					4,
					bb,
					cdytail,
					cytbb
				);

				temp16clen = ScaleExpansionZeroElim(
					cytbblen,
					cytbb,
					-adx,
					temp16c
				);

				temp32alen = FastExpansionSumZeroElim(
					temp16alen,
					temp16a,
					temp16blen,
					temp16b,
					temp32a
				);

				temp48len = FastExpansionSumZeroElim(
					temp16clen,
					temp16c,
					temp32alen,
					temp32a,
					temp48
				);

				finlength = FastExpansionSumZeroElim(
					finlength,
					finnow,
					temp48len,
					temp48,
					finother
				);

				finswap = finnow;
				finnow = finother;
				finother = finswap;
			}

			real axtbct[16], aytbct[16];
			int axtbctlen, aytbctlen;

			real axtbctt[8], aytbctt[8];
			int axtbcttlen, aytbcttlen;

			real bct[8], bctt[4];
			int bctlen, bcttlen;

			real ti1, ti0, tj1, tj0, negate, u[4], v[4];

			real temp8[8], temp32b[32], temp64[64];
			int temp8len, temp32blen, temp64len;

			if ((adxtail != real(0)) || (adytail != real(0)))
			{
				if (
					(bdxtail != real(0)) || (bdytail != real(0)) ||
					(cdxtail != real(0)) || (cdytail != real(0))
					)
				{

					TwoProduct(bdxtail, cdy, ti1, ti0);

					TwoProduct(bdx, cdytail, tj1, tj0);

					TwoTwoSum(
						ti1,
						ti0,
						tj1,
						tj0,
						u[3],
						u[2],
						u[1],
						u[0]
					);

					negate = -bdy;
					TwoProduct(cdxtail, negate, ti1, ti0);

					negate = -bdytail;
					TwoProduct(cdx, negate, tj1, tj0);

					TwoTwoSum(
						ti1,
						ti0,
						tj1,
						tj0,
						v[3],
						v[2],
						v[1],
						v[0]
					);

					bctlen = FastExpansionSumZeroElim(
						4,
						u,
						4,
						v,
						bct
					);

					TwoProduct(bdxtail, cdytail, ti1, ti0);
					TwoProduct(cdxtail, bdytail, tj1, tj0);

					TwoTwoDiff(
						ti1,
						ti0,
						tj1,
						tj0,
						bctt[3],
						bctt[2],
						bctt[1],
						bctt[0]
					);

					bcttlen = 4;
				}
				else
				{
					bct[0] = real(0);
					bctlen = 1;
					bctt[0] = real(0);
					bcttlen = 1;
				}

				if (adxtail != real(0))
				{
					temp16alen = ScaleExpansionZeroElim(
						axtbclen,
						axtbc,
						adxtail,
						temp16a
					);

					axtbctlen = ScaleExpansionZeroElim(
						bctlen,
						bct,
						adxtail,
						axtbct
					);

					temp32alen = ScaleExpansionZeroElim(
						axtbctlen,
						axtbct,
						2.0 * adx,
						temp32a
					);

					temp48len = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp32alen,
						temp32a,
						temp48
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp48len,
						temp48,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;

					if (bdytail != real(0))
					{
						temp8len = ScaleExpansionZeroElim(
							4,
							cc,
							adxtail,
							temp8
						);

						temp16alen = ScaleExpansionZeroElim(
							temp8len,
							temp8,
							bdytail,
							temp16a
						);

						finlength = FastExpansionSumZeroElim(
							finlength,
							finnow,
							temp16alen,
							temp16a,
							finother
						);

						finswap = finnow;
						finnow = finother;
						finother = finswap;
					}

					if (cdytail != real(0))
					{
						temp8len = ScaleExpansionZeroElim(
							4,
							bb,
							-adxtail,
							temp8
						);

						temp16alen = ScaleExpansionZeroElim(
							temp8len,
							temp8,
							cdytail,
							temp16a
						);

						finlength = FastExpansionSumZeroElim(
							finlength,
							finnow,
							temp16alen,
							temp16a,
							finother
						);
						finswap = finnow;
						finnow = finother;
						finother = finswap;
					}

					temp32alen = ScaleExpansionZeroElim(
						axtbctlen,
						axtbct,
						adxtail,
						temp32a
					);

					axtbcttlen = ScaleExpansionZeroElim(
						bcttlen,
						bctt,
						adxtail,
						axtbctt
					);

					temp16alen = ScaleExpansionZeroElim(
						axtbcttlen,
						axtbctt,
						2.0 * adx,
						temp16a
					);

					temp16blen = ScaleExpansionZeroElim(
						axtbcttlen,
						axtbctt,
						adxtail,
						temp16b
					);

					temp32blen = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp16blen,
						temp16b,
						temp32b
					);

					temp64len = FastExpansionSumZeroElim(
						temp32alen,
						temp32a,
						temp32blen,
						temp32b,
						temp64
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp64len,
						temp64,
						finother
					);
					finswap = finnow;
					finnow = finother;
					finother = finswap;
				}

				if (adytail != real(0))
				{
					temp16alen = ScaleExpansionZeroElim(
						aytbclen,
						aytbc,
						adytail,
						temp16a
					);

					aytbctlen = ScaleExpansionZeroElim(
						bctlen,
						bct,
						adytail,
						aytbct
					);

					temp32alen = ScaleExpansionZeroElim(
						aytbctlen,
						aytbct,
						2.0 * ady,
						temp32a
					);

					temp48len = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp32alen,
						temp32a,
						temp48
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp48len,
						temp48,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;

					temp32alen = ScaleExpansionZeroElim(
						aytbctlen,
						aytbct,
						adytail,
						temp32a
					);

					aytbcttlen = ScaleExpansionZeroElim(
						bcttlen,
						bctt,
						adytail,
						aytbctt
					);

					temp16alen = ScaleExpansionZeroElim(
						aytbcttlen,
						aytbctt,
						2.0 * ady,
						temp16a
					);

					temp16blen = ScaleExpansionZeroElim(
						aytbcttlen,
						aytbctt,
						adytail,
						temp16b
					);

					temp32blen = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp16blen,
						temp16b,
						temp32b
					);

					temp64len = FastExpansionSumZeroElim(
						temp32alen,
						temp32a,
						temp32blen,
						temp32b,
						temp64
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp64len,
						temp64,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;
				}
			}

			real bxtcat[16], bytcat[16];
			int bxtcatlen, bytcatlen;

			real bxtcatt[8], bytcatt[8];
			int bxtcattlen, bytcattlen;

			real cat[8], catt[4];
			int catlen, cattlen;

			if ((bdxtail != real(0)) || (bdytail != real(0)))
			{
				if (
					(cdxtail != real(0)) || (cdytail != real(0)) ||
					(adxtail != real(0)) || (adytail != real(0))
					)
				{
					TwoProduct(cdxtail, ady, ti1, ti0);
					TwoProduct(cdx, adytail, tj1, tj0);
					TwoTwoSum(
						ti1,
						ti0,
						tj1,
						tj0,
						u[3],
						u[2],
						u[1],
						u[0]
					);

					negate = -cdy;
					TwoProduct(adxtail, negate, ti1, ti0);

					negate = -cdytail;
					TwoProduct(adx, negate, tj1, tj0);

					TwoTwoSum(
						ti1,
						ti0,
						tj1,
						tj0,
						v[3],
						v[2],
						v[1],
						v[0]
					);

					catlen = FastExpansionSumZeroElim(
						4,
						u,
						4,
						v,
						cat
					);

					TwoProduct(cdxtail, adytail, ti1, ti0);
					TwoProduct(adxtail, cdytail, tj1, tj0);

					TwoTwoDiff(
						ti1,
						ti0,
						tj1,
						tj0,
						catt[3],
						catt[2],
						catt[1],
						catt[0]
					);
					cattlen = 4;
				}
				else
				{
					cat[0] = real(0);
					catlen = 1;
					catt[0] = real(0);
					cattlen = 1;
				}

				if (bdxtail != real(0))
				{
					temp16alen = ScaleExpansionZeroElim(
						bxtcalen,
						bxtca,
						bdxtail,
						temp16a
					);

					bxtcatlen = ScaleExpansionZeroElim(
						catlen,
						cat,
						bdxtail,
						bxtcat
					);

					temp32alen = ScaleExpansionZeroElim(
						bxtcatlen,
						bxtcat,
						2.0 * bdx,
						temp32a
					);

					temp48len = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp32alen,
						temp32a,
						temp48
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp48len,
						temp48,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;

					if (cdytail != real(0))
					{
						temp8len = ScaleExpansionZeroElim(
							4,
							aa,
							bdxtail,
							temp8
						);

						temp16alen = ScaleExpansionZeroElim(
							temp8len,
							temp8,
							cdytail,
							temp16a
						);

						finlength = FastExpansionSumZeroElim(
							finlength,
							finnow,
							temp16alen,
							temp16a,
							finother
						);

						finswap = finnow;
						finnow = finother;
						finother = finswap;
					}

					if (adytail != real(0))
					{
						temp8len = ScaleExpansionZeroElim(
							4,
							cc,
							-bdxtail,
							temp8
						);

						temp16alen = ScaleExpansionZeroElim(
							temp8len,
							temp8,
							adytail,
							temp16a
						);

						finlength = FastExpansionSumZeroElim(
							finlength,
							finnow,
							temp16alen,
							temp16a,
							finother
						);

						finswap = finnow;
						finnow = finother;
						finother = finswap;
					}

					temp32alen = ScaleExpansionZeroElim(
						bxtcatlen,
						bxtcat,
						bdxtail,
						temp32a
					);

					bxtcattlen = ScaleExpansionZeroElim(
						cattlen,
						catt,
						bdxtail,
						bxtcatt
					);

					temp16alen = ScaleExpansionZeroElim(
						bxtcattlen,
						bxtcatt,
						2.0 * bdx,
						temp16a
					);

					temp16blen = ScaleExpansionZeroElim(
						bxtcattlen,
						bxtcatt,
						bdxtail,
						temp16b
					);

					temp32blen = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp16blen,
						temp16b,
						temp32b
					);

					temp64len = FastExpansionSumZeroElim(
						temp32alen,
						temp32a,
						temp32blen,
						temp32b,
						temp64
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp64len,
						temp64,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;
				}

				if (bdytail != real(0))
				{
					temp16alen = ScaleExpansionZeroElim(
						bytcalen,
						bytca,
						bdytail,
						temp16a
					);

					bytcatlen = ScaleExpansionZeroElim(
						catlen,
						cat,
						bdytail,
						bytcat
					);

					temp32alen = ScaleExpansionZeroElim(
						bytcatlen,
						bytcat,
						2.0 * bdy,
						temp32a
					);

					temp48len = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp32alen,
						temp32a,
						temp48
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp48len,
						temp48,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;

					temp32alen = ScaleExpansionZeroElim(
						bytcatlen,
						bytcat,
						bdytail,
						temp32a
					);

					bytcattlen = ScaleExpansionZeroElim(
						cattlen,
						catt,
						bdytail,
						bytcatt
					);

					temp16alen = ScaleExpansionZeroElim(
						bytcattlen,
						bytcatt,
						2.0 * bdy,
						temp16a
					);

					temp16blen = ScaleExpansionZeroElim(
						bytcattlen,
						bytcatt,
						bdytail,
						temp16b
					);

					temp32blen = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp16blen,
						temp16b,
						temp32b
					);

					temp64len = FastExpansionSumZeroElim(
						temp32alen,
						temp32a,
						temp32blen,
						temp32b,
						temp64
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp64len,
						temp64,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;
				}
			}

			real cxtabt[16], cytabt[16];
			int cxtabtlen, cytabtlen;

			real cxtabtt[8], cytabtt[8];
			int cxtabttlen, cytabttlen;

			real abt[8], abtt[4];
			int abtlen, abttlen;

			if ((cdxtail != real(0)) || (cdytail != real(0)))
			{
				if (
					(adxtail != real(0)) || (adytail != real(0)) ||
					(bdxtail != real(0)) || (bdytail != real(0))
					)
				{
					TwoProduct(adxtail, bdy, ti1, ti0);
					TwoProduct(adx, bdytail, tj1, tj0);
					TwoTwoSum(
						ti1,
						ti0,
						tj1,
						tj0,
						u[3],
						u[2],
						u[1],
						u[0]
					);

					negate = -ady;
					TwoProduct(bdxtail, negate, ti1, ti0);

					negate = -adytail;
					TwoProduct(bdx, negate, tj1, tj0);

					TwoTwoSum(
						ti1,
						ti0,
						tj1,
						tj0,
						v[3],
						v[2],
						v[1],
						v[0]
					);

					abtlen = FastExpansionSumZeroElim(
						4,
						u,
						4,
						v,
						abt
					);

					TwoProduct(adxtail, bdytail, ti1, ti0);
					TwoProduct(bdxtail, adytail, tj1, tj0);

					TwoTwoDiff(
						ti1,
						ti0,
						tj1,
						tj0,
						abtt[3],
						abtt[2],
						abtt[1],
						abtt[0]
					);

					abttlen = 4;
				}
				else
				{
					abt[0] = real(0);
					abtlen = 1;
					abtt[0] = real(0);
					abttlen = 1;
				}

				if (cdxtail != real(0))
				{
					temp16alen = ScaleExpansionZeroElim(
						cxtablen,
						cxtab,
						cdxtail,
						temp16a
					);

					cxtabtlen = ScaleExpansionZeroElim(
						abtlen,
						abt,
						cdxtail,
						cxtabt
					);

					temp32alen = ScaleExpansionZeroElim(
						cxtabtlen,
						cxtabt,
						2.0 * cdx,
						temp32a
					);

					temp48len = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp32alen,
						temp32a,
						temp48
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp48len,
						temp48,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;

					if (adytail != real(0))
					{
						temp8len = ScaleExpansionZeroElim(
							4,
							bb,
							cdxtail,
							temp8
						);

						temp16alen = ScaleExpansionZeroElim(
							temp8len,
							temp8,
							adytail,
							temp16a
						);

						finlength = FastExpansionSumZeroElim(
							finlength,
							finnow,
							temp16alen,
							temp16a,
							finother
						);

						finswap = finnow;
						finnow = finother;
						finother = finswap;
					}

					if (bdytail != real(0))
					{
						temp8len = ScaleExpansionZeroElim(
							4,
							aa,
							-cdxtail,
							temp8
						);

						temp16alen = ScaleExpansionZeroElim(
							temp8len,
							temp8,
							bdytail,
							temp16a
						);

						finlength = FastExpansionSumZeroElim(
							finlength,
							finnow,
							temp16alen,
							temp16a,
							finother
						);

						finswap = finnow;
						finnow = finother;
						finother = finswap;
					}

					temp32alen = ScaleExpansionZeroElim(
						cxtabtlen,
						cxtabt,
						cdxtail,
						temp32a
					);

					cxtabttlen = ScaleExpansionZeroElim(
						abttlen,
						abtt,
						cdxtail,
						cxtabtt
					);

					temp16alen = ScaleExpansionZeroElim(
						cxtabttlen,
						cxtabtt,
						2.0 * cdx,
						temp16a
					);

					temp16blen = ScaleExpansionZeroElim(
						cxtabttlen,
						cxtabtt,
						cdxtail,
						temp16b
					);

					temp32blen = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp16blen,
						temp16b,
						temp32b
					);

					temp64len = FastExpansionSumZeroElim(
						temp32alen,
						temp32a,
						temp32blen,
						temp32b,
						temp64
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp64len,
						temp64,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;
				}

				if (cdytail != real(0))
				{
					temp16alen = ScaleExpansionZeroElim(
						cytablen,
						cytab,
						cdytail,
						temp16a
					);

					cytabtlen = ScaleExpansionZeroElim(
						abtlen,
						abt,
						cdytail,
						cytabt
					);

					temp32alen = ScaleExpansionZeroElim(
						cytabtlen,
						cytabt,
						2.0 * cdy,
						temp32a
					);

					temp48len = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp32alen,
						temp32a,
						temp48
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp48len,
						temp48,
						finother
					);

					finswap = finnow;
					finnow = finother;
					finother = finswap;

					temp32alen = ScaleExpansionZeroElim(
						cytabtlen,
						cytabt,
						cdytail,
						temp32a
					);

					cytabttlen = ScaleExpansionZeroElim(
						abttlen,
						abtt,
						cdytail,
						cytabtt
					);

					temp16alen = ScaleExpansionZeroElim(
						cytabttlen,
						cytabtt,
						2.0 * cdy,
						temp16a
					);

					temp16blen = ScaleExpansionZeroElim(
						cytabttlen,
						cytabtt,
						cdytail,
						temp16b
					);

					temp32blen = FastExpansionSumZeroElim(
						temp16alen,
						temp16a,
						temp16blen,
						temp16b,
						temp32b
					);

					temp64len = FastExpansionSumZeroElim(
						temp32alen,
						temp32a,
						temp32blen,
						temp32b,
						temp64
					);

					finlength = FastExpansionSumZeroElim(
						finlength,
						finnow,
						temp64len,
						temp64,
						finother
					);
					finswap = finnow;
					finnow = finother;
					finother = finswap;
				}
			}

			return finnow[finlength - 1];
		}


		/**
		 * \fn template <typename real> real Predicates<real>::InCircleAdapt(real ax, real ay, real bx, real by, real cx, real cy, real dx, real dy)
		 *
		 * \brief Returns a positive value if point with coordinates (
		 * dx , dy ) lies inside  the circle passing through points of
		 * coordinates ( ax , ay  ) , ( bx , by ), and (  cx , cy ); a
		 * negative value  if it  lies outside, and  zero if  the four
		 * points are cocircular. The points a, b, and c must be given
		 * in counterclockwise order,  or the sign of  the result will
		 * be reversed.  This method is numerically robust.
		 *
		 * \param ax First Cartesian coordinate of the first point.
		 * \param ay Second Cartesian coordinate of the first point.
		 * \param bx First Cartesian coordinate of the second point.
		 * \param by Second Cartesian coordinate of the second point.
		 * \param cx First Cartesian coordinate of the third point.
		 * \param cy Second Cartesian coordinate of the third point.
		 * \param dx  First Cartesian  coordinate of  the point  to be
		 * tested.
		 * \param dy  Second Cartesian coordinate  of the third  to be
		 * tested.
		 *
		 * \return A positive  value if point with coordinates  ( dx ,
		 * dy  ) lies  inside  the circle  passing  through points  of
		 * coordinates ( ax , ay  ) , ( bx , by ), and (  cx , cy ); a
		 * negative value  if it  lies outside, and  zero if  the four
		 * points belong to the same circumference.
		 *
		 */
		template <typename real>
		real
			Predicates<real>::InCircleAdapt(
				real ax,
				real ay,
				real bx,
				real by,
				real cx,
				real cy,
				real dx,
				real dy
			)
		{
			real adx = ax - dx;
			real bdx = bx - dx;
			real cdx = cx - dx;
			real ady = ay - dy;
			real bdy = by - dy;
			real cdy = cy - dy;

			real bdxcdy = bdx * cdy;
			real cdxbdy = cdx * bdy;
			real alift = adx * adx + ady * ady;

			real cdxady = cdx * ady;
			real adxcdy = adx * cdy;
			real blift = bdx * bdx + bdy * bdy;

			real adxbdy = adx * bdy;
			real bdxady = bdx * ady;
			real clift = cdx * cdx + cdy * cdy;

			real det = alift * (bdxcdy - cdxbdy)
				+ blift * (cdxady - adxcdy)
				+ clift * (adxbdy - bdxady);

			real permanent = (Absolute(bdxcdy) + Absolute(cdxbdy)) * alift
				+ (Absolute(cdxady) + Absolute(adxcdy)) * blift
				+ (Absolute(adxbdy) + Absolute(bdxady)) * clift;

			real errbound = _iccerrboundA * permanent;

			if ((det > errbound) || (-det > errbound)) {
				return det;
			}

			return InCircleAdapt(
				ax,
				ay,
				bx,
				by,
				cx,
				cy,
				dx,
				dy,
				permanent
			);
		}

	}

}

/** @} */ //end of group class.
