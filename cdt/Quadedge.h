/**
 * \file Quadedge.h
 *
 * \brief Definition  and implementation of the  class QuadEdge, which
 * represents  two pairs  of  symmetric edges  of  the quad-edge  data
 * structure.
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
		 * \class QuadEdge
		 *
		 * \brief This  class represents two pairs  of symmetric edges
		 * of the quad-edge data structure.
		 *
		 */
		class QuadEdge
		{
		protected:

			// -------------------------------------------------------
			//
			// Protected data members
			//
			// -------------------------------------------------------

			Edge _edges[4];     ///< Array with the two pairs of symmetric edges that define this quad-edge.
			bool _marked;       ///< A flag to be used by visitor-type of algorithms.
			bool _constrained;  ///< A flag to indicate whether this quad-edge is constrained.


		public:

			// -------------------------------------------------------
			//
			// Public methods
			//
			// -------------------------------------------------------

			/**
			 * \fn QuadEdge()
			 *
			 * \brief Creates an instance of this class.
			 *
			 */
			QuadEdge()
			{
				// Set  up the  id,  quad-edge,  and constrained  flag
				// vertex of each edge.
				for (unsigned int i = 0; i <= 3; i++)
				{
					_edges[i].SetOwner(this);
					_edges[i].SetId(i);
				}

				// Initialize the  \c m_next field of  the four edges.
				// Recall that the two symmetric edges of the dual are
				// next of  each other,  while the symmetric  edges of
				// the primal are next of themselves.
				_edges[0].SetNext(&(_edges[0]));   // primal edge
				_edges[2].SetNext(&(_edges[2]));   // prinal edge

				_edges[1].SetNext(&(_edges[3]));   // dual edge
				_edges[3].SetNext(&(_edges[1]));   // dual edge

				// Set the mark flag to false.
				SetMark(false);

				// Set the constrained flag to false.
				SetConstrained(false);

				return;
			}


			/**
			 * \fn virtual ~QuadEdge()
			 *
			 * \brief Releases the memory held  by an instance of this
			 * class.
			 *
			 */
			virtual ~QuadEdge()
			{
			}


			/**
			 * \fn Edge* GetEdge(unsigned int i)
			 *
			 * \brief Returns the i-th edge of this quad-edge.
			 *
			 * \param i An edge index.
			 *
			 * \return The i-th edge of this quad-edge.
			 *
			 */
			Edge* 
				GetEdge(
					unsigned int i
				)
			{
				if (i > 3)
				{
					throw std::runtime_error("Attempt to access an edge of the triangulation with an invalid ID");
				}

				return &(_edges[i]);
			}


			/**
			 * \fn bool IsConstrained() const
			 *
			 * \brief Returns the Boolean value true if this quad-edge
			 * is a constrained quad-edge, and the Boolean value false
			 * otherwise.
			 *
			 * \return The  Boolean value  true if  the edges  of this
			 * quad-edge are constrained edges,  and the Boolean value
			 * false otherwise.
			 *
			 */
			bool IsConstrained() const
			{
				return _constrained;
			}


			/**
			 * \fn void SetConstrained(bool value)
			 *
			 * \brief Assigns a Boolean  value to the constrained flag
			 * of the four edges associated with this quad-edge.
			 *
			 * \param value A Boolean value.
			 *
			 */
			void 
				SetConstrained(
					bool value
				)
			{
				_constrained = value;

				return;
			}


			/**
			 * \fn void SetMark(bool value)
			 *
			 * \brief Assigns a Boolean value to the mark flag of this
			 * quad-edge.
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
			 * \brief Returns the Boolean value true if this quad-edge
			 * has been marked, and the Boolean value false otherwise.
			 *
			 * \return The  Boolean value  true if this  quad-edge has
			 * been marked, and the Boolean value false otherwise.
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

		};


		/**
		 * \fn inline void Edge::SetConstrained(bool value)
		 *
		 * \brief Assigns a  Boolean value to the  constrained flag of
		 * this edge.
		 *
		 * \param value A Boolean value.
		 *
		 */
		inline
		void
			Edge::SetConstrained(
				bool value
			)
		{
			_owner->SetConstrained(value);
		}


		/**
		 * \fn inline bool Edge::IsConstrained() const
		 *
		 * \brief Returns  the Boolean  value true if  this edge  is a
		 * constrained edge, and the Boolean value false otherwise.
		 *
		 * \return  The  Boolean  value  true if  the  edges  of  this
		 * quad-edge  are constrained  edges,  and  the Boolean  value
		 * false otherwise.
		 *
		 */
		inline
		bool
			Edge::IsConstrained() const
		{
			return _owner->IsConstrained();
		}

	}

}

/** @} */ //end of group class.
