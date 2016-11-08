/**
 * \file Vertex.h
 *
 * \brief  Definition and  implementation of  a class  to represent  a
 * constrained Delaunay  triangulation (CDT)  vertex in  the quad-edge
 * data structure.
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

#include "Visitor.h"      // Visitor
#include "DtPoint.h"      // DtPoint

#include <cstddef>        // size_t
#include <list>           // std::list
#include <memory>         // std::shared_ptr


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
		 * Forward definition of class CDT.
		 */
		class CDT;


		/**
		 * \class Vertex
		 *
		 * \brief This  class represents  a vertex of  an edge  in the
		 * quad-edge data  structure.  In principle, the  vertex could
		 * be in  \f$E^n\f$, but  our implementation of  the quad-edge
		 * data structure  is restricted to subdivisions  in \f$E^2\f$
		 * (the 2d Euclidean affine space).
		 */
		class Vertex
		{
		public:

			// -------------------------------------------------------
			//
			// Type definitions
			//
			// -------------------------------------------------------

			/**
			 * \typedef VertexList
			 *
			 * \brief Definition of  a type for a list  of pointers to
			 * vertices.
			 *
			 */
			typedef std::list<Vertex*> VertexList;


			/**
			 * \typedef VertexIterator
			 *
			 * \brief Definition of a type for a vertex list iterator.
			 *
			 */
			typedef VertexList::iterator VertexIterator;


			/**
			 * \typedef spPoint
			 *
			 * \brief Alias for a shared pointer to DtPoint objects.
			 *
			 */
			typedef DtPoint::spPoint spPoint;


		protected:
			
			// -------------------------------------------------------
			//
			// Protected data members
			//
			// -------------------------------------------------------

			spPoint _point;         ///< A pointer to an object defining the geometric information of this vertex.
			Edge* _edge;            ///< Pointer to an edge that has this vertex as its origin.
			VertexIterator _pos;    ///< Position of this vertex in the list of vertices of the CDT it belongs to. 


		public:

			// -------------------------------------------------------
			//
			// Public methods
			//
			// -------------------------------------------------------

			/**
			 * \fn Vertex()
			 *
			 * \brief Creates an instance of this class.
			 *
			 */
			Vertex() : _point(std::make_shared<DtPoint>()), _edge(nullptr)
			{
			}


			/**
			 * \fn Vertex(spPoint pt, Edge* e)
			 *
			 * \brief Creates an instance of this class.
			 *
			 * \param pt A shared  pointer to  the geometric  information of
			 * this vertex.
			 * \param e An edge incident to this vertex.
			 *
			 */
			Vertex(
				spPoint pt,
				Edge* e
			) 
				: 
				_point(pt),
				_edge(e)
			{
			}


			/**
			 * \fn Vertex(const Vertex& v)
			 *
			 * \brief Creates  an instance of this  class from another
			 * instance of this class.
			 *
			 * \param v An instance of this class.
			 *
			 */
			Vertex(
				const Vertex& p
			) 
				: 
				_point(p._point),
				_edge(p._edge),
				_pos(p._pos)       // shallow copy!
			{
			}


			/**
			 * \fn virtual ~Vertex()
			 *
			 * \brief Releases the memory held  by an instance of this
			 * class.
			 *
			 */
			virtual ~Vertex()
			{
			}


			/**
			 * \fn double GetU() const
			 *
			 * \brief Returns  the first Cartesian coordinate  of this
			 * vertex.
			 *
			 * \return The first Cartesian coordinate of this vertex.
			 *
			 */
			double GetU() const
			{
				return _point->GetU();
			}


			/**
			 * \fn double GetV() const
			 *
			 * \brief Returns the second  Cartesian coordinate of this
			 * vertex.
			 *
			 * \return The second Cartesian coordinate of this vertex.
			 *
			 */
			double GetV() const
			{
				return _point->GetV();
			}


			/**
			 * \fn spPoint GetPoint() const
			 *
			 * \brief Returns the point associated with this vertex.
			 *
			 * \return  A pointer  to the  point associated  with this
			 * vertex.
			 *
			 */
			spPoint GetPoint() const
			{
				return _point;
			}


			/**
			 * \fn Edge* GetEdge() const
			 *
			 * \brief Returns an edge incident to this vertex.
			 *
			 * \return An edge incident to this vertex.
			 *
			 */
			Edge* GetEdge() const
			{
				return _edge;
			}


			/**
			 * \fn VertexIterator GetPosition() const
			 *
			 * \brief Returns the position of  this vertex in the list
			 * of vertices  of the constrained  Delaunay triangulation
			 * it belongs to.
			 *
			 * \return  The position  of this  vertex in  the list  of
			 * vertices of  the constrained Delaunay  triangulation it
			 * belongs to.
			 *
			 */
			VertexIterator GetPosition() const
			{
				return _pos;
			}


			/**
			 * \fn void SetCoordinates(double u, double v)
			 *
			 * \brief  Assigns  values  to  the  coordinates  of  this
			 * vertex.
			 *
			 * \param u The first Cartesian coordinate of this vertex.
			 * \param  v  The  second  Cartesian  coordinate  of  this
			 * vertex.
			 *
			 */
			void 
				SetCoordinates(
					double u,
					double v
				)
			{
				_point->SetParameterCoordinates(u, v);
			}


			/**
			 * \fn void SetEdge(Edge* e)
			 *
			 * \brief Assigns an edge to this vertex.
			 *
			 * \param e A pointer to an edge.
			 *
			 */
			void
				SetEdge(
					Edge* e
				)
			{
				_edge = e;
			}


			/**
			 * \fn bool operator==(const Vertex& p) const
			 *
			 * \brief Returns the Boolean value  true if, and only if,
			 * this vertex is the same as a given one.
			 *
			 * \param p A given vertex.
			 *
			 * \return The  Boolean value true  if this vertex  is the
			 * same  as  a given  one,  and  the Boolean  value  false
			 * otherwise.
			 *
			 */
			bool 
				operator==(
					const Vertex& p
				) 
				const
			{
				return (GetU() == p.GetU()) && (GetV() == p.GetV());
			}


			/**
			 * \fn bool operator!=(const Vertex& v) const
			 *
			 * \brief Returns the Boolean value  true if, and only if,
			 * this vertex is not the same as a given one.
			 *
			 * \param v A given vertex.
			 *
			 * \return The  Boolean value true  if this vertex  is not
			 * the same  as a given  one, and the Boolean  value false
			 * otherwise.
			 *
			 */
			bool 
				operator!=(
					const Vertex& v
				) 
				const
			{
				return !(*this == v);
			}


			/**
			 * \fn void Accept(Visitor* visitor)
			 *
			 * \brief Accepts a visit of a generic visitor.
			 *
			 * \param visitor A pointer to a generic visitor.
			 *
			 */
			void 
				Accept(
					Visitor* visitor
				)
			{
				visitor->Visit(this);

				return;
			}

			/**
			 * Declare CDT as a friend of this class.
			 */
			friend class CDT;


		protected:

			// -------------------------------------------------------
			//
			// Protected methods
			//
			// -------------------------------------------------------

			/**
			 * \fn void SetPosition(const std::list<Vertex*>::iterator& it)
			 *
			 * \brief Assigns a  position to this vertex in  a list of
			 * points of the CDT it belongs to.
			 *
			 * \param it  Iterator position in  a list of  vertices of
			 * the CDT.
			 *
			 */
			void
				SetPosition(
					const VertexIterator& it
				)
			{
				_pos = it;
			}

		};

	}

}

/** @} */ //end of group class.
