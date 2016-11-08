/**
 * \file TriangulationVisitor.cpp
 *
 * \brief  Implementation of  class TriangulationVisitor,  which is  a
 * visitor to collect all (bounded)  triangles, edges, and vertices of
 * the  non-trimmed regions  of a  Constrained Delaunay  Triangulation
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

#include "TriangulationVisitor.h"     // TriangulationVisitor

#include <stdexcept>                  // std::runtime_error


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
		 * \fn void TriangulationVisitor::Visit(CDT& cdt, size_t& numberOfTriangles, std::vector<size_t>& triangles, size_t& numberOfEdges, std::vector<size_t>& edges, std::vector<EdgeType>& edgeType, size_t& numberOfVertices, PointList& vertices)
		 *
		 * \brief A generic  visitor method to enumerate  the lists of
		 * all  faces, edges,  and  vertices of  the non-trimmed  away
		 * regions of a CDT.
		 *
		 * \param  cdt  A  CDT   represented  by  the  quad-edge  data
		 * structure.
		 * \param numberOfTriangles  Total number of triangles  of the
		 * CDT.
		 * \param  triangles  A  reference  to an  array  with  vertex
		 * indices of all triangles.
		 * \param  numberOfEdges   Total  number   of  edges   in  the
		 * triangulation.
		 * \param  edges  A reference  to  an  array to  store  vertex
		 * indices.
		 * \param edgeType A  reference to an array to  store the type
		 * of each edge.
		 * \param  numberOfVertices Total  number of  vertices of  the
		 * triangulation.
		 * \param vertices A reference to an array to store the point
		 * attributes of all vertices of the triangulation.
		 *
		 */
		void
			TriangulationVisitor::Visit(
				CDT& cdt,
				size_t& numberOfTriangles,
				std::vector<size_t>& triangles,
				size_t& numberOfEdges,
				std::vector<size_t>& edges,
				std::vector<EdgeType>& edgeType,
				size_t& numberOfVertices,
				PointList& vertices
			)
		{
			// Visit all outer trimmed away regions.
			RegionVisitor regionVisitor;

			// Mark all faces
			for (CDT::FaceIterator fit = cdt.FacesBegin(); fit != cdt.FacesEnd(); ++fit)
			{
				Face* f = *fit;
				f->SetMark(true);
			}

			regionVisitor.Visit(cdt);

			// Creates temporary  lists of vertices, edges,  and faces
			// of this CDT.
			_faceList.clear();
			_edgeList.clear();
			_vertexList.clear();

			// Loop over all faces of  the non-trimmed away regions of
			// the CDT.
			for (CDT::FaceIterator fit = cdt.FacesBegin(); fit != cdt.FacesEnd(); ++fit)
			{
				// Visit the current face.
				Face* f = *fit;

				if (f->IsBounded() && !f->IsMarked())
				{
					f->Accept(this);
				}
			}

			// Get the number of vertices of the CDT.
			numberOfVertices = _vertexList.size();

			// Allocate memory for the vertex coordinates.
			vertices.resize(numberOfVertices);

			// Get the coordinates of all vertices.
			VTAB::const_iterator vit;

			for (vit = _vertexList.begin(); vit != _vertexList.end(); ++vit)
			{
				size_t i = vit->second;
				vertices[i] = vit->first->GetPoint();
			}

			// Get the number of edges of the CDT.
			numberOfEdges = _edgeList.size();

			// Allocate memory for the indices of the edge vertices.
			edges.resize(2 * numberOfEdges);

			// Allocate memory to store the type of the edges.
			edgeType.resize(numberOfEdges);

			// Get the index of the two vertices of each edge.
			for (size_t i = 0; i < numberOfEdges; i++)
			{
				Vertex* p1 = _edgeList[i]->GetOrigin();
				vit = _vertexList.find(p1);

#ifdef DEBUGMODE
				if (vit == _vertexList.end())
				{
					throw std::runtime_error("Attempt to access a vertex with an invalid index");
				}
#endif

				edges[2 * i] = vit->second;

				Vertex* p2 = _edgeList[i]->GetDestination();
				vit = _vertexList.find(p2);

#ifdef DEBUGMODE
				if (vit == _vertexList.end())
				{
					throw std::runtime_error("Attempt to access a vertex with an invalid index");
				}
#endif

				edges[2 * i + 1] = vit->second;

				if (_edgeList[i]->IsConstrained())
				{
					edgeType[i] = EdgeType::Constrained;
				}
				else if (_edgeList[i]->Symmetric()->GetFace()->IsBounded())
				{
					edgeType[i] = EdgeType::Regular;
				}
				else
				{
					edgeType[i] = EdgeType::Boundary;
				}

				// Reset the  marked flag of the  quad-edge containing
				// the edge.
				_edgeList[i]->GetOwner()->SetMark(false);
			}

			// Get the number of faces of the CDT.
			numberOfTriangles = _faceList.size();

			// Allocate memory for the vertex coordinates.
			triangles.resize(3 * numberOfTriangles);

			// Get the index of the three vertices of each face.
			for (size_t i = 0; i < numberOfTriangles; i++)
			{
				Vertex* v1 = _faceList[i]->GetEdge()->GetOrigin();
				Vertex* v2 = _faceList[i]->GetEdge()->LeftNext()->GetOrigin();
				Vertex* v3 = _faceList[i]->GetEdge()->LeftPrev()->GetOrigin();

				vit = _vertexList.find(v1);

#ifdef DEBUGMODE
				if (vit == _vertexList.end())
				{
					throw std::runtime_error("Attempt to access a vertex with an invalid index");
				}
#endif

				triangles[3 * i] = vit->second;
				vit = _vertexList.find(v2);

#ifdef DEBUGMODE   
				if (vit == _vertexList.end())
				{
					throw std::runtime_error("Attempt to access a vertex with an invalid index");
				}
#endif

				triangles[3 * i + 1] = vit->second;
				vit = _vertexList.find(v3);

#ifdef DEBUGMODE
				if (vit == _vertexList.end())
				{
					throw std::runtime_error("Attempt to access a vertex with an invalid index");
				}
#endif

				triangles[3 * i + 2] = vit->second;
			}

			// Clean up the temporary vertex and edge lists.
			_vertexList.clear();
			_edgeList.clear();
			_faceList.clear();

			// Reset face marks.
			for (CDT::FaceIterator fit = cdt.FacesBegin(); fit != cdt.FacesEnd(); ++fit)
			{
				Face* f = *fit;
				f->SetMark(false);
			}

			return;
		}

	}

}

/** @} */ //end of group class.
