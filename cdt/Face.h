/**
 * \file Face.h
 *
 * \brief  Definition  and implementation  of  the  class Face,  which
 * represents a  bounded face in a  constrained Delaunay triangulation
 * (CDT).
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
#include "Edge.h"      // Edge

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


		/**
		 * \class Face
		 *
		 * \brief This class represents a face in a CDT or its dual.
		 */
		class Face
		{
		public:

			// -------------------------------------------------------
			//
			// Type definitions
			//
			// -------------------------------------------------------

			/**
			 * \typedef FaceList
			 *
			 * \brief Definition of  a type for a list  of pointers to
			 * faces.
			 */
			typedef std::list<Face*> FaceList;


			/**
			 * \typedef FaceIterator
			 *
			 * \brief Definition of a type for a face list iterator.
			 */
			typedef FaceList::iterator FaceIterator;


		protected:

			// -------------------------------------------------------
			//
			// Protected data members
			//
			// -------------------------------------------------------

			Edge* _edge;            ///< A pointer to an edge of this face.
			bool _bounded;          ///< A flag to indicate whether this face is bounded.
			FaceIterator _pos;		///< Position of this face in a list of faces of the CDT.
			bool _marked;           ///< A flag to be used by visitor-type of algorithms.

		public:

			// -------------------------------------------------------
			//
			// Public methods
			//
			// -------------------------------------------------------

			/**
			 * \fn Face()
			 *
			 * \brief Creates an instance of this class.
			 *
			 */
			Face() : _edge(nullptr), _bounded(false)
			{
				SetMark(false);
			}


			/**
			 * \fn virtual ~Face()
			 *
			 * \brief Releases the memory held  by an instance of this
			 * class.
			 *
			 */
			virtual ~Face()
			{
			}


			/**
			 * \fn Edge* GetEdge() const
			 *
			 * \brief Returns a  pointer to an edge  belonging to this
			 * face.
			 *
			 * \return A pointer to an edge of this face.
			 *
			 */
			Edge* GetEdge() const
			{
				return _edge;
			}


			/**
			 * \fn bool IsBounded() const
			 *
			 * \brief Returns the  Boolean value true if  this face is
			 * bounded. Otherwise, it returns the Boolean value false.
			 *
			 * \return  The  Boolean  value   true  if  this  face  is
			 * bounded.   Otherwise,  the   Boolean  value   false  is
			 * returned.
			 *
			 */
			bool IsBounded() const
			{
				return _bounded;
			}


			/**
			 * \fn FaceIterator GetPosition() const
			 *
			 * \brief Returns the  position of this face in  a list of
			 * faces of the triangulation it belong to.
			 *
			 * \return The position of this face  in a list of face of
			 * the triangulation it belong to.
			 *
			 */
			FaceIterator GetPosition() const
			{
				return _pos;
			}


			/**
			 * \fn void SetMark(bool value)
			 *
			 * \brief Assigns a Boolean value to the mark flag of this
			 * face.
			 *
			 * \param value A Boolean value.
			 *
			 */
			void
				SetMark(
					bool value
				)
			{
				_marked = value;
			}


			/**
			 * \fn bool IsMarked() const
			 *
			 * \brief Returns the Boolean value  true if this face has
			 * been marked, and the Boolean value false otherwise.
			 *
			 * \return The  Boolean value true  if this face  has been
			 * marked, and the Boolean value false otherwise.
			 *
			 */
			bool IsMarked() const
			{
				return _marked;
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


			//
			// Declare CDT as a friend of this class.
			//
			friend class CDT;


		protected:

			// -------------------------------------------------------
			//
			// Protected methods
			//
			// -------------------------------------------------------


			/**
			 * \fn void SetEdge(Edge* e)
			 *
			 * \brief Assign an edge to this face.
			 *
			 * \param e A pointer to an edge.
			 *
			 */
			void 
				SetEdge(
					Edge* e
				)
			{
				if (e == nullptr)
				{
					throw new std::runtime_error("Attempt to assign null pointer to an edge");
				}

				_edge = e;
			}


			/**
			 * \fn void SetBounded(bool value)
			 *
			 * \brief Assigns a  Boolean value to the  bounded flag of
			 * this face.
			 *
			 * \param value A Boolean value.
			 *
			 */
			void 
				SetBounded(
					bool value
				)
			{
				_bounded = value;
			}


			/**
			 * \fn void SetPosition(const FaceIterator& it)
			 *
			 * \brief Assigns  a position for  this face in a  list of
			 * faces of the triangulation it belong to.
			 *
			 * \param it  An iterator position  in a list of  edges of
			 * the triangulation it belongs to.
			 *
			 */
			void 
				SetPosition(
					const FaceIterator& it
				)
			{
				_pos = it;
			}

		};

	}

}

/** @} */ //end of group class.
