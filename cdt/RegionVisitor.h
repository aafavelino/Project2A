/**
 * \file RegionVisitor.h
 *
 * \brief Definition and implementation  of class RegionVisitor, which
 * is a visitor to identify a  maximal, connected set of bounded faces
 * of the CDT (i.e., triangles) bounded by constrained and/or boundary
 * edges.
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

#include "Visitor.h"    // Visitor
#include "Cdt.h"        // CDT
#include "Face.h"       // Face
#include "Quadedge.h"   // QuadEdge
#include "Edge.h"       // Edge
#include "Vertex.h"     // Vertex

#include <vector>       // std::vector
#include <list>         // std::list
#include <set>          // std::set
#include <utility>      // std::pair
#include <stdexcept>    // std::runtime_error

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
		 * \class RegionVisitor
		 *
		 * \brief  This class  consists of  a visitor  for identifying
		 * trimmed and  non-trimmed regions of a  Constrained Delaunay
		 * Triangulation (CDT).
		 *
		 */
		class RegionVisitor : public Visitor
		{
		public:

			// -------------------------------------------------------
			//
			// Type definitions
			//
			// -------------------------------------------------------

			/**
			 * \typedef TRIANGLESET
			 *
			 * \brief Defines a type for a set of triangles.
			 *
			 */
			typedef std::set<Face*> TRIANGLESET;


		private:

			// -------------------------------------------------------
			//
			// Private data members
			//
			// -------------------------------------------------------

			TRIANGLESET _visitedTriangles;  ///< A set of triangles visited by the visitor.
			bool _trimmedFlag;              ///< A flag to indicate whether the region to be visited is assumed to be trimmed.


		public:

			// -------------------------------------------------------
			//
			// Public methods
			//
			// -------------------------------------------------------

			/**
			 * \fn RegionVisitor()
			 *
			 * \brief Creates an instance of this class.
			 *
			 */
			RegionVisitor()
			{
			}


			/**
			 * \fn void Visit(CDT& cdt)
			 *
			 * \brief A  visitor method to  assign a Boolean  value to
			 * each triangle  of a single  region of the CDT,  where a
			 * region is a maximal, connected  set of bounded faces of
			 * the CDT,  which is bounded by  closed, polygonal chains
			 * of constrained and/or boundary edges.
			 *
			 * \param  cdt A  CDT  represented by  the quad-edge  data
			 * structure.
			 *
			 */
			void 
				Visit(
					CDT& cdt
				);


			/**
			 * \fn virtual void Visit(Face* f)
			 *
			 * \brief A method to visit a face.
			 *
			 * \param f A face of the quad-edge data structure.
			 *
			 */
			virtual 
			void
				Visit(
					Face* f
				)
			{
				// Make sure the face is bounded.
				if (!f->IsBounded())
				{
					throw std::runtime_error("Attempt to access an unbounded face when a bounded face is expected");
				}

				// Make sure face has not been visited before.
				if (_visitedTriangles.find(f) != _visitedTriangles.end())
				{
					throw std::runtime_error("Attempt to visit the same face for the second time");
				}

				// Mark this face as visited.
				f->SetMark(_trimmedFlag);

				std::pair<TRIANGLESET::iterator, bool> ret = _visitedTriangles.insert(f);

				if (!ret.second)
				{
					throw std::runtime_error("Attempt to insert the same face in a dictionary twice");
				}

				return;
			}


			/**
			 * \fn virtual void Visit(QuadEdge* qe)
			 *
			 * \brief  A visitor  method  to visit  a  quad-edge of  a
			 * Constrained   Delaunay  triangulation   based  on   the
			 * quad-edge data structure.
			 *
			 * \param qe A quad-edge of the quad-edge data structure.
			 *
			 */
			virtual 
			void 
				Visit(
					QuadEdge* qe
				)
			{
				return;
			}

			/**
			 * \fn virtual virtual void Visit(Edge* e)
			 *
			 * \brief A method to visit an edge.
			 *
			 * \param e An edge of the quad-edge data structure.
			 *
			 */
			virtual 
			void
				Visit(
					Edge* e
				)
			{
				return;
			}


			/**
			 * \fn virtual void Visit(Vertex* v)
			 *
			 * \brief A visitor method to visit a vertex.
			 *
			 * \param v A vertex of the quad-edge data structure.
			 *
			 */
			virtual 
			void
				Visit(
					Vertex* v
				)
			{
				return;
			}


		private:

			// -------------------------------------------------------
			//
			// Private methods
			//
			// -------------------------------------------------------

			/**
			 * \fn void VisitOuterTrimmedRegion(CDT& cdt)
			 *
			 * \brief Visits all outer, trimmed regions of the CDT.
			 *
			 * \param  cdt A  CDT  represented by  the quad-edge  data
			 * structure.
			 *
			 */
			void
				VisitOuterTrimmedRegion(
					CDT& cdt
				);


			/**
			 * \fn void VisitOuterNonTrimmedRegion(CDT& cdt)
			 *
			 * \brief  Visits all  outer, non-trimmed  regions of  the
			 * CDT.
			 *
			 * \param  cdt A  CDT  represented by  the quad-edge  data
			 * structure.
			 *
			 */
			void
				VisitOuterNonTrimmedRegion(
					CDT& cdt
				);


			/**
			 * \fn void VisitInteriorRegions(CDT& cdt)
			 *
			 * \brief Visits all interior regions of the CDT.
			 *
			 * \param  cdt A  CDT  represented by  the quad-edge  data
			 * structure.
			 *
			 */
			void 
				VisitInteriorRegions(
					CDT& cdt
				);


			/**
			 * \fn void CollectConstrainedBoundaryEdges(CDT& cdt, std::list<Edge*>& listOfConstrainedBoundaryEdges) const
			 *
			 * \brief  Collects all  constrained  boundary edges  that
			 * belong to bounded faces of a given CDT.
			 *
			 * \param  cdt A  CDT  represented by  the quad-edge  data
			 * structure.
			 * \param listOfConstrainedBoundaryEdges A  reference to a
			 * list of edges.
			 *
			 */
			void 
				CollectConstrainedBoundaryEdges(
					CDT& cdt,
					std::list<Edge*>& listOfConstrainedBoundaryEdges
				)
				const;


			/**
			 * \fn void CollectUnconstrainedBoundaryEdges(CDT& cdt, std::list<Edge*>& listOfUnconstrainedBoundaryEdges) const
			 *
			 * \brief Collects  all unconstrained boundary  edges that
			 * belong to bounded faces of a given CDT.
			 *
			 * \param  cdt A  CDT  represented by  the quad-edge  data
			 * structure.
			 * \param listOfUnconstrainedBoundaryEdges  A reference to
			 * a list of edges.
			 *
			 */
			void 
				CollectUnconstrainedBoundaryEdges(
					CDT& cdt,
					std::list<Edge*>& listOfUnconstrainedBoundaryEdges
				)
				const;


			/**
			 * \fn void CollectConstrainedInteriorEdges(CDT& cdt, std::list<Edge*>& listOfConstrainedInteriorEdges) const
			 *
			 * \brief Collects  all constrained,  interior edges  of a
			 * given CDT.
			 *
			 * \param  cdt A  CDT  represented by  the quad-edge  data
			 * structure.
			 * \param listOfConstrainedInteriorEdges A  reference to a
			 * list of edges.
			 *
			 */
			void 
				CollectConstrainedInteriorEdges(
					CDT& cdt,
					std::list<Edge*>& listOfConstrainedInteriorEdges
				)
				const;


			/**
			 * \fn void VisitFacesInRegion(Face* f)
			 *
			 * \brief Visits all  faces in the same region  of a given
			 * face.
			 *
			 * \param f A pointer to a  face that has not been visited
			 * yet.
			 *
			 */
			void 
				VisitFacesInRegion(
					Face* f
				);

		};

	}

}

/** @} */ //end of group class.
