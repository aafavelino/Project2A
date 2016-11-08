/**
 * \file FaceVisitor.h
 *
 * \brief Definition and implementation of class FaceVisitor, which is
 * a visitor that creates lists of all faces, edges, and vertices of a
 * CDT.
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

#include "Visitor.h"          // Visitor
#include "Cdt.h"              // CDT
#include "Face.h"             // Face
#include "Quadedge.h"         // QuadEdge
#include "Edge.h"             // Edge
#include "Vertex.h"           // Vertex

#include <map>                // map
#include <functional>         // make_pair
#include <vector>             // vector
#include <list>               // list
#include <stdexcept>          // std::runtime_error
#include <memory>             // std::shared_ptr


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

	namespace cdt {

		/**
		 * \class FaceVisitor
		 *
		 * \brief This class is a visitor for enumerating the lists of
		 * all  faces, edges,  and  vertices  of a  CDT  based on  the
		 * quad-edge data structure.
		 *
		 */
		class FaceVisitor : public Visitor {
		public:

			// -------------------------------------------------------
			//
			// Type definitions
			//
			// -------------------------------------------------------

			/**
			 * \enum EdgeType
			 *
			 * \brief A type for edge status.
			 *
			 */
			enum class EdgeType
			{
			    Regular,       ///< An interior, unconstrained edge of the CDT.
				Constrained,   ///< A constrained edge (interior or boundary) of the CDT.
				Boundary       ///< A boundary, unconstrained edge of the CDT.
			};

			/**
			 * \typedef VTAB
			 *
			 * \brief  Defines a  type  for a  vertex identifier  hash
			 * table.
			 */
			typedef std::map<Vertex*, size_t> VTAB;

			/**
			 * \typedef spPoint
			 *
			 * \brief Alias for a shared pointer.
			 *
			 */
			typedef Vertex::spPoint spPoint;

		    /**
			 * \typedef PointList
			 *
			 * \brief Alias for an array of vertex point attributes.
			 *
			 */
			typedef std::vector<spPoint> PointList;


		private:

			// ------------------------------------------------------
			//
			// Private data members
			//
			// ------------------------------------------------------

			VTAB _vertexList;              ///< A temporary list of vertex coordinates.
			std::vector<Edge*> _edgeList;  ///< A temporary list of edge vertex identifiers.
			std::vector<Face*> _faceList;  ///< A temporary list of face vertex identifiers.


		public:

			// ------------------------------------------------------
			//
			// Public methods
			//
			// ------------------------------------------------------

			/**
			* \fn FaceVisitor()
			*
			* \brief Creates an instance of this class.
			*
			*/
			FaceVisitor()
			{}


			/**
			 * \fn void Visit(CDT& cdt, size_t& numberOfTriangles, std::vector<size_t>& triangles, size_t& numberOfEdges, std::vector<size_t>& edges, std::vector<EdgeType>& edgeType, size_t& numberOfVertices, PointList& vertices)
			 *
			 * \brief A generic visitor  method to enumerate the lists
			 * of all  faces, edges,  and vertices of a CDT.
			 *
			 * \param  cdt A  CDT  represented by  the quad-edge  data
			 * structure.
			 * \param numberOfTriangles  Total number of  triangles of
			 * the CDT.
			 * \param triangles  A reference  to an array  with vertex
			 * indices of all triangles.
			 * \param  numberOfEdges  Total  number of  edges  in  the
			 * triangulation.
			 * \param edges  A reference to  an array to  store vertex
			 * indices.
			 * \param edgeType  A reference to  an array to  store the
			 * type of each edge.
			 * \param numberOfVertices Total number of vertices of the
			 * triangulation.
			 * \param vertices  A reference to  an array to  store the
			 * point attributes of all vertices of the triangulation.
			 *
			 */
			void
				Visit(
					CDT& cdt,
					size_t& numberOfTriangles,
					std::vector<size_t>& triangles,
					size_t& numberOfEdges,
					std::vector<size_t>& edges,
					std::vector<EdgeType>& edgeType,
					size_t& numberOfVertices,
					PointList& vertices
				);


			/**
			 * \fn virtual void Visit( Face* f )
			 *
			 * \brief A generic visitor method to visit a face.
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
				// Get the three edges of the current face.
				Edge* e1 = f->GetEdge();

#ifdef DEBUGMODE
				//
				// Make sure the pointer is valid.
				//
				if ((e1 == nullptr) || (e1->GetFace() != f))
				{
					throw std::runtime_error("Quadedge has inconsistent information");
				}
#endif

				Edge* e2 = e1->LeftNext();

#ifdef DEBUGMODE
				//
				// Make sure the pointer is valid.
				//
				if ((e2 == nullptr) || (e2->GetFace() != f))
				{
					throw std::runtime_error("Quadedge has inconsistent information");
				}
#endif

				Edge* e3 = e2->LeftNext();

#ifdef DEBUGMODE
				//
				// Make sure the pointer is valid.
				//
				if ((e3 == nullptr) || (e3->GetFace() != f) || (e3->LeftNext() != e1))
				{
					throw std::runtime_error("Quadedge has inconsistent information");
				}
#endif

				// Visit the edges that have not been visited before.
				if (!e1->GetOwner()->IsMarked()) {
					e1->Accept(this);
				}

				if (!e2->GetOwner()->IsMarked()) {
					e2->Accept(this);
				}

				if (!e3->GetOwner()->IsMarked()) {
					e3->Accept(this);
				}

				// Store the face in the face list.
				_faceList.push_back(f);

				return;
			}


			/**
			 * \fn virtual void Visit ( QuadEdge* qe )
			 *
			 * \brief A generic visitor method to visit a quad-edge of a CDT.
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
				//
				// Do nothing --- it is never called by a method of this class.
				//

				return;
			}


			/**
			 * \fn virtual void Visit( Edge* e )
			 *
			 * \brief A generic visitor method to visit an edge of a CDT.
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
				// Visit the two vertices incident to this edge.
				e->GetOrigin()->Accept(this);
				e->GetDestination()->Accept(this);

				// Store the edge in the edge list.
				_edgeList.push_back(e);

				// Mark the quad-edge containing this edge as visited.
				e->GetOwner()->SetMark(true);

				return;
			}


			/**
			* \fn virtual void Visit(Vertex* v)
			*
			* \brief A generic visitor method to visit a vertex.
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
				VTAB::iterator vit = _vertexList.find(v);

				if (vit == _vertexList.end()) {
					_vertexList.insert(std::make_pair(v, _vertexList.size()));
				}

				return;
			}

		};

	}

}

/** @} */ //end of group class.
