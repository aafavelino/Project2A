/**
 * \file Edge.h
 *
 * \brief  Definition  and implementation  of  the  class Edge,  which
 * represents any  of the four edges  of a quad-edge in  the quad-edge
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

#include "Visitor.h"   // Visitor
#include "Vertex.h"    // Vertex

#include <stdexcept>   // std::runtime_error
#include <list>        // std::list


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

		// Forward definition of class CDT.
		class CDT;

		// Forward definition of class QuadEdge.
		class QuadEdge;

		// Forward definition of class Face.
		class Face;


		/**
		 * \class Edge
		 *
		 * \brief This  class represents  any of the  four edges  of a
		 * quad-edge in the quad-edge data structure.
		 */
		class Edge {
		public:

			// -------------------------------------------------------
			//
			// Type definitions
			//
			// -------------------------------------------------------

			/**
			 * \typedef EdgeList
			 *
			 * \brief Definition of  a type for a list  of pointers to
			 * edges.
			 *
			 */
			typedef std::list<Edge*> EdgeList;


			/**
			 * \typedef EdgeIterator
			 *
			 * \brief Definition of a type for an edge list iterator.
			 *
			 */
			typedef EdgeList::iterator EdgeIterator;


		protected:

			// -------------------------------------------------------
			//
			// Protected data members
			//
			// -------------------------------------------------------

			QuadEdge* _owner;     ///< The quad edge that contains this edge.
			unsigned _id;         ///< The id of this edge (an integer from 0 to 3 ).
			Edge* _next;          ///< The address of the \c next edge.
			Vertex* _orig;        ///< The origin vertex of this edge.
			Face* _face;          ///< A bounded face containing this edge (if one exists).
			EdgeIterator _pos;    ///< Position of this edge in a list of edges of the CDT.

		public:

			// ---------------------------------------------------------------
			//
			// Public methods
			//
			// ---------------------------------------------------------------

			/**
			 * \fn Edge()
			 *
			 * \brief Creates an instance of this class.
			 *
			 */
			Edge() 
				:
				_owner(nullptr),
				_id(0),
				_next(nullptr),
				_orig(nullptr),
				_face(nullptr)
			{
			}


			/**
			 * \fn virtual ~Edge()
			 *
			 * \brief Releases the memory held  by an instance of this
			 * class.
			 *
			 */
			virtual ~Edge()
			{
			}


			/**
			 * \fn QuadEdge* GetOwner() const
			 *
			 * \brief Returns a pointer to the quad-edge that contains
			 * this edge.
			 *
			 * \return A pointer to the quad-edge that contains this edge.
			 *
			 */
			QuadEdge* GetOwner() const
			{
				return _owner;
			}


			/**
			 * \fn unsigned GetId() const
			 *
			 * \brief Returns the identifier of this edge.
			 *
			 * \return The identifier of this edge.
			 *
			 */
			unsigned GetId() const
			{
				return _id;
			}


			/**
			 * \fn Vertex* GetOrigin() const
			 *
			 * \brief Returns the origin vertex of this edge.
			 *
			 * \return The origin vertex of this edge.
			 *
			 */
			Vertex* GetOrigin() const
			{
				return _orig;
			}


			/**
			 * \fn Face* GetFace() const
			 *
			 * \brief Returns  the bounded face containing  this edge.
			 * If the  edge does  not belong to  a bounded  face, then
			 * return a NULL pointer.
			 *
			 * \return The face containing this edge.
			 *
			 */
			Face* GetFace() const
			{
				return _face;
			}


			/**
			 * \fn Vertex* GetDestination() const
			 *
			 * \brief Returns the destination vertex of this edge.
			 *
			 * \return The destination vertex of this edge.
			 *
			 */
			Vertex* GetDestination() const
			{
				return Symmetric()->GetOrigin();
			}


			/**
			 * \fn EdgeIterator GetPosition() const
			 *
			 * \brief Returns the  position of this edge in  a list of
			 * edges of the triangulation it belongs to.
			 *
			 * \return The position of this edge in a list of edges of
			 * the triangulation it belongs to.
			 *
			 */
			EdgeIterator GetPosition() const
			{
				return _pos;
			}


			/**
			 * \fn Edge* Rotation() const
			 *
			 * \brief Returns the dual of this edge.
			 *
			 * \return The dual of this edge.
			 *
			 */
			Edge* Rotation() const
			{
				return (GetId() < 3) ? const_cast<Edge*>(this + 1) :
					const_cast<Edge*>(this - 3);
			}


			/**
			 * \fn Edge* InverseRotation() const
			 *
			 * \brief Returns the symmetric of the dual of this edge.
			 *
			 * \return The symmetric of the dual of this edge.
			 *
			 */
			Edge* InverseRotation() const
			{
				return (GetId() > 0) ? const_cast<Edge*>(this - 1) :
					const_cast<Edge*>(this + 3);
			}


			/**
			 * \fn Edge* Symmetric() const
			 *
			 * \brief Returns the symmetric of this edge.
			 *
			 * \return The symmetric of this edge.
			 *
			 */
			Edge* Symmetric() const
			{
				return (GetId() < 2) ? const_cast<Edge*>(this + 2) :
					const_cast<Edge*>(this - 2);
			}


			/**
			 * \fn Edge* OriginNext() const
			 *
			 * \brief Returns the  next edge in a CCW  traverse of the
			 * star of the origin vertex of this edge.
			 *
			 * \return The next edge in a  CCW traverse of the star of
			 * the origin vertex of this edge.
			 *
			 */
			Edge* OriginNext() const
			{
				return _next;
			}


			/**
			 * \fn Edge* OriginPrev() const
			 *
			 * \brief Returns  the next edge  in a CW traverse  of the
			 * star of the origin vertex of this edge.
			 *
			 * \return The next  edge in a CW traverse of  the star of
			 * the origin vertex of this edge.
			 *
			 */
			Edge* OriginPrev() const
			{
				return Rotation()->OriginNext()->Rotation();
			}


			/**
			 * \fn Edge* DestinationNext() const
			 *
			 * \brief Returns the  next edge in a CCW  traverse of the
			 * star of the destination vertex this edge.
			 *
			 * \return The next edge in a  CCW traverse of the star of
			 * the destination vertex of this edge.
			 *
			 */
			Edge* DestinationNext() const
			{
				return Symmetric()->OriginNext()->Symmetric();
			}


			/**
			 * \fn Edge* DestinationPrev() const
			 *
			 * \brief Returns  the next edge  in a CW traverse  of the
			 * star of the destination vertex of this edge.
			 *
			 * \return The next  edge in a CW traverse of  the star of
			 * the destination vertex of this edge.
			 *
			 */
			Edge* DestinationPrev() const
			{
				return InverseRotation()->OriginNext()->InverseRotation();
			}


			/**
			 * \fn Edge* LeftNext() const
			 *
			 * \brief Returns the  next edge in a CCW  traverse of the
			 * left face of this edge.
			 *
			 * \return The  next edge  in a CCW  traverse of  the left
			 * face of this edge.
			 *
			 */
			Edge* LeftNext() const
			{
				return InverseRotation()->OriginNext()->Rotation();
			}


			/**
			 * \fn Edge* LeftPrev() const
			 *
			 * \brief Returns  the previous edge  in a CW  traverse of
			 * the left face of this edge.
			 *
			 * \return The previous edge in  a CW traverse of the left
			 * face of this edge.
			 *
			 */
			Edge* LeftPrev() const
			{
				return OriginNext()->Symmetric();
			}


			/**
			 * \fn Edge* RightNext() const
			 *
			 * \brief Returns the  next edge in a CCW  traverse of the
			 * right face of this edge.
			 *
			 * \return The  next edge in  a CCW traverse of  the right
			 * face of this edge.
			 *
			 */
			Edge* RightNext() const
			{
				return Rotation()->OriginNext()->InverseRotation();
			}


			/**
			 * \fn Edge* RightPrev() const
			 *
			 * \brief Returns  the previous edge  in a CW  traverse of
			 * the right face of this edge.
			 *
			 * \return The previous edge in a CW traverse of the right
			 * face of this edge.
			 *
			 */
			Edge* RightPrev() const
			{
				return Symmetric()->OriginNext();
			}


			/**
			 * \fn void SetEndpoints(Vertex* orig, Vertex* dest)
			 *
			 * \brief Assigns an origin and a destination vertex to an
			 * edge of this quad-edge.
			 *
			 * \param orig A vertex to be assigned as origin vertex of
			 * an edge.
			 * \param  dest A  vertex  to be  assigned as  destination
			 * vertex of an edge.
			 *
			 */
			void 
				SetEndpoints(
					Vertex* orig,
					Vertex* dest
				)
			{
				SetOrigin(orig);
				SetDestination(dest);

				return;
			}


			/**
			 * \fn void Splice(Edge* e)
			 *
			 * \brief If this  edge and the given edge  share the same
			 * origin  vertex, then  this method  will break  the star
			 * into two ones (i.e., this  edge and the given edge will
			 * be separated from each  other).  Otherwise, the star of
			 * the  origin vertex  of this  edge and  the star  of the
			 * origin vertex of  the given edge will  be combined into
			 * one star  (i.e., this edge  and the given edge  will be
			 * attached  to  each other).   In  the  latter case,  the
			 * operation   only   makes   sense  (from   a   geometric
			 * perspective) if  the origin vertices of  both edges are
			 * the "same". But,  topologically speaking, the operation
			 * is always consistent (see Guibas and Stolfi paper).
			 *
			 * \param e A pointer to a given edge.
			 *
			 */
			void 
				Splice(
					Edge* e
				)
			{
				Edge* tnextdual = this->OriginNext()->Rotation();
				Edge* enextdual = e->OriginNext()->Rotation();

				Edge* temp1 = e->OriginNext();
				Edge* temp2 = this->OriginNext();
				Edge* temp3 = enextdual->OriginNext();
				Edge* temp4 = tnextdual->OriginNext();

				this->SetNext(temp1);
				e->SetNext(temp2);

				tnextdual->SetNext(temp3);
				enextdual->SetNext(temp4);

				return;
			}

			//
			// The implementation of isConstrained() is in Quadedge.h
			//

			/**
			 * \fn inline bool IsConstrained() const
			 *
			 * \brief Returns the Boolean value true if this edge is a
			 * constrained   edge,  and   the   Boolean  value   false
			 * otherwise.
			 *
			 * \return The  Boolean value  true if  the edges  of this
			 * quad-edge are constrained edges,  and the Boolean value
			 * false otherwise.
			 *
			 */
			inline bool IsConstrained() const;


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
			 * Declare QuadEdge as a friend of this class.
			 */
			friend class QuadEdge;


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
			 * \fn void SetOwner(QuadEdge* qe)
			 *
			 * \brief Makes this edge part of a quad-edge.
			 *
			 * \param qe A pointer to a quad-edge.
			 *
			 */
			void 
				SetOwner(
					QuadEdge* qe
				)
			{
				if (qe == nullptr)
				{
					throw std::runtime_error("Attempt to access an unitialized quadedge");
				}

				_owner = qe;
			}


			/**
			 * \fn void SetId( unsigned id )
			 *
			 * \brief Assign an identifier to this edge.
			 *
			 * \param id An edge identifier (an integer from 0 to 3).
			 *
			 */
			void 
				SetId(
					unsigned id
				)
			{
				if (id > 3)
				{
					throw std::runtime_error("Attempt to access an edge of the triangulation with an invalid ID");
				}

				_id = id;
			}


			/**
			 * \fn void SetNext(Edge* e)
			 *
			 * \brief Assign a next edge to this edge.
			 *
			 * \param e A pointer to the next edge.
			 *
			 */
			void
				SetNext(
					Edge* e
				)
			{
				if (e == nullptr)
				{
					throw std::runtime_error("Attempt to assign a null pointer to an edge of the triangulation");
				}

				_next = e;
			}


			/**
			 * \fn void SetOrigin(Vertex* v)
			 *
			 * \brief Assign an origin vertex to this edge.
			 *
			 * \param v A pointer to a vertex.
			 *
			 */
			void
				SetOrigin(
					Vertex* v
				)
			{
				_orig = v;
			}


			/**
			 * \fn void SetDestination(Vertex* v)
			 *
			 * \brief Assign a destination vertex to this edge.
			 *
			 * \param v A pointer to a vertex.
			 *
			 */
			void
				SetDestination(
					Vertex* v
				)
			{
				Symmetric()->SetOrigin(v);
			}


			/**
			 * \fn void SetPosition(const std::list< Edge* >::iterator& it)
			 *
			 * \brief Assigns  a position for  this edge in a  list of
			 * edges of the triangulation it belongs to.
			 *
			 * \param it  An iterator position  in a list of  edges of
			 * the triangulation it belongs to.
			 *
			 */
			void
				SetPosition(
					const std::list<Edge*>::iterator& it
				)
			{
				_pos = it;
			}


			/**
			 * \fn void SetFace(Face* f)
			 *
			 * \brief Assign a face to this edge.
			 *
			 * \param f A pointer to a face.
			 *
			 */
			void
				SetFace(
					Face* f
				)
			{
				_face = f;
			}


			//
			// The implementation of SetConstrained() is in Quadedge.h
			//

			/**
			 * \fn inline void SetConstrained(bool value)
			 *
			 * \brief Assigns a Boolean  value to the constrained flag
			 * of this edge.
			 *
			 * \param value A Boolean value.
			 *
			 */
			inline
			void
				SetConstrained(
					bool value
				);

		};

	}

}

/** @} */ //end of group class.
