/**
 * \file Cdt.h
 *
 * \brief  Definition  and  implementation  of the  class  CDT,  which
 * represents a Constrained Delaunay  Triangulation (CDT for short) of
 * a Planar  Straight-Line Graph  (PSLG) based  on the  quad-edge data
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

#include "DtPoint.h"       // DtPoint
#include "Visitor.h"       // Visitor
#include "Vertex.h"        // Vertex
#include "Edge.h"          // Edge
#include "Quadedge.h"      // QuadEdge
#include "Face.h"          // Face
#include "Predicates.h"    // Predicates<double>

#include <vector>          // std::vector
#include <list>            // std::list
#include <memory>          // std::shared_ptr


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
		 * \class CDT
		 *
		 * \brief  This   class  represents  a   Constrained  Delaunay
		 * triangulation (CDT)  of a  PSLG in  \f$E^2\f$ based  on the
		 * quad-edge data structure.
		 */
		class CDT {
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
			typedef Vertex::VertexList VertexList;


			/**
			 * \typedef VertexIterator
			 *
			 * \brief Definition of a type for a vertex list iterator.
			 *
			 */
			typedef VertexList::const_iterator VertexIterator;


			/**
			 * \typedef EdgeList
			 *
			 * \brief Definition of  a type for a list  of pointers to
			 * edges.
			 *
			 */
			typedef Edge::EdgeList EdgeList;


			/**
			 * \typedef EdgeIterator
			 *
			 * \brief Definition of a type for an edge list iterator.
			 *
			 */
			typedef EdgeList::const_iterator EdgeIterator;


			/**
			 * \typedef FaceList
			 *
			 * \brief Definition of  a type for a list  of pointers to
			 * faces.
			 *
			 */
			typedef Face::FaceList FaceList;


			/**
			 * \typedef FaceIterator
			 *
			 * \brief Definition of a type for a face list iterator.
			 *
			 */
			typedef FaceList::const_iterator FaceIterator;


			/**
			 * \typedef spPoint
		 	 *
			 * \brief Alias for a shared pointer to vertex point attributes.
			 *
			 */
			typedef Vertex::spPoint spPoint;


		protected:

			// -------------------------------------------------------
			//
			// Protected data members
			//
			// -------------------------------------------------------

			Edge* _sedge;                 ///< An edge of this triangulation.
			double _max;                  ///< Maximum absolute value among the coordinates of a vertex of the enclosing triangle.
			VertexList _lverts;           ///< List of vertices of this CDT.
			EdgeList _ledges;             ///< List of edges of this CDT.
			FaceList _lfaces;             ///< List of faces of this CDT.
			Predicates< double > _preds;  ///< A set of robust geometric predicates.


		public:

			// -------------------------------------------------------
			//
			// Public methods
			//
			// -------------------------------------------------------


		    /**
			 * \fn CDT(size_t numberOfPoints, const std::vector<spPoint>& points)
			 *
			 * \brief  Creates   a  instance  of  this   class,  which
			 * represents a Delaunay triangulation of a set of points.
			 *
			 * \param numberOfPoints The number of points.
			 * \param points An array with the point coordinates.
			 *
			 */
			CDT(
				size_t numberOfPoints,
				const std::vector<spPoint>& points
			);
			

			/**
			 * \fn CDT(size_t numberOfPoints, size_t numberOfSegments, const std::vector<spPoint>& points, const std::vector<size_t>& segments)
			 *
			 * \brief  Creates   a  instance  of  this   class,  which
			 * represents  a  CDT of  a  set  of points  and  segments
			 * defining a Planar Straight-Line Graph (PSLG).
			 *
			 * \param numberOfPoints The number of points.
			 * \param numberOfSegments The number of segments.
			 * \param points An array with the point coordinates.
			 * \param  segments An  array  with the  ID's  of the  two
			 * vertices of each segment.
			 *
			 */
			CDT(
				size_t numberOfPoints,
				size_t numberOfSegments,
				const std::vector<spPoint>& points,
				const std::vector<size_t>& segments
			);


			/**
			 * \fn virtual ~CDT()
			 *
			 * \brief Releases the memory held by an instance of this class.
			 *
			 */
			virtual ~CDT();


		    /**
			 * \fn bool CheckInputConsistency(size_t numberOfPoints, const std::vector<spPoint>& points)
			 *
			 * \brief  Determines  whether  the input  is  consistent.
			 * This amounts to  verifying if not all  given points are
			 * collinear and  if no given line  segment intersects the
			 * interior of another given line segment.
			 *
			 * \param numberOfPoints The number of points.
			 * \param points The point coordinates.
			 *
			 * \return  The   logic  value   true  if  the   input  is
			 * consistent, and the logic value false otherwise.
			 *
			 */
			bool
				CheckInputConsistency(
					size_t numberOfPoints,
					const std::vector<spPoint>& points
				);


			/**
			 * \fn bool CheckInputConsistency(size_t numberOfPoints, size_t numberOfSegments, const std::vector<spPoint>& points, const std::vector<size_t>& segments)
			 *
			 * \brief  Determines  whether  the input  is  consistent.
			 * This amounts to  verifying if not all  given points are
			 * collinear and  if no given line  segment intersects the
			 * interior of another given line segment.
			 *
			 * \param numberOfPoints The number of points.
			 * \param numberOfSegments The number of segments.
			 * \param points The point coordinates.
			 * \param segments  The ID's of  the two vertices  of each
			 * segment.
			 *
			 * \return  The   logic  value   true  if  the   input  is
			 * consistent, and the logic value false otherwise.
			 *
			 */
			bool
				CheckInputConsistency(
					size_t numberOfPoints,
					size_t numberOfSegments,
					const std::vector<spPoint>& points,
					const std::vector<size_t>& segments
				);


			/**
			 * \fn Edge* GetStartingEdge() const
			 *
			 * \brief Returns the starting edge of this CDT.
			 *
			 * \return The starting edge of this CDT.
			 *
			 */
			Edge* GetStartingEdge() const
			{
				return _sedge;
			}


			/**
			 * \fn double GetLargestAbsoluteValueOfCoordinate() const
			 *
			 * \brief  Returns  the  absolute  value  of  the  largest
			 * coordinate of a vertex of the enclosing triangle.
			 *
			 * \return The absolute value of the largest coordinate of
			 * a vertex of the enclosing triangle.
			 *
			 */
			double GetLargestAbsoluteValueOfCoordinate() const
			{
				return _max;
			}


			/**
			 * \fn size_t GetNumberOfVertices() const
			 *
			 * \brief Returns the number of vertices of this CDT.
			 *
			 * \return The number of vertices of this CDT.
			 *
			 */
			size_t GetNumberOfVertices() const
			{
				return _lverts.size();
			}


			/**
			 * \fn size_t GetNumberOfEdges() const
			 *
			 * \brief Returns the number of edges of this CDT.
			 *
			 * \return The number of edges of this CDT.
			 *
			 */
			size_t GetNumberOfEdges() const
			{
				return _ledges.size() >> 1;
			}


			/**
			 * \fn size_t GetNumberOfFaces() const
			 *
			 * \brief  Returns  the  number  of  faces  of  this  CDT,
			 * including the unbounded face.
			 *
			 * \return The number of faces  of this CDT (including the
			 * unbounded).
			 *
			 */
			size_t GetNumberOfFaces() const
			{
				return _lfaces.size();
			}


			/**
			 * \fn VertexIterator VertsBegin() const
			 *
			 * \brief Returns  a point iterator pointing  to the first
			 * vertex of the list of vertices of this CDT.
			 *
			 * \return  A point  list iterator  pointing to  the first
			 * vertex of the list of vertices of this CDT.
			 *
			 */
			VertexIterator VertsBegin() const
			{
				return _lverts.begin();
			}


			/**
			 * \fn EdgeIterator EdgesBegin() const
			 *
			 * \brief Returns  an edge iterator pointing  to the first
			 * edge of the list of edges of this CDT.
			 *
			 * \return  An edge  list iterator  pointing to  the first
			 * edge of the list of edges of this CDT.
			 *
			 */
			EdgeIterator EdgesBegin() const
			{
				return _ledges.begin();
			}


			/**
			 * \fn FaceIterator FacesBegin() const
			 *
			 * \brief Returns  a face  iterator pointing to  the first
			 * face of the list of faces of this CDT.
			 *
			 * \return A face list iterator pointing to the first face
			 * of the list of faces of this CDT.
			 *
			 */
			FaceIterator FacesBegin() const
			{
				return _lfaces.begin();
			}


			/**
			 * \fn VertexIterator VertsEnd() const
			 *
			 * \brief Returns  a vertex iterator pointing  to the last
			 * vertex of the list of vertices of this CDT.
			 *
			 * \return A  vertex list iterator pointing  past the last
			 * vertex of the list of vertices of this CDT.
			 *
			 */
			VertexIterator VertsEnd() const
			{
				return _lverts.end();
			}


			/**
			 * \fn inline EdgeIterator EdgesEnd() const
			 *
			 * \brief Returns an edge  iterator pointing past the last
			 * edge of the list of edges of this CDT.
			 *
			 * \return An  edge list  iterator pointing past  the last
			 * edge of the list of edges this CDT.
			 *
			 */
			EdgeIterator EdgesEnd() const
			{
				return _ledges.end();
			}


			/**
			 * \fn inline FaceIterator FacesEnd() const
			 *
			 * \brief Returns  a face iterator pointing  past the last
			 * face of the list of faces of this CDT.
			 *
			 * \return  A face  list iterator  pointing past  the last
			 * face of the list of faces of this CDT.
			 *
			 */
			FaceIterator FacesEnd() const
			{
				return _lfaces.end();
			}


		protected:

			// -------------------------------------------------------
			//
			// Protected methods
			//
			// -------------------------------------------------------

			/**
			 * \fn void SetStartingEdge(Edge* e)
			 *
			 * \brief  Assigns  an  address  to  the  pointer  to  the
			 * starting edge of this triangulation.
			 *
			 * \param e The address of an edge.
			 *
			 */
			void
				SetStartingEdge(
					Edge* e
				)
			{
				_sedge = e;
			}


			/**
			 * \fn void SetLargestAbsoluteValueOfCoordinate(double val)
			 *
			 * \brief  Set a  value  to the  largest  coordinate of  a
			 * vertex of the enclosing triangle.
			 *
			 * \param  val  Maximum  absolute valute  of  the  largest
			 * coordinate of a vertex of the enclosing triangle.
			 *
			 */
			void
				SetLargestAbsoluteValueOfCoordinate(
					double val
				)
			{
				_max = val;
			}


			/**
			 * \fn void CreateEnclosingTriangle(spPoint pa, spPoint pb, spPoint pc)
			 *
			 * \brief Creates  an enclosing triangle for  the vertices
			 * of this CDT.   This triangle will be  removed after the
			 * triangulation is built.
			 *
			 * \param pa First  vertex of the enclosing  triangle in a
			 * CCW traverse.
			 * \param pb Second vertex of  the enclosing triangle in a
			 * CCW traverse.
			 * \param pc Third  vertex of the enclosing  triangle in a
			 * CCW traverse.
			 *
			 */
			void
				CreateEnclosingTriangle(
					spPoint pa,
					spPoint pb,
					spPoint pc
				);



			/**
			 * \fn void InsertPoint(spPoint p)
			 *
			 * \brief Insert a new vertex into the current CDT.
			 *
			 * \param p A pointer to the vertex location.
			 *
			 */
			void 
				InsertPoint(
					spPoint p
				);


			/**
			 * \fn void InsertSegment(spPoint p1, spPoint p2)
			 *
			 * \brief Insert a segment (i.e., a constrained edge) into
			 * the CDT.
			 *
			 * \param p1 An endpoint of the segment.
			 * \param p2 Another endpoint of the segment.
			 *
			 */
			void
				InsertSegment(
					spPoint p1,
					spPoint p2
				);


			/**
			 * \fn void RemoveEnclosingTriangle()
			 *
			 * \brief Remove the enclosing  triangle and all triangles
			 * adjacent  to  each  one   of  its  vertices  (as  these
			 * triangles do  not belong to the  Delaunay triangulation
			 * of the given input set of points).
			 *
			 */
			void RemoveEnclosingTriangle();


			/**
			 * \fn Edge* Locate(spPoint p)
			 *
			 * \brief Search for  an edge that contains  a given point
			 * or belongs to a triangle  that contains the given point
			 * in its  interior.  The search starts  with the starting
			 * edge of this CDT.
			 *
			 * \param p A given point.
			 *
			 * \return A pointer to an edge of this CDT.
			 *
			 */
			Edge* 
				Locate(
					spPoint p
				);


			/**
			 * \fn void SplitEdgeOrTriangle(spPoint p, Edge*& e)
			 *
			 * \brief Insert a vertex inside  a triangle or on an edge
			 * of   the  current   triangulation,   producing  a   new
			 * triangulation which may not be a CDT.
			 *
			 * \param p  A pointer  to the  point associated  with the
			 * vertex to be inserted into the triangulation.
			 * \param e A pointer to an edge of the current CDT.
			 *
			 */
			void 
				SplitEdgeOrTriangle(
					spPoint p,
					Edge*& e
				);


			/**
			 * \fn void RestoreDelaunayProperty(spPoint p, Edge* e)
			 *
			 * \brief  If   the  insertion   of  a  point   makes  the
			 * triangulation no longer be  Delaunay then swap edges to
			 * make the trinagulation into a CDT.
			 *
			 * \param  p A  pointer  to the  point  associated with  a
			 * vertex inserted into the triangulation.
			 * \param e A pointer to  an edge opposite to the inserted
			 * point.
			 *
			 */
			void 
				RestoreDelaunayProperty(
					spPoint p,
					Edge* e
				);


			/**
			 * \fn Edge* Connect(Edge* ea, Edge* eb)
			 *
			 * \brief  Add   a  new  edge  to   the  triangulation  by
			 * connecting  the destination  vertex of  an edge  to the
			 * origin vertex of another. All three edges will have the
			 * same left face after being connected.
			 *
			 * \param ea Pointer to an edge.
			 * \param eb Pointer to an edge.
			 *
			 * \return Pointer the newly created edge.
			 *
			 */
			Edge*
				Connect(
					Edge* ea,
					Edge* eb
				);


			/**
			 * \fn void Swap(Edge* e)
			 *
			 * \brief Turn  a given  edge counterclockwise  inside the
			 * quadrilateral given  by the  two triangles  incident to
			 * the edge.
			 *
			 * \param e Pointer to an edge.
			 *
			 */
			void
				Swap(
					Edge* e
				);


			/**
			 * \fn void DeleteEdge(Edge* e)
			 *
			 * \brief Removes a given edge from the CDT.
			 *
			 * \param e Pointer to an edge.
			 *
			 */
			void 
				DeleteEdge(
					Edge* e
				);


			/**
			 * \fn bool RightOf(spPoint p, Edge* e)
			 *
			 * \brief Returns the Boolean value  true if and only if a
			 * point is on the right face of a given edge.
			 *
			 * \param p A pointer to a point.
			 * \param e A given edge.
			 *
			 * \return The Boolean value true if the given point is on
			 * the right face of the given edge, and false otherwise.
			 *
			 */
			bool 
				RightOf(
					spPoint p,
					Edge* e
				);


			/**
			 * \fn bool OnEdge(spPoint p, Edge* e)
			 *
			 * \brief Returns the Boolean value  true if and only if a
			 * point belongs to a given edge.
			 *
			 * \param p A given point.
			 * \param e A pointer to an edge.
			 *
			 * \return  The  Boolean value  true  if  the given  point
			 * belongs to the given edge, and false otherwise.
			 *
			 */
			bool 
				OnEdge(
					spPoint p,
					Edge* e
				);


			/**
			 * \fn bool InCircle(spPoint a, spPoint b, spPoint c, spPoint d)
			 *
			 * \brief Returns the Boolean value  true if and only if a
			 * given point d is in the interior of a circle defined by
			 * three points: a, b, and c.
			 *
			 * \param a First point defining a circle.
			 * \param b Second point defining a circle.
			 * \param c Third point defining a circle.
			 * \param d A point to be tested against the incircle test.
			 *
			 * \return The Boolean value true  if the test point is in
			 * the interior of  the circle defined by  the other three
			 * points, and false otherwise.
			 *
			 */
			bool
				InCircle(
					spPoint a,
					spPoint b,
					spPoint c,
					spPoint d
				);


			/**
			 * \fn Edge* FindEnclosingTriangleEdge()
			 *
			 * \brief Find one edge of the enclosing triangle.
			 *
			 * \return A pointer to an edge of the enclosing triangle.
			 *
			 */
			Edge* FindEnclosingTriangleEdge();


			/**
			 * \fn void AddVertex(Vertex* v)
			 *
			 * \brief Inserts  a vertex into  the list of  vertices of
			 * this CDT.
			 *
			 * \param v A pointer to a vertex.
			 *
			 */
			void
				AddVertex(
					Vertex* v
				)
			{
				v->SetPosition(_lverts.insert(_lverts.end(), v));
			}


			/**
			 * \fn void AddEdge(Edge* e)
			 *
			 * \brief Inserts an  edge into the list of  edges of this
			 * CDT.
			 *
			 * \param e A pointer to an edge.
			 *
			 */
			void
				AddEdge(
					Edge* e
				)
			{
				e->SetPosition(_ledges.insert(_ledges.end(), e));
			}


			/**
			 * \fn void AddFace(Face* f)
			 *
			 * \brief Inserts  a face into  the list of faces  of this
			 * CDT.
			 *
			 * \param f A pointer to a face.
			 *
			 */
			void
				AddFace(
					Face* f
				)
			{
				f->SetPosition(_lfaces.insert(_lfaces.end(), f));
			}


			/**
			 * \fn void RemoveBoundaryVertex(Edge* e)
			 *
			 * \brief Remove all triangles incident to a vertex of the
			 * enclosing triangle, and then remove the vertex from the
			 * current CDT.
			 *
			 * \param e A  boundary edge incident to the  vertex to be
			 * removed.
			 *
			 */
			void 
				RemoveBoundaryVertex(
					Edge* e
				);


			/**
			 * \fn void RemoveBoundaryTriangle(Edge* e)
			 *
			 * \brief  Remove  a  triangle  incident  to  exactly  one
			 * boundary edge.
			 *
			 * \param e A boundary edge incident to the triangle.
			 *
			 */
			void 
				RemoveBoundaryTriangle(
					Edge* e
				);


			/**
			 * \fn void RemoveBoundaryTriangleAndVertex(Edge* e1, Edge* e2)
			 *
			 * \brief  Remove  a  triangle  incident  to  exactly  two
			 * boundary edges.
			 *
			 * \param e1 Pointer to a boundary edge of the triangle.
			 * \param e2  Pointer to  the other  boundary edge  of the
			 * triangle.
			 *
			 */
			void
				RemoveBoundaryTriangleAndVertex(
					Edge* e1,
					Edge* e2
				);


			/**
			 * \fn void DeleteFace(Face* f)
			 *
			 * \brief Removes a given bounded face from the CDT.
			 *
			 * \param f Pointer to a face.
			 *
			 */
			void
				DeleteFace(
					Face* f
				);


			/**
			 * \fn void MarkEdgeAsConstrained(Edge* e)
			 *
			 * \brief  Set the  constrained flag  of a  given edge  to
			 * true, as well  as the flags of its  symmetric edge, its
			 * dual edge, and the dual of its symmetric edge.
			 *
			 * \param e A pointer to an edge.
			 *
			 */
			void
				MarkEdgeAsConstrained(
					Edge* e
				)
			{
				e->SetConstrained(true);

				return;
			}


			/**
			 * \fn Edge* FindVerticesOnTheRightSide(Edge* e1, Edge* e2, EdgeList& elist)
			 *
			 * \brief  Finds  the  set  of  "right"  vertices  of  the
			 * (consecutive)  edges of  the  CDT  whose interiors  are
			 * crossed  when  traversing  a   line  segment  (i.e.,  a
			 * constraint) oriented from the  origin vertex of a given
			 * edge \c e1  to the origin vertex of  another given edge
			 * \c e2.  The method returns  a pointer to an  edge whose
			 * origin vertex lies on  the oriented line segment (i.e.,
			 * the oriented  line segment intersects this  edge at its
			 * origin  vertex)  and  is  the closest  vertex  (on  the
			 * segment) to  the origin  vertex of  \c e1.   The origin
			 * vertex of returned edge may not  be the same as the one
			 * of \c e2, as the segment may intersect several vertices
			 * of the CDT.
			 *
			 * \param e1 A  pointer to an edge whose  origin vertex is
			 * at the origin point of an oriented line segment.
			 * \param e2 A  pointer to an edge whose  origin vertex is
			 * at the destination point of an oriented line segment.
			 * \param elist A reference to a list to store the crossed
			 * edges.
			 *
			 * \return A pointer  to an edge whose  origin vertex lies
			 * on the  oriented line segment (i.e.,  the oriented line
			 * segment intersects this edge  at its origin vertex) and
			 * is the  closest vertex (on  the segment) to  the origin
			 * vertex of the segment (but is not at its origin).
			 *
			 */
			Edge*
				FindVerticesOnTheRightSide(
					Edge* e1,
					Edge* e2,
					EdgeList& elist
				);


			/**
			 * \fn Edge* GetClosestEdgeOnTheRightSide(Edge* e, spPoint q)
			 *
			 * \brief  Traverse the  star of  the origin  vertex of  a
			 * given edge  \c e in order  to find and return  the edge
			 * that defines the largest nonpositive angle with respect
			 * to the  oriented line  segment defined from  the origin
			 * vertex of \c e and a  given point \c q.  The edge found
			 * is the closest edge to the oriented line segment on the
			 * right side of the supporting  line of the oriented line
			 * segment.
			 *
			 * \param e  A pointer to  an edge whose origin  vertex is
			 * the origin vertex of the oriented line segment.
			 * \param q The destination point of the oriented segment.
			 *
			 * \return  The  closest edge  to  a  given oriented  line
			 * segment. The edge and the segment share the same origin
			 * point, and  the edge belongs  to the right side  of the
			 * (oriented) supporting line of the segment.
			 *
			 */
			Edge*
				GetClosestEdgeOnTheRightSide(
					Edge* e,
					spPoint q
				);


			/**
			 * \fn void SwapEdgesAwayFromConstraint(Edge* e1, Edge* e2, EdgeList& elist, EdgeList& swapped)
			 *
			 * \brief  Swap  edges  whose   interiors  cross  a  given
			 * oriented  line  segment,  so that  the  segment  itself
			 * becomes a new edge of the CDT.
			 *
			 * \param e1 A  pointer to an edge whose  origin vertex is
			 * the origin vertex of the oriented line segment.
			 * \param e2 A pointer to an edge whose destination vertex
			 * is the destination vertex of the oriented line segment.
			 * \param  elist A  reference to  a list  with the  set of
			 * vertices of the current CDT  that are on the right-side
			 * on  the oriented  line segment  and belong  to an  edge
			 * crossed by the segment.
			 * \param  swapped A  reference  to a  list  of all  edges
			 * swapped away from the segment. At the beginning of this
			 * function, this list must be  empty.  At the end of this
			 * function, this  list must  contain all edges  that were
			 * swapped away from the segment.
			 *
			 */
			void
				SwapEdgesAwayFromConstraint(
					Edge* e1,
					Edge* e2,
					EdgeList& elist,
					EdgeList& swapped
				);


			/**
			 * \fn void RestoreDelaunayProperty(const EdgeList& elist)
			 *
			 * \brief   For  each   edge  of   a  list   of  interior,
			 * unconstrained  edges, determines  whether  the edge  is
			 * Delaunay.  If it is not, swap the edge.
			 *
			 * \param elist A list of unconstrained edges.
			 *
			 */
			void
				RestoreDelaunayProperty(
					const EdgeList& elist
				);


			/**
			 * \fn bool IsConvex(spPoint a, spPoint b, spPoint c, spPoint d)
			 *
			 * \brief Returns the Boolean value  true if and only if a
			 * given quadrilateral  \f$ [ a  , b  , c ,  d ] \f$  is a
			 * strictly convex quadrilateral.
			 *
			 * \param a First  vertex of the given  quadrilateral in a
			 * CCW traversal of its vertices.
			 * \param b Second vertex of  the given quadrilateral in a
			 * CCW traversal of its vertices.
			 * \param c Third  vertex of the given  quadrilateral in a
			 * CCW traversal of its vertices.
			 * \param d Fourth vertex of  the given quadrilateral in a
			 * CCW traversal of its vertices.
			 *
			 * \return   The  Boolean   value   true   if  the   given
			 * quadrilateral  is strictly  convex; otherwise,  returns
			 * the Boolean value false.
			 *
			 */
			bool
				IsConvex(
					spPoint a,
					spPoint b,
					spPoint c,
					spPoint d
				);


			/**
			 * \fn bool CrossSegment(spPoint a, spPoint b, spPoint c, spPoint d)
			 *
			 * \brief Returns  the Boolean value  true if and  only if
			 * the two endpoints of a  given oriented line segment lie
			 * in opposite  sides of  the oriented supporting  line of
			 * another oriented line segment.
			 *
			 * \param  a Origin vertex of the first segment.
			 * \param  b Destination vertex of the first segment.
			 * \param  c Origin vertex of the second segment.
			 * \param  d Destination vertex of the second segment.
			 *
			 * \return The Boolean value true  if the two endpoints of
			 * a given oriented line segment  lie in opposite sides of
			 * the oriented  supporting line of another  oriented line
			 * segment; otherwise, returns the Boolean value false.
			 *
			 */
			bool 
				CrossSegment(
					spPoint a,
					spPoint b,
					spPoint c,
					spPoint d
				);


			/**
			 * \fn void FindEnclosingEdges(spPoint p, spPoint q, Edge* e, Edge*& el, Edge*& er)
			 *
			 * \brief Finds the two enclosing edges of a given edge \c
			 * e whose interior crosses  a given segment oriented from
			 * a point \c p to a point \c q.
			 *
			 * \param p Origin vertex of an oriented line segment.
			 * \param q Destination vertex of an oriented line segment.
			 * \param e  A pointer to  an edge whose  interior crosses
			 * the line segment.
			 * \param  el  A  reference  to  a  pointer  to  the  left
			 * enclosing edge.
			 * \param  er  A  reference  to a  pointer  to  the  right
			 * enclosing edge.
			 *
			 */
			void 
				FindEnclosingEdges(
					spPoint p,
					spPoint q,
					Edge* e,
					Edge*& el,
					Edge*& er
				);



			/**
			 * \fn bool HaveSameLocation(spPoint p, spPoint q) const
			 *
			 * \brief Returns true if and  only if two points have the
			 * same location in the plane.
			 *
			 * \param p A pointer to a point.
			 * \param q A pointer to another point.
			 *
			 */
			bool 
				HaveSameLocation(
					spPoint p,
					spPoint q
				)
				const
			{
				return ((p->GetU() == q->GetU()) && (p->GetV() == q->GetV()));
			}

		};

	}

}

/** @} */ //end of group class.
