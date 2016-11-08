/**
 * \file Cdt.cpp
 *
 * \brief  Implementation of  non-inline methods  of class  CDT, which
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
 // for more details  regarding the quad-edge data  structure, see the
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

#include "Cdt.h"           // Cdt

#include <iostream>        // std::cerr, std::endl
#include <cmath>           // fabs
#include <queue>           // std::queue
#include <list>            // std::list
#include <stdexcept>       // std::runtime_error


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
		 * \fn CDT::CDT(size_t numberOfPoints, const std::vector<spPoint>& points)
		 *
		 * \brief Creates a instance of this class, which represents a
		 * CDT  of a  set of  points  and segments  defining a  Planar
		 * Straight-Line Graph (PSLG).
		 *
		 * \param numberOfPoints The number of points.
		 * \param points An array with the point coordinates.
		 *
		 */
		CDT::CDT(
			size_t numberOfPoints,
			const std::vector<spPoint>& points
		)
		{
			// Determine whether the input data is consistent.
			if (numberOfPoints < 3)
			{
				throw std::runtime_error("The number of points must be at least 3.");
			}
		  
#ifdef DEBUGMODE
			if (numberOfPoints > points.size())
			{
				throw std::runtime_error("The number of points informed is larger than the number of points in the array.");
			}
#endif

			bool isConsistent = CheckInputConsistency(
				numberOfPoints,
				points
			);

			if (!isConsistent)
			{
				throw std::runtime_error("The given points are all collinear.");
			}

			// Create  the  Delaunay  triangulation   of  the  set  of
			// points.  Find the  largest absolute  coordinate of  the
			// given points.
			double max = 0;

			for (size_t i = 0; i < numberOfPoints; i++)
			{
				double u = fabs(points[i]->GetU());
				double v = fabs(points[i]->GetV());

				if (u > max)
				{
					max = u;
				}

				if (v > max)
				{
					max = v;
				}
			}

			// Keep the value of the largest coordinate of a vertex of
			// the enclosing triangle.  This value will be used in the
			// incircle test.
			max *= 3;
			SetLargestAbsoluteValueOfCoordinate(max);

			// Creates the three vertices of the enclosing triangle.
			spPoint pa = std::make_shared<DtPoint>( max,    0);
			spPoint pb = std::make_shared<DtPoint>(   0,  max);
			spPoint pc = std::make_shared<DtPoint>(-max, -max);

			// Creates the enclosing triangle.
			CreateEnclosingTriangle(pa, pb, pc);

			// Insert the given points into the triangulation.
			for (size_t i = 0; i < numberOfPoints; i++)
			{
				InsertPoint(points[i]);
			}

			// Remove  the enclosing  triangle of  the current  CDT as
			// well as the triangles lying  outside the convex hull of
			// the given point set.
			RemoveEnclosingTriangle();

			return;
		}


		/**
		 * \fn CDT::CDT(size_t numberOfPoints, size_t numberOfSegments, const std::vector<spPoint>& points, const std::vector<size_t>& segments)
		 *
		 * \brief Creates a instance of this class, which represents a
		 * CDT  of a  set of  points  and segments  defining a  Planar
		 * Straight-Line Graph (PSLG).
		 *
		 * \param numberOfPoints The number of points.
		 * \param numberOfSegments The number of segments.
		 * \param points An array with the point coordinates.
		 * \param segments An array with  the ID's of the two vertices
		 * of each segment.
		 *
		 */
		CDT::CDT(
			size_t numberOfPoints,
			size_t numberOfSegments,
			const std::vector<spPoint>& points,
			const std::vector<size_t>& segments
		)
		{
		  	// Determine whether the input data is consistent.
			if (numberOfPoints < 3)
			{
				throw std::runtime_error("The number of points must be at least equal to 3.");
			}
			
			// Determine whether the input data is consistent.
#ifdef DEBUGMODE
			if (numberOfPoints > points.size())
			{
				throw std::runtime_error("The number of points informed is larger than the number of points in the array.");
			}
			if (numberOfSegments > segments.size())
			{
				throw std::runtime_error("The number of segments informed is larger than the number of segments in the array.");
			}
#endif

			bool isConsistent = CheckInputConsistency(
				numberOfPoints,
				numberOfSegments,
				points,
				segments
			);

			if (!isConsistent)
			{
				throw std::runtime_error("Either the given points are all collinear or a given line segment intersect the interior of another.");
			}

			// First, create the Delaunay  triangulation of the set of
			// points.  Later,  we  insert   the  given  segments  and
			// generate the CDT.

			// Find  the  largest  absolute coordinate  of  the  given
			// points.
			double max = 0;

			for (size_t i = 0; i < numberOfPoints; i++)
			{
				double u = fabs(points[i]->GetU());
				double v = fabs(points[i]->GetV());

				if (u > max)
				{
					max = u;
				}

				if (v > max)
				{
					max = v;
				}
			}

			// Keep the value of the largest coordinate of a vertex of
			// the enclosing triangle.  This value will be used in the
			// incircle test.
			max *= 3;
			SetLargestAbsoluteValueOfCoordinate(max);

			// Creates the three vertices of the enclosing triangle.
			spPoint pa = std::make_shared<DtPoint>( max,    0);
			spPoint pb = std::make_shared<DtPoint>(   0,  max);
			spPoint pc = std::make_shared<DtPoint>(-max, -max);

			// Creates the enclosing triangle.
			CreateEnclosingTriangle(pa, pb, pc);

			// Insert the given points into the triangulation.
			for (size_t i = 0; i < numberOfPoints; i++)
			{
				InsertPoint(points[i]);
			}

			// Insert the segments and generate the CDT.
			for (size_t i = 0; i < numberOfSegments; i++)
			{
				size_t j1 = segments[2 * i];
				size_t j2 = segments[2 * i + 1];

				InsertSegment(
					points[j1],
					points[j2]
				);
			}

			// Remove  the enclosing  triangle of  the current  CDT as
			// well as the triangles lying  outside the convex hull of
			// the given point set.
			RemoveEnclosingTriangle();

			return;
		}


		/**
		 * \fn CDT::~CDT()
		 *
		 * \brief  Releases the  memory held  by an  instance of  this
		 * class.
		 *
		 */
		CDT::~CDT()
		{
			FaceIterator fit = FacesBegin();

			while (fit != FacesEnd())
			{
				Face* f = *fit;
				++fit;
				DeleteFace(f);
			}

			// Create a queue for storing edges.
			std::queue<Edge*> q;

			// Initialize it with the starting edge.
			Edge* efirst = GetStartingEdge();

			q.push(efirst);

			// Mark the quad-edge of the starting edge as visited.
			efirst->GetOwner()->SetMark(true);

			// Visit all quad-edges.
			while (!q.empty())
			{
				// Get the first edge of the queue.
				Edge* e = q.front();
				q.pop();

				// Insert adjacent edges into the queue.
				Edge* eaux;

				eaux = e->OriginNext();
				if (!eaux->GetOwner()->IsMarked())
				{
					q.push(eaux);
					eaux->GetOwner()->SetMark(true);
				}

				eaux = e->OriginPrev();
				if (!eaux->GetOwner()->IsMarked())
				{
					q.push(eaux);
					eaux->GetOwner()->SetMark(true);
				}

				eaux = e->DestinationNext();
				if (!eaux->GetOwner()->IsMarked())
				{
					q.push(eaux);
					eaux->GetOwner()->SetMark(true);
				}

				eaux = e->DestinationPrev();
				if (!eaux->GetOwner()->IsMarked())
				{
					q.push(eaux);
					eaux->GetOwner()->SetMark(true);
				}

				// Remove the first quad-edge of the queue.
				DeleteEdge(e);
			}

			return;
		}


	  	/**
		 * \fn bool CDT::CheckInputConsistency(size_t numberOfPoints, const std::vector<spPoint>& points)
		 *
		 * \brief  Determines whether  the input  is consistent.  This
		 * amounts to verifying if not  all given points are collinear
		 * and if  no given  line segment  intersects the  interior of
		 * another given line segment.
		 *
		 * \param numberOfPoints The number of points.
		 * \param points The point coordinates.
		 *
		 * \return The  logic value true  if the input  is consistent,
		 * and the logic value false otherwise.
		 *
		 */
		bool 
			CDT::CheckInputConsistency(
				size_t numberOfPoints,
				const std::vector<spPoint>& points
			)
		{
			// Make sure the point set has at least three points.
			if (numberOfPoints < 3)
			{
				return false;
			}

			// Make sure not all points are collinear.

			double pu = points[0]->GetU();
			double pv = points[0]->GetV();
			double qu = points[1]->GetU();
			double qv = points[1]->GetV();

			bool allCollinear = true;
			size_t i = 2;
			while (allCollinear && (i < numberOfPoints))
			{
				// Get the coordinates of the i-th point.
				double su = points[i]->GetU();
				double sv = points[i]->GetV();

				// If the  i-th point  is collinear with  all previous
				// points,  then   increment  the  point   counter  \c
				// i. Otherwise, do not increment it.
				allCollinear = _preds.Collinear(
					pu,
					pv,
					qu,
					qv,
					su,
					sv
				);

				if (allCollinear)
				{
					++i;
				}
			}

			if (i == numberOfPoints)
			{
				return false;
			}

			// If  we  reach  this  point, then  the  points  are  not
			// collinear.
			return true;
		}


		/**
		 * \fn bool CDT::CheckInputConsistency(size_t numberOfPoints, size_t numberOfSegments, const std::vector<spPoint>& points, const std::vector<size_t>& segments)
		 *
		 * \brief  Determines whether  the input  is consistent.  This
		 * amounts to verifying if not  all given points are collinear
		 * and if  no given  line segment  intersects the  interior of
		 * another given line segment.
		 *
		 * \param numberOfPoints The number of points.
		 * \param numberOfSegments The number of segments.
		 * \param points The point coordinates.
		 * \param  segments  The ID's  of  the  two vertices  of  each
		 * segment.
		 *
		 * \return The  logic value true  if the input  is consistent,
		 * and the logic value false otherwise.
		 *
		 */
		bool 
			CDT::CheckInputConsistency(
				size_t numberOfPoints,
				size_t numberOfSegments,
				const std::vector<spPoint>& points,
				const std::vector<size_t>& segments
			)
		{
			// Make sure the point set has at least three points.
			if (numberOfPoints < 3)
			{
				return false;
			}

			// Make sure not all points are collinear.

			double pu = points[0]->GetU();
			double pv = points[0]->GetV();
			double qu = points[1]->GetU();
			double qv = points[1]->GetV();

			bool allCollinear = true;
			size_t i = 2;
			while (allCollinear && (i < numberOfPoints))
			{
				// Get the coordinates of the i-th point.
				double su = points[i]->GetU();
				double sv = points[i]->GetV();

				// If the  i-th point  is collinear with  all previous
				// points,  then   increment  the  point   counter  \c
				// i. Otherwise, do not increment it.
				allCollinear = _preds.Collinear(
					pu,
					pv,
					qu,
					qv,
					su,
					sv
				);

				if (allCollinear)
				{
					++i;
				}
			}

			if (i == numberOfPoints)
			{
				return false;
			}

			// Make sure no segment intersects the interior of another
			// segment.
			for (size_t i = 1; i < numberOfSegments; i++)
			{
				// Test the i-th segment against previously considered
				// segments.
				for (size_t j = 0; j < i; j++)
				{
					// Determines  whether the  endpoints of  the j-th
					// line segment  belong to  opposite sides  of the
					// line by the endpoints of the i-th segment.
					Predicates<double>::Orientation res1 =
						_preds.Classify(
							points[segments[2 * i]]->GetU(),
							points[segments[2 * i]]->GetV(),
							points[segments[2 * i + 1]]->GetU(),
							points[segments[2 * i + 1]]->GetV(),
							points[segments[2 * j]]->GetU(),
							points[segments[2 * j]]->GetV()
						);

					Predicates<double>::Orientation res2 =
						_preds.Classify(
							points[segments[2 * i]]->GetU(),
							points[segments[2 * i]]->GetV(),
							points[segments[2 * i + 1]]->GetU(),
							points[segments[2 * i + 1]]->GetV(),
							points[segments[2 * j + 1]]->GetU(),
							points[segments[2 * j + 1]]->GetV()
						);

					if (
							(
								(res1 == Predicates<double>::Orientation::Left ) 
								&&
								(res2 == Predicates<double>::Orientation::Right)
							)
							||
							(
								(res1 == Predicates<double>::Orientation::Right) 
								&&
								(res2 == Predicates<double>::Orientation::Left )
							)
						)
					{
						// The  endpoints  of  the j-th  line  segment
						// belong to opposite sides of the line by the
						// endpoints  of the  i-th line  segment.  So,
						// there is a possibilility that the i-th line
						// segment intersects the interior of the j-th
						// line segment.
						Predicates<double>::Orientation res3 =
							_preds.Classify(
								points[segments[2 * j]]->GetU(),
								points[segments[2 * j]]->GetV(),
								points[segments[2 * j + 1]]->GetU(),
								points[segments[2 * j + 1]]->GetV(),
								points[segments[2 * i]]->GetU(),
								points[segments[2 * i]]->GetV()
							);

						if (res3 == Predicates<double>::Orientation::Between) {
							// An  endpoint of  the i-th  line segment
							// belong to the interior of the j-th line
							// segment, which should not be allowed.
							return false;
						}
						else if (res3 == Predicates<double>::Orientation::Left) {
							// An endpoint of the i-th line segment is
							// on the  left side of the  oriented line
							// from the  first to the  second endpoint
							// of the j-th line segment.  If the other
							// endpoint of the i-th line segment is on
							// the same  line or on the  right side of
							// it,   then   the  i-th   line   segment
							// intersects  the  interior of  the  j-th
							// line segment.
							Predicates<double>::Orientation res4 =
								_preds.Classify(
									points[segments[2 * j]]->GetU(),
									points[segments[2 * j]]->GetV(),
									points[segments[2 * j + 1]]->GetU(),
									points[segments[2 * j + 1]]->GetV(),
									points[segments[2 * i + 1]]->GetU(),
									points[segments[2 * i + 1]]->GetV()
								);

							if (
									(res4 == Predicates<double>::Orientation::Right  ) 
								||
									(res4 == Predicates<double>::Orientation::Between)
								)
							{
								return false;
							}
						}
						else if (res3 == Predicates<double>::Orientation::Right) {
							// An endpoint of the i-th line segment is
							// on the right side  of the oriented line
							// from the  first to the  second endpoint
							// of the j-th line segment.  If the other
							// endpoint of the i-th line segment is on
							// the same  line or  on the left  side of
							// it,   then   the  i-th   line   segment
							// intersects  the  interior of  the  j-th
							// line segment.
							Predicates<double>::Orientation res4 =
								_preds.Classify(
									points[segments[2 * j]]->GetU(),
									points[segments[2 * j]]->GetV(),
									points[segments[2 * j + 1]]->GetU(),
									points[segments[2 * j + 1]]->GetV(),
									points[segments[2 * i + 1]]->GetU(),
									points[segments[2 * i + 1]]->GetV()
								);
							if (
									(res4 == Predicates<double>::Orientation::Left   ) 
								||
									(res4 == Predicates<double>::Orientation::Between)
								)
							{
								return false;
							}
						}
					}
					else if (
								(
									(res1 == Predicates<double>::Orientation::Origin     ) 
									&&
									(res2 == Predicates<double>::Orientation::Destination)
								)
								||
								(
									(res1 == Predicates<double>::Orientation::Destination)
									&&
									(res2 == Predicates<double>::Orientation::Origin     )
								)
							)
					{
						// The i-th  and the j-th line  segments share
						// exactly   the  same   two  endpoints,   and
						// therefore  they are  the same  line segment
						// (possibly  with opposite  directions if  we
						// consider an  orientation from the  first to
						// the second endpoint).
						return false;
					}
				}
			}

			// If we  reach this  line, points  and line  segments are
			// consistent!
			return true;
		}


		/**
		 * \fn void CDT::CreateEnclosingTriangle(spPoint pa, spPoint pb, spPoint pc)
		 *
		 * \brief Creates  an enclosing  triangle for the  vertices of
		 * this  CDT.   This  triangle   will  be  removed  after  the
		 * triangulation is built.
		 *
		 * \param pa First  vertex of the enclosing triangle  in a CCW
		 * traverse.
		 * \param pb Second vertex of  the enclosing triangle in a CCW
		 * traverse.
		 * \param pc Third  vertex of the enclosing triangle  in a CCW
		 * traverse.
		 *
		 */
		void
			CDT::CreateEnclosingTriangle(
				spPoint pa,
				spPoint pb,
				spPoint pc
			)
		{
			// Create the three vertices of the enclosing triangle.
			Vertex* va = new Vertex(pa, nullptr);
			Vertex* vb = new Vertex(pb, nullptr);
			Vertex* vc = new Vertex(pc, nullptr);

			// Create  the first  edge, which  connects the  first and
			// second vertices.
			QuadEdge* qea = new QuadEdge();
			Edge* ea = qea->GetEdge(0);
			ea->SetEndpoints(va, vb);

			// Create the  second edge, which connects  the second and
			// third vertices.
			QuadEdge* qeb = new QuadEdge();
			Edge* eb = qeb->GetEdge(0);
			eb->SetEndpoints(vb, vc);

			// Attach the first two edges at their common vertex.
			ea->Symmetric()->Splice(eb);

			// Creates the third edge,  which connecting the first and
			// third vertices,  giving rise  to the first  triangle of
			// the triangulation.
			QuadEdge* qec = new QuadEdge();
			Edge* ec = qec->GetEdge(0);
			ec->SetEndpoints(vc, va);

			// Attach the third edge to the second one at their common
			// vertex.
			eb->Symmetric()->Splice(ec);

			// Attach the third edge to  the first one at their common
			// vertex.
			ec->Symmetric()->Splice(ea);

			// Make the first edge of the triangle the starting edge.
			SetStartingEdge(ea);

			// Set the edge  pointers of the first,  second, and third
			// vertices.
			va->SetEdge(ea);
			vb->SetEdge(eb);
			vc->SetEdge(ec);

			// Add the new edges to the list of edges of this CDT.
			AddEdge(ea);
			AddEdge(eb);
			AddEdge(ec);

			AddEdge(ea->Symmetric());
			AddEdge(eb->Symmetric());
			AddEdge(ec->Symmetric());

			// Add the  new vertices to  the list of vertices  of this
			// CDT.
			AddVertex(va);
			AddVertex(vb);
			AddVertex(vc);

			// Create the first bounded face of this CDT.
			Face* f = new Face();

			// Set the first edge of the face.
			f->SetEdge(ea);

			// Set the face pointer of all edges of the face.
			f->GetEdge()->SetFace(f);
			f->GetEdge()->LeftNext()->SetFace(f);
			f->GetEdge()->LeftPrev()->SetFace(f);

			// Set the face as bounded.
			f->SetBounded(true);

			// Add the new face to the list of faces of this CDT.
			AddFace(f);

			// Create the first and only unbounded face of this CDT.
			f = new Face();

			// Set the first edge of the unbounded face.
			f->SetEdge(ea->Symmetric());

			// Set  the face  pointer of  all edges  of the  unbounded
			// face.
			f->GetEdge()->SetFace(f);
			f->GetEdge()->LeftNext()->SetFace(f);
			f->GetEdge()->LeftPrev()->SetFace(f);

			// Set the face as unbounded.
			f->SetBounded(false);

			// Add the new face to the list of faces of this CDT.
			AddFace(f);

			return;
		}


		/**
		 * \fn void CDT::InsertPoint(spPoint p)
		 *
		 * \brief Insert a new vertex into the current CDT.
		 *
		 * \param p A given vertex.
		 *
		 */
		void 
			CDT::InsertPoint(
				spPoint p
			)
		{
			// Locate the point in the current CDT.
			Edge* e = Locate(p);

			// If the  point is  already in  the triangulation,  it is
			// discarded.
			if (
				HaveSameLocation(p, e->GetOrigin()->GetPoint())
				||
				HaveSameLocation(p, e->GetDestination()->GetPoint())
				)
			{
				return;
			}

			// Insert the point in the current triangulation.
			SplitEdgeOrTriangle(p, e);

			// -------------------------------------------------------
			// At this point, edge e is opposite to point p in a newly
			// created triangle,  and the  starting edge follows  e in
			// the left face of e.
			// -------------------------------------------------------

			// Restore the Delaunay property  of the triangulation, if
			// needed.
			RestoreDelaunayProperty(p, e);

			return;
		}


		/**
		 * \fn void CDT::InsertSegment(spPoint p1, spPoint p2)
		 *
		 * \brief Insert a segment (i.e., a constrained edge) into the
		 * current CDT.
		 *
		 * \param p1 An endpoint of the segment.
		 * \param p2 Another endpoint of the segment.
		 *
		 */
		void 
			CDT::InsertSegment(
				spPoint p1,
				spPoint p2
			)
		{
			// Find an edge incident on one endpoint of the segment.
			Edge* e1 = Locate(p1);

			if (!HaveSameLocation(p1, e1->GetOrigin()->GetPoint()))
			{
				e1 = e1->Symmetric();

#ifdef DEBUGMODE
				if (!HaveSameLocation(p1, e1->GetOrigin()->GetPoint()))
				{
					throw std::runtime_error("Could not find a triangulation edge sharing a vertex with the given segment");
				}
#endif
			}

			// If the destination  vertex of \c e1 is \c  p2, then the
			// segment  is already  an edge  of the  current CDT,  and
			// there  is nothing  else to  do other  than setting  the
			// constrained flag of \c e1 to true.
			if (HaveSameLocation(p2, e1->GetDestination()->GetPoint()))
			{
				MarkEdgeAsConstrained(e1);
				return;
			}

			// Find  an edge  incident on  the other  endpoint of  the
			// segment.
			Edge* e2 = Locate(p2);

			if (!HaveSameLocation(p2, e2->GetOrigin()->GetPoint()))
			{
				e2 = e2->Symmetric();

#ifdef DEBUGMODE
				if (!HaveSameLocation(p2, e2->GetOrigin()->GetPoint()))
				{
					throw std::runtime_error("Could not find a triangulation edge sharing a vertex with the given segment");
				}
#endif
			}

			// Find the  longest sequence  of edges whose  interior is
			// crossed by the oriented line segment defined from \c p1
			// to  \c  p2.  Then,  swap   such  edges  away  from  the
			// segment. This is  done by first finding  a vertex whose
			// crossing edges incident to it can all be swapped.
			EdgeList elist;
			Edge* e;
			bool done = false;
			do
			{
				// Create  a list  with  the set  of  vertices of  the
				// crossing edges which  are on the right  side of the
				// oriented line segment from \c p1 to \c p2.
				e = FindVerticesOnTheRightSide(
					e1,
					e2,
					elist
				);

				// If the list of vertices  is empty, then the segment
				// connecting the  origin vertices of  \c e1 and  \c e
				// must be an edge of the current CDT.
				if (elist.empty())
				{
#ifdef DEBUGMODE
					//
					// Ensure     the     post-condition     of     \c
					// findVerticesOnTheRightSide holds.
					//
					if (!HaveSameLocation(e->LeftPrev()->GetOrigin()->GetPoint(), e1->GetOrigin()->GetPoint()))
					{
						throw std::runtime_error("Could not find a triangulation edge sharing a vertex with the given segment");
					}
#endif

					// Set edge \c e1 as a constrained edge.
					MarkEdgeAsConstrained(e->LeftPrev());
				}
				else {
					// Swap  the   crossing  edges  incident   on  the
					// vertices  in  list  \c   vlist.  By  doing  so,
					// "constraint"  \c  p1p   is  inserted  into  the
					// current CDT.  If vertex \c  p is the same as \c
					// p2, we are done.  Otherwise, we repeat the loop
					// to insert segment \c p2.
					EdgeList slist;
					SwapEdgesAwayFromConstraint(
						e1,
						e,
						elist,
						slist
					);

#ifdef DEBUGMODE
					// Checking post-condition:
					if (slist.empty())
					{
						throw std::runtime_error("Could not insert a coinstrained segment in the triangulation");
					}
#endif

					// Get the last edge of the list of swapped edges.
					Edge* constraint = slist.back();
					slist.pop_back();

#ifdef DEBUGMODE
					// Make sure the segment and the last edge match.
					if (HaveSameLocation(constraint->GetOrigin()->GetPoint(), e->GetOrigin()->GetPoint()))
					{
						if (!HaveSameLocation(constraint->GetDestination()->GetPoint(), e1->GetOrigin()->GetPoint()))
						{
							throw std::runtime_error("Could not insert a coinstrained segment in the triangulation");
						}
					}
					else
					{
						if (!HaveSameLocation(constraint->GetOrigin()->GetPoint(), e1->GetOrigin()->GetPoint()))
						{
							throw std::runtime_error("Could not insert a coinstrained segment in the triangulation");
						}
						if (!HaveSameLocation(constraint->GetDestination()->GetPoint(), e->GetOrigin()->GetPoint()))
						{
							throw std::runtime_error("Could not insert a coinstrained segment in the triangulation");
						}
					}
#endif

					// Set the constrained flag of the last edge.
					MarkEdgeAsConstrained(constraint);

					// Restore  the Delaunay  property of  the current
					// triangulation, which ensures that the resulting
					// triangulation is a CDT.
					RestoreDelaunayProperty(slist);
				}

				if (HaveSameLocation(e->GetOrigin()->GetPoint(), p2))
				{
					done = true;
				}
				else {
					e1 = e;
				}

			} while (!done);

			return;
		}


		/**
		 * \fn void CDT::RemoveEnclosingTriangle()
		 *
		 * \brief  Remove the  enclosing  triangle  and all  triangles
		 * adjacent to each one of its vertices (as these triangles do
		 * not belong to the Delaunay triangulation of the given input
		 * set of points).
		 *
		 */
		void CDT::RemoveEnclosingTriangle()
		{
			// Find an edge of the enclosing triangle.
			Edge* eNext = FindEnclosingTriangleEdge();

			for (unsigned int i = 0; i < 3; i++)
			{
				// Set a  pointer to a  boundary edge incident  to the
				// vertex  of the  enclosing  triangle  to be  removed
				// along with its adjacent triangles.
				Edge* eIter = eNext;

				// Get the another boundary  edge of the triangulation
				// before removing a vertex  of the enclosing triangle
				// and its adjacent triangles.
				eNext = eNext->Symmetric()->OriginNext();

				// Remove a vertex of  the enclosing triangles and its
				// adjacent triangles.
				RemoveBoundaryVertex(eIter);
			}

			return;
		}


		/**
		 * \fn Edge* CDT::Locate(spPoint p)
		 *
		 * \brief Search  for an edge  that contains a given  point or
		 * belongs to a triangle that  contains the given point in its
		 * interior.  The search starts with the starting edge of this
		 * CDT.
		 *
		 * \param p A given point.
		 *
		 * \return A pointer to an edge of the triangulation.
		 *
		 */
		Edge*
			CDT::Locate(
				spPoint p
			)
		{
			// Starts the search from the starting edge of the CDT.
			Edge* e = GetStartingEdge();

			// Loop   over  the   triangles   of  this   triangulation
			// (indirectly)  until  a  triangle containing  the  given
			// point is found. Note that all points are supposed to be
			// inside  the enclosing  triangle,  so  this loop  should
			// always   terminate  (unless   some  numerical   problem
			// occurs).
			bool end = false;

			while (!end)
			{
				// If  the given  point coincides  with the  origin or
				// destination vertex of edge \c e, then we return the
				// edge itself.
				if (HaveSameLocation(p, e->GetOrigin()->GetPoint()) || HaveSameLocation(p, e->GetDestination()->GetPoint()))
				{
					return e;
				}
				else if (RightOf(p, e))
				{
					// If the point is on the right face of edge \c e,
					// consider the symmetric of  \c e, and repeat the
					// test for this symmetric edge.
					e = e->Symmetric();
				}
				else if (!RightOf(p, e->OriginNext()))
				{
					// The point  is not  on the right  face of  \c e.
					// But,  it  is not  in  the  left  face of  \c  e
					// either. So,  consider let us try  the next face
					// in a CCW traverse of  the faces incident to the
					// origin vertex  of \c e, starting  with the face
					// following the left face of \c e.
					e = e->OriginNext();
				}
				else if (!RightOf(p, e->DestinationPrev()))
				{
					// The point is on the  "cone" defined by \c e and
					// its  next edge  with  the  same origin  vertex.
					// However, the point is  beyond the third edge of
					// the  triangle  containing  \c e  and  the  next
					// edge. In this case,  we consider the right face
					// of the third edge.
					e = e->DestinationPrev();
				}
				else
				{
					end = true;
				}
			}

			return e;
		}


		/**
		 * \fn void CDT::SplitEdgeOrTriangle(spPoint p, Edge*& e)
		 *
		 * \brief Insert a  vertex inside a triangle or on  an edge of
		 * the  current triangulation,  producing a  new triangulation
		 * which may not be a CDT.
		 *
		 * \param p A pointer to  the point associated with the vertex
		 * to be inserted into the triangulation.
		 * \param e A pointer to an edge of the current CDT.
		 *
		 */
		void
			CDT::SplitEdgeOrTriangle(
				spPoint p,
				Edge*& e
			)
		{
			//
			// Assume  that v  is either  on  e or  inside a  triangle
			// containing e.
			//

			if (OnEdge(p, e))
			{
				// The point  is on an  edge, and we remove  the edge.
				// Before,  we   ensure  that   the  edge  is   not  a
				// constrained edge. The removed edge can be the start
				// one.  If so, the starting edge pointer is updated.

				//
				// Abort if point is on a constrained edge.
				//
				if (e->IsConstrained())
				{
					throw std::runtime_error("Attempt to insert point on a constrained edge.");
				}

				e = e->OriginPrev();

				DeleteFace(e->GetFace());

				DeleteEdge(e->OriginNext());
			}

			// Connect the point  to the vertices of  the triangle (or
			// quadrilateral) that contains it.  To do that, we create
			// three (or four) new edges.
			QuadEdge* qeb = new QuadEdge();
			Edge* eb = qeb->GetEdge(0);

			// Add the  new edge  (and its symmetric)  to the  list of
			// edges.
			AddEdge(eb);
			AddEdge(eb->Symmetric());

			// Create the new point.
			Vertex* newVertex = new Vertex(p, nullptr);

			// Add the new vertex to the list of vertices of this CDT.
			AddVertex(newVertex);

			// Set up the coordinates of the first edge vertices.
			eb->SetEndpoints(e->GetOrigin(), newVertex);

			// Set up the edge pointer of the new point.
			newVertex->SetEdge(eb->Symmetric());

			// Attach the first edge to the current CDT.
			eb->Splice(e);

			// Update the starting edge value.
			SetStartingEdge(eb);

			// Create the remaining edges of the triangle.
			do
			{
				// A triangle is created by connecting the destination
				// vertex of  edge \c  e to the  origin vertex  of the
				// symmetric of edge \c eb.
				eb = Connect(e, eb->Symmetric());

				// Update edge \c e.
				e = eb->OriginPrev();

				// Create a new bounded face of this CDT.
				Face* f = new Face();

				// Set the first edge of the face.
				f->SetEdge(eb);

				// Set the face as bounded.
				f->SetBounded(true);

				// Set the face pointer of all edges of the face.
				f->GetEdge()->SetFace(f);
				f->GetEdge()->LeftNext()->SetFace(f);
				f->GetEdge()->LeftPrev()->SetFace(f);

				// Add the new face to the list of faces of this CDT.
				AddFace(f);

			} while (e->LeftNext() != GetStartingEdge());

			// Update the cycle of edges corresponding to the splitted
			// face.
			e->GetFace()->SetEdge(e);
			e->LeftNext()->SetFace(e->GetFace());
			e->LeftPrev()->SetFace(e->GetFace());

			return;
		}


		/**
		 * \fn void CDT::RestoreDelaunayProperty(spPoint p, Edge* e)
		 *
		 * \brief If the insertion of  a point makes the triangulation
		 * to violate the property of being  a CDT, then swap edges to
		 * restore the property.
		 *
		 * \param p  A pointer to  the point associated with  a vertex
		 * inserted into the triangulation.
		 * \param  e A  pointer to  an edge  opposite to  the inserted
		 * point.
		 *
		 */
		void
			CDT::RestoreDelaunayProperty(
				spPoint p,
				Edge* e
			)
		{
			// Examine suspicious  edges to  ensure that all  edges in
			// the current triangulation  are edges of the  CDT of the
			// given PSLG.
			do
			{
				// Get an edge in the right face of edge \c e.
				Edge* t = e->OriginPrev();

				// If edge \c e is  not constrained and the right face
				// is not  unbounded, then  we can apply  the incircle
				// test to  edge \c e,  so that  it is flipped  if and
				// only if it does not pass the test.
				if (
					!e->IsConstrained()                   // e is unconstrained edge
					&&
					RightOf(                              // t is not a boundary edge
						t->GetDestination()->GetPoint(),
						e
					)
					&&
					InCircle(                             // incircle test
						e->GetOrigin()->GetPoint(),
						t->GetDestination()->GetPoint(),
						e->GetDestination()->GetPoint(),
						p
					)
					)
				{
					// Swap edge \c e.
					Swap(e);

					e->GetFace()->SetEdge(e);
					e->LeftPrev()->SetFace(e->GetFace());

					e->Symmetric()->GetFace()->SetEdge(e->Symmetric());
					e->Symmetric()->LeftPrev()->SetFace(e->Symmetric()->GetFace());

					// Update  variable \c  e, so  it becomes  another
					// suspicious edge.
					e = e->OriginPrev();
				}
				else if (e->OriginNext() == GetStartingEdge())
				{
					// If the tested edge passes the incircle test and
					// there is no  other edge to be  tested, then the
					// current triangulation is a CDT.
					return;
				}
				else {
					// Get  the next  suspicious  edge.  This edge  is
					// obtained in  a CW  traverse of the  boundary of
					// the star  polygon defined  by the union  of all
					// triangles incident to  the newly inserted point
					// \c p.
					e = e->OriginNext()->LeftPrev();
				}
			} while (true);

			return;
		}


		/**
		 * \fn Edge* CDT::Connect(Edge* ea, Edge* eb)
		 *
		 * \brief Add  a new edge  to the triangulation  by connecting
		 * the destination vertex  of an edge to the  origin vertex of
		 * another. All three edges will have the same left face after
		 * being connected.
		 *
		 * \param ea Pointer to an edge.
		 * \param eb Pointer to an edge.
		 *
		 * \return Pointer to the newly created edge.
		 *
		 */
		Edge* 
			CDT::Connect(
				Edge* ea,
				Edge* eb
			)
		{
			// Create a new quad-edge.
			QuadEdge* qe = new QuadEdge();

			// Get the first primal edge of the new quad-edge.
			Edge* newEdge = qe->GetEdge(0);

			// Add the new edge to the list of edges of this CDT.
			AddEdge(newEdge);
			AddEdge(newEdge->Symmetric());

			// Connect the new edge to edge \c ea.
			newEdge->Splice(ea->LeftNext());

			// Connect the new edge to edge \c eb.
			newEdge->Symmetric()->Splice(eb);

			// Assign vertices to the new edge.
			newEdge->SetEndpoints(ea->GetDestination(), eb->GetOrigin());

			return newEdge;
		}


		/**
		 * \fn void CDT::Swap(Edge* e)
		 *
		 * \brief  Turn  a  given  edge  counterclockwise  inside  the
		 * quadrilateral given  by the  two triangles incident  to the
		 * edge.
		 *
		 * \param e Pointer to an edge.
		 *
		 */
		void
			CDT::Swap(
				Edge* e
			)
		{
			// Get  an edge  that shares  the origin  vertex with  the
			// given edge, and another one that shares the destination
			// vertex  (in  the  quadrilateral   defined  by  the  two
			// triangles incident to the given edge).
			Edge* ea = e->OriginPrev();
			Edge* eb = e->Symmetric()->OriginPrev();

			// Adjust the edge  pointers of the origin  vertices of \c
			// ea and \c eb to point to those edges (just in case they
			// point to edge \c e).
			ea->GetOrigin()->SetEdge(ea);
			eb->GetOrigin()->SetEdge(eb);

			// Dettach the given edge from  the edges sharing the same
			// origin vertex.
			e->Splice(ea);

			// Dettach the given edge from  the edges sharing the same
			// destination vertex.
			e->Symmetric()->Splice(eb);

			// Attach  the new  edge to  the  other two  edges of  the
			// quadrilateral.
			e->Splice(ea->LeftNext());
			e->Symmetric()->Splice(eb->LeftNext());

			// Update the  origin and destination vertices  of the new
			// edge.
			e->SetEndpoints(ea->GetDestination(), eb->GetDestination());

			return;
		}


		/**
		 * \fn void CDT::DeleteEdge(Edge* e)
		 *
		 * \brief Removes a given edge from the CDT.
		 *
		 * \param e Pointer to an edge.
		 *
		 */
		void 
			CDT::DeleteEdge(
				Edge* e
			)
		{
			// Get the origin and destination vertices of edge \c e.
			Vertex* vo = e->GetOrigin();
			Vertex* vd = e->GetDestination();

			// If the vertices of edge \c e do not belong to any other
			// edge of the triangulation, the memory held by them must
			// be released.  Otherwise, their edge pointers must point
			// to other incident edges.
			if (e != e->OriginPrev())
			{
				vo->SetEdge(e->OriginPrev());
				vo = nullptr;
			}

			if (e != e->DestinationPrev())
			{
				vd->SetEdge(e->Symmetric()->OriginPrev());
				vd = nullptr;
			}

			// Dettach the edge from the stars of its vertices.
			e->Splice(e->OriginPrev());
			e->Symmetric()->Splice(e->Symmetric()->OriginPrev());

			// Remove  the edge  and its  symmetric from  the list  of
			// edges of the CDT.
			_ledges.erase(e->GetPosition());
			_ledges.erase(e->Symmetric()->GetPosition());

			delete e->GetOwner();

			// If needed, release  the memory held by  the vertices of
			// the deleted edge.
			if (vo != nullptr)
			{
				_lverts.erase(vo->GetPosition());
				delete vo;
			}

			if (vd != nullptr)
			{
				_lverts.erase(vd->GetPosition());
				delete vd;
			}

			return;
		}


		/**
		 * \fn bool CDT::RightOf(spPoint p, Edge* e)
		 *
		 * \brief  Returns the  Boolean value  true if  and only  if a
		 * point is on the right face of a given edge.
		 *
		 * \param p A pointer to a point.
		 * \param e A given edge.
		 *
		 * \return The Boolean value true if the given point is on the
		 * right face of the given edge, and false otherwise.
		 *
		 */
		bool
			CDT::RightOf(
				spPoint p,
				Edge* e
			)
		{
			spPoint po = e->GetOrigin()->GetPoint();
			spPoint pd = e->GetDestination()->GetPoint();

			bool res = _preds.LeftOn(
				po->GetU(),
				po->GetV(),
				pd->GetU(),
				pd->GetV(),
				p->GetU(),
				p->GetV()
			);

			return !res;
		}


		/**
		 * \fn bool CDT::OnEdge(spPoint p, Edge* e)
		 *
		 * \brief  Returns the  Boolean value  true if  and only  if a
		 * point belongs to an edge.
		 *
		 * \param p A pointer to a point.
		 * \param e A pointer to an edge.
		 *
		 * \return The Boolean  value true if the  given point belongs
		 * to the given edge, and false otherwise.
		 *
		 */
		bool
			CDT::OnEdge(
				spPoint p,
				Edge* e
			)
		{
			spPoint po = e->GetOrigin()->GetPoint();
			spPoint pd = e->GetDestination()->GetPoint();

			Predicates< double >::Orientation orient =
				_preds.Classify(
					po->GetU(),
					po->GetV(),
					pd->GetU(),
					pd->GetV(),
					p->GetU(),
					p->GetV()
				);

			return 
				(orient == Predicates<double>::Orientation::Origin     ) 
				||
				(orient == Predicates<double>::Orientation::Destination) 
				||
				(orient == Predicates<double>::Orientation::Between    );
		}


		/**
		 * \fn bool CDT::InCircle(spPoint a, spPoint b, spPoint c, spPoint d)
		 *
		 * \brief  Returns the  Boolean value  true if  and only  if a
		 * given point  d is in  the interior  of a circle  defined by
		 * three points: a, b, and c.
		 *
		 * \param a First point defining a circle.
		 * \param b Second point defining a circle.
		 * \param c Third point defining a circle.
		 * \param d A point to be tested against the incircle test.
		 *
		 * \return The Boolean value true if  the test point is in the
		 * interior of the  circle defined by the  other three points,
		 * and false otherwise.
		 *
		 */
		bool
			CDT::InCircle(
				spPoint a,
				spPoint b,
				spPoint c,
				spPoint d
			)
		{
			// To  make  sure that  the  enclosing  triangle does  not
			// interfere  with the  Delaunay triangulation,  we use  a
			// slight modification  of the  strategy suggested  in the
			// book "Computational Geometry", by  de Berg, Cheong, van
			// Kreveld, and Overmars, 3rd edition, 2008, page 204.
			unsigned rankPointA = 0;
			if (fabs(a->GetU()) == GetLargestAbsoluteValueOfCoordinate())
			{
				rankPointA = 1;
			}

			if (fabs(a->GetV()) == GetLargestAbsoluteValueOfCoordinate())
			{
				rankPointA += 2;
			}

			unsigned rankPointB = 0;
			if (fabs(b->GetU()) == GetLargestAbsoluteValueOfCoordinate())
			{
				rankPointB = 1;
			}

			if (fabs(b->GetV()) == GetLargestAbsoluteValueOfCoordinate())
			{
				rankPointB += 2;
			}

			unsigned rankPointC = 0;

			if (fabs(c->GetU()) == GetLargestAbsoluteValueOfCoordinate())
			{
				rankPointC = 1;
			}

			if (fabs(c->GetV()) == GetLargestAbsoluteValueOfCoordinate())
			{
				rankPointC += 2;
			}

			if ((rankPointA == 0) && (rankPointB == 0) && (rankPointC == 0))
			{
				// None of  \c a, \c  b, and \c c  is a vertex  of the
				// enclosing triangle.
				bool res = _preds.InCircle(
					a->GetU(),
					a->GetV(),
					b->GetU(),
					b->GetV(),
					c->GetU(),
					c->GetV(),
					d->GetU(),
					d->GetV()
				);
				return res;
			}

			if ((rankPointB > rankPointC) && (rankPointB > rankPointA))
			{
				// Point \c b  is a vertex of  the enclosing triangle,
				// and  maybe one  of  \c a  and \c  c  is special  as
				// well.   However,   either  the   quadrilateral   \c
				// [a,b,c,d] is  not strictly convex or  edge \c [b,d]
				// intersects edge  \c [a,c].  In the  former case, we
				// cannot flip \c  [a,c].  In the latter  case, we can
				// conclude that  \c [a,c] belongs to  the convex hull
				// of  the original  input point  set.  So,  in either
				// case, edge \c [a,c] should  never be flipped and we
				// return false.

				return false;
			}

			// If we reach  this point, then we know for  sure that at
			// least one of \c  a, \c b, and \c c  is a special point,
			// but \c b is not special  or it is not the special point
			// with the  largest rank. So,  we should always  flip the
			// edge, except  for case in  which \c [a,b,c,d] is  not a
			// strictly convex quadrilateral. That is, if \c [a,b,c,d]
			// is strictly convex, then we should always keep the edge
			// that is incident  to the vertex of  least rank (between
			// \c  b  and the  one  of  \c a  and  \c  c with  largest
			// rank). This  arbitrary rule ensures that  the algorithm
			// does not loop forever swapping the same edges.
			bool res1 = _preds.Left(
				b->GetU(),
				b->GetV(),
				c->GetU(),
				c->GetV(),
				d->GetU(),
				d->GetV()
			);

			bool res2 = _preds.LeftOn(
				b->GetU(),
				b->GetV(),
				a->GetU(),
				a->GetV(),
				d->GetU(),
				d->GetV()
			);

			// Return  true  if  and  only if  [a,b,c,d]  is  strictly
			// convex.
			return res1 && !res2;
		}


		/**
		 * \fn Edge* CDT::FindEnclosingTriangleEdge()
		 *
		 * \brief Find one edge of the enclosing triangle.
		 *
		 * \return A pointer to an edge of the enclosing triangle.
		 *
		 */
		Edge* 
			CDT::FindEnclosingTriangleEdge()
		{
			// Create a point with coordinates ( max , 0 ).
			spPoint po = std::make_shared<DtPoint>(
				GetLargestAbsoluteValueOfCoordinate(),
				0
			);

			// Find an edge incident on this point.
			Edge* e = Locate(po);

			// If  the edge  edge  is  not an  edge  of the  enclosing
			// triangle, then find such an edge by traversing the star
			// of the vertex at point po.

			// Make sure \c  po is the origin vertex of  \c e. If not,
			// let \c e be its mate.
			if (e->GetOrigin()->GetU() != GetLargestAbsoluteValueOfCoordinate())
			{
				e = e->Symmetric();

#ifdef DEBUGMODE
				// If \c  po is not  the origin  vertex of \c  e, then
				// something  went  wrong   with  the  point  location
				// method,  as  it  is  expected  to  return  an  edge
				// incident to  a vertex at  \c po.  So, we  can abort
				// execution.
				if (e->GetOrigin()->GetU() != GetLargestAbsoluteValueOfCoordinate())
				{
					throw std::runtime_error("Failed to find the enclosing triangle of the Delaunay triangulation of the given set of points.");
				}
#endif
			}

			// Traverse  the star  of the  vertex at  \c po  until the
			// destination  vertex of  the current  edge \c  e is  the
			// vertex of coordinates (0 , max).
			while (e->GetDestination()->GetV() != GetLargestAbsoluteValueOfCoordinate())
			{
				e = e->OriginNext();
			}

			return e;
		}


		/**
		 * \fn void CDT::RemoveBoundaryVertex(Edge* e)
		 *
		 * \brief Remove  all triangles  incident to  a vertex  of the
		 * enclosing  triangle, and  then remove  the vertex  from the
		 * current CDT.
		 *
		 * \param  e A  boundary edge  incident  to the  vertex to  be
		 * removed.
		 *
		 */
		void 
			CDT::RemoveBoundaryVertex(
				Edge* e
			)
		{
			// Get the  other boundary  edge incident to  the boundary
			// vertex.
			Edge* eIter = e;
			Edge* eLast = e->OriginPrev();

			// Visit  and delete  all edges  incident to  the boundary
			// vertex.
			bool done = false;
			while (!done)
			{
				// Get  the next  edge in  a CCW  traverse around  the
				// vertex.
				Edge* eNext = eIter->OriginNext();

				// Make  sure  the  edge  to be  removed  is  not  the
				// starting edge.
				if (
					(eIter == GetStartingEdge()) ||
					(eIter->Symmetric() == GetStartingEdge())
					)
				{
					SetStartingEdge(eIter->DestinationPrev());
				}

				// If the  next edge  in the  traversal is  a boundary
				// edge, then  there is  only one  remaining triangle,
				// and  hence  two  edges  must be  removed  from  the
				// triangulation.  Otherwise, only one is.
				if (eNext == eLast)
				{
					RemoveBoundaryTriangleAndVertex(eIter, eLast);
					done = true;
				}
				else
				{
					RemoveBoundaryTriangle(eIter);
					eIter = eNext;
				}
			}

			return;
		}


		/**
		 * \fn void CDT::RemoveBoundaryTriangle(Edge* e)
		 *
		 * \brief Remove  a triangle incident to  exactly one boundary
		 * edge.
		 *
		 * \param e A boundary edge incident to the triangle.
		 *
		 */
		void 
			CDT::RemoveBoundaryTriangle(
				Edge* e
			)
		{
#ifdef DEBUGMODE
			//
			// Make sure edge \c e belongs to a bounded face.
			//
			if (!e->GetFace()->IsBounded())
			{
				throw std::runtime_error("Attempt to remove the unbounded face.");
			}
#endif

			// Get the symmetric edge.
			Edge* e2 = e->Symmetric();

			// Make the symmetric edge belongs to the unbounded face.
			Face* fub = e2->GetFace();

#ifdef DEBUGMODE
			//
			// Make sure edge \c e2 belongs to an  unbounded face.
			//
			if (fub->IsBounded())
			{
				throw std::runtime_error("Failed to find a boundary edge.");
			}
#endif

			// Remove the bounded face.
			DeleteFace(e->GetFace());

			// Change the face pointers of  the other two edges of the
			// bounded face.
			e->LeftPrev()->SetFace(fub);
			e->LeftNext()->SetFace(fub);

			// Update  the  edge pointer  of  the  unbounded face,  if
			// needed.
			if (fub->GetEdge() == e2)
			{
				fub->SetEdge(e->LeftNext());
			}

			DeleteEdge(e);

			return;
		}


		/**
		 * \fn void CDT::RemoveBoundaryTriangleAndVertex(Edge* e1, Edge* e2)
		 *
		 * \brief Remove  a triangle incident to  exactly two boundary
		 * edges.
		 *
		 * \param e1 Pointer to a boundary edge of the triangle.
		 * \param  e2  Pointer  to  the other  boundary  edge  of  the
		 * triangle.
		 *
		 */
		void
			CDT::RemoveBoundaryTriangleAndVertex(
				Edge* e1,
				Edge* e2
			)
		{
			// Get the point  associated with the origin  vertex of \c
			// e1.
			spPoint p1 = e1->GetOrigin()->GetPoint();

			if (e1 != e2)
			{
				// There  exists only  one  triangle  incident on  the
				// boundary vertex.
				Face* fub = e1->Symmetric()->GetFace();

#ifdef DEBUGMODE
				// Make sure the first edge  belongs to a bounded face
				// and the second, to  the unbounded face.  Both edges
				// will be removed later.
				if (!e1->GetFace()->IsBounded() || fub->IsBounded() || e2->GetFace()->IsBounded())
				{
					throw std::runtime_error("Failed to remove a boundary triangle and a vertex.");
				}
#endif

				// Remove the bounded face.
				DeleteFace(e1->GetFace());

				// Change the  face pointers of the  edge following \c
				// e1 in the bounded face.
				e1->LeftNext()->SetFace(fub);

				// Update the edge pointer of the unbounded face.
				if (fub->GetEdge() == e2)
				{
					fub->SetEdge(e1->LeftNext());
				}

				DeleteEdge(e1);
				DeleteEdge(e2);
			}
			else
			{
				// There  is only  one edge  incident on  the boundary
				// vertex.
				DeleteEdge(e1);
			}

			return;
		}


		/**
		 * \fn void CDT::DeleteFace(Face* f)
		 *
		 * \brief Removes a given bounded face from the CDT.
		 *
		 * \param f Pointer to a face.
		 *
		 */
		void
			CDT::DeleteFace(
				Face* f
			)
		{
			Edge* e = f->GetEdge();

#ifdef DEBUGMODE
			if (e == nullptr)
			{
				throw std::runtime_error("Attempt to destroy an edge referenced by a null pointer.");
			}
#endif

			// Make  sure no  edge points  to face  \c f  after it  is
			// removed.
			if (f->IsBounded())
			{
				e->SetFace(nullptr);
				e->LeftPrev()->SetFace(nullptr);
				e->LeftNext()->SetFace(nullptr);
			}
			else {
				Edge* eaux = e->LeftNext();
				do {
					eaux->SetFace(nullptr);
					eaux = eaux->LeftNext();
				} while (eaux != e);
			}

			// Remove the face from the list of faces of the CDT.
			_lfaces.erase(f->GetPosition());

			// Release memory held by the face.
			delete f;

			return;
		}


		/**
		 * \fn Edge* CDT::FindVerticesOnTheRightSide(Edge* e1, Edge* e2, EdgeList& elist)
		 *
		 * \brief   Finds  the   set  of   "right"  vertices   of  the
		 * (consecutive) edges of the  CDT whose interiors are crossed
		 * when  traversing  a  line   segment  (i.e.,  a  constraint)
		 * oriented from  the origin vertex of  a given edge \c  e1 to
		 * the origin vertex  of another given edge \c  e2. The method
		 * returns a  pointer to an  edge whose origin vertex  lies on
		 * the oriented line segment  (i.e., the oriented line segment
		 * intersects  this edge  at  its origin  vertex)  and is  the
		 * closest vertex (on the segment)  to the origin vertex of \c
		 * e1.  The origin vertex of returned edge may not be the same
		 * as the one  of \c e2, as the segment  may intersect several
		 * vertices of the CDT.
		 *
		 * \param e1  A pointer to an  edge whose origin vertex  is at
		 * the origin point of an oriented line segment.
		 * \param e2  A pointer to an  edge whose origin vertex  is at
		 * the destination point of an oriented line segment.
		 * \param elist  A reference  to a list  to store  the crossed
		 * edges.
		 *
		 * \return A  pointer to an  edge whose origin vertex  lies on
		 * the oriented line segment  (i.e., the oriented line segment
		 * intersects  this edge  at  its origin  vertex)  and is  the
		 * closest vertex (on the segment) to the origin vertex of the
		 * segment (but is not at its origin).
		 *
		 */
		Edge*
			CDT::FindVerticesOnTheRightSide(
				Edge* e1,
				Edge* e2,
				EdgeList& elist
			)
		{
			// Get the origin vertices of the given edges.
			spPoint p = e1->GetOrigin()->GetPoint();
			spPoint q = e2->GetOrigin()->GetPoint();

			// Find the edge that (1)  shares the same origin point \c
			// p  with \c  e1,  (2)  lies in  the  right  side of  the
			// supporting line of  the oriented line segment  \c pq or
			// on the \c pq itself, and (3) defines the smallest angle
			// with the oriented line segment \c pq.
			Edge* e = GetClosestEdgeOnTheRightSide(e1, q);

			// Find the longest  sequence of edges of  the current CDT
			// whose interiors are  crossed by the segment  \c pq when
			// traversing  the  segment  from  \c  p  to  \c  q.   The
			// following loop stops as soon as an edge intersection \c
			// pq at its origin or destination vertex is found.
			spPoint last = nullptr;
			bool done = false;
			do {
				// Is the destination vertex of \c e on the segment \c
				// qp?  If so, stop the  loop and return the following
				// \c e in  a CCW traversal of the  face containing \c
				// e.  Note that point \c s may be \c q.
				spPoint s = e->GetDestination()->GetPoint();

				Predicates< double >::Orientation orient =
					_preds.Classify(
						p->GetU(),
						p->GetV(),
						q->GetU(),
						q->GetV(),
						s->GetU(),
						s->GetV()
					);

#ifdef DEBUGMODE
				//
				// Make sure the result is not inconsistent.
				//
				if (
					(orient == Predicates< double >::Orientation::Behind) 
					||
					(orient == Predicates< double >::Orientation::Beyond) 
					||
					(orient == Predicates< double >::Orientation::Origin)
				   )
				{
					throw std::runtime_error("Inconsistent result in orientation test.");
				}
#endif

				if (
					(orient == Predicates< double >::Orientation::Between)
					||
					(orient == Predicates< double >::Orientation::Destination)
				   )
				{
					/*
					 * Point \c s lies on segment \c pq.
					 */
					done = true;
				}
				else {
					// If \c s lies on the right side of the oriented,
					// supporting line of \c  pq, then the interior of
					// the edge following  \c e in a  CCW traversal of
					// its  left  face is  crossed  by  \c pq  in  its
					// interior.  Otherwise,  edge   \c  e  itself  is
					// crossed by \c pq.
					if (orient == Predicates< double >::Orientation::Right)
					{
						e = e->LeftNext();
					}

					// Make sure  crossing edge  is not  a constrained
					// edge.

					if (e->IsConstrained())
					{
						throw std::runtime_error("Input contains a segment that intersects the interior of another segment.");
					}

					// Add the origin vertex  of the crossed edge into
					// the list of  vertices that are on  the right of
					// the oriented line from p to q. If the vertex is
					// already in the list, it is not added again.
					spPoint r = e->GetOrigin()->GetPoint();

					if (r != last)
					{
						elist.push_back(e);
						last = r;
					}

					// Get the next candidate edge.
					e = e->Symmetric()->LeftNext();
				}
			} while (!done);

			// Returns an edge whose origin vertex lies on \c pq.
			return e->LeftNext();
		}


		/**
		 * \fn Edge* CDT::GetClosestEdgeOnTheRightSide(Edge* e, spPoint q)
		 *
		 * \brief Traverse  the star of  the origin vertex of  a given
		 * edge \c e in order to find and return the edge that defines
		 * the largest nonpositive angle  with respect to the oriented
		 * line segment defined  from the origin vertex of \c  e and a
		 * given point  \c q.  The edge  found is the closest  edge to
		 * the  oriented  line  segment  on  the  right  side  of  the
		 * supporting line of the oriented line segment.
		 *
		 * \param e  A pointer to an  edge whose origin vertex  is the
		 * origin vertex of the oriented line segment.
		 * \param q The destination point of the oriented segment.
		 *
		 * \return  The   closest  edge  to  a   given  oriented  line
		 * segment. The  edge and  the segment  share the  same origin
		 * point,  and the  edge  belongs  to the  right  side of  the
		 * (oriented) supporting line of the segment.
		 *
		 */
		Edge* CDT::GetClosestEdgeOnTheRightSide(
			Edge* e,
			spPoint q
		)
		{
			// Get the origin vertex of the given edge.
			spPoint p = e->GetOrigin()->GetPoint();

			// Get the destination vertex of the given edge.
			spPoint s = e->GetDestination()->GetPoint();

			// Find out the position of  the destination vertex of the
			// given edge  with respect  to the line  segment oriented
			// from the origin vertex of the edge to \c q.
			Predicates< double >::Orientation orient =
				_preds.Classify(
					p->GetU(),
					p->GetV(),
					q->GetU(),
					q->GetV(),
					s->GetU(),
					s->GetV()
				);

#ifdef DEBUGMODE
			//
			// Make sure the result is consistent.
			//
			if (
				(orient == Predicates< double >::Orientation::Beyond)
				||
				(orient == Predicates< double >::Orientation::Origin)
				)
			{
				throw std::runtime_error("Inconsistent result in orientation test.");
			}
#endif

			// If  the destination  vertex belongs  to the  supporting
			// line of the  oriented segment and it is either  \c q or
			// it is in between the origin vertex of \c e and \c q. In
			// either case, then we are done.
			if (
				(orient == Predicates< double >::Orientation::Destination)
				||
				(orient == Predicates< double >::Orientation::Between    )
				)
			{
				return e;
			}

			// If the destination vertex is on the left side or behind
			// the  origin  vertex  on  the  supporting  line  of  the
			// oriented    segment,    then     we    should    rotate
			// counterclockwise around the origin of \c e.
			bool ccw = true;
			if (
				(orient == Predicates< double >::Orientation::Left  )
				||
				(orient == Predicates< double >::Orientation::Behind)
			   )
			{
				ccw = false;
			}

			// Rotate  around the  origin  vertex of  \c  e until  the
			// destination  vertex of  the  current edge  lies on  the
			// supporting  line of  the  oriented line  segment or  it
			// becomes the closest destination  vertex to the oriented
			// line segment on  the right side of  the supporting line
			// of the  segment (among the destination  vertices of all
			// edges sharing the same origin vertex with \c e).
			bool done = false;
			Edge* eaux = e;
			do
			{
				// Get the  next edge  in a  counterclockwise rotation
				// around the origin vertex of \c eaux if the value of
				// variable \c  ccw is  true; otherwise, get  the next
				// edge in a clockwise rotation.
				if (ccw)
				{
					eaux = eaux->OriginNext();
				}
				else
				{
					eaux = eaux->OriginPrev();
				}

				// Get the destination vertex of the current edge.
				spPoint s = eaux->GetDestination()->GetPoint();

				// Compute the  position of the destination  vertex of
				// the current  edge with respect to  the line segment
				// oriented from the  origin vertex of the  edge to \c
				// q.
				Predicates< double >::Orientation orient =
					_preds.Classify(
						p->GetU(),
						p->GetV(),
						q->GetU(),
						q->GetV(),
						s->GetU(),
						s->GetV()
					);

#ifdef DEBUGMODE
				//
				// Make sure the result is consistent.
				//
				if (
					(orient == Predicates< double >::Orientation::Beyond)
					||
					(orient == Predicates< double >::Orientation::Origin)
				   )
				{
					throw std::runtime_error("Inconsistent result in orientation test.");
				}
#endif

				// If the  destination vertex belongs to  the oriented
				// line segment and it is not behind the origin vertex
				// of the edge, then we are done with the search.
				if (
					(orient == Predicates< double >::Orientation::Destination)
					||
					(orient == Predicates< double >::Orientation::Between    )
				   )
				{
					done = true;
				}
				else if ((orient == Predicates< double >::Orientation::Left) && ccw)
				{
					// The destination  vertex is on the  left side of
					// the (oriented) supporting  line of the oriented
					// line  segment.  If it  was  on  the right  side
					// before,  then  we  are done  with  the  search.
					// Otherwise,   rotate  in   the  counterclockwise
					// direction.
					done = true;
					eaux = eaux->OriginPrev();
				}
				else if ((orient == Predicates< double >::Orientation::Right) && !ccw)
				{
					// The destination vertex is  on the right side of
					// the (oriented) supporting  line of the oriented
					// line  segment.  If  it  was on  the  left  side
					// before,  then  we  are done  with  the  search.
					// Otherwise, rotate in the clockwise direction.
					done = true;
				}
			} while (!done);

			return eaux;
		}


		/**
		 * \fn void CDT::SwapEdgesAwayFromConstraint(Edge* e1, Edge* e2, EdgeList& elist, EdgeList& swapped)
		 *
		 * \brief Swap  edges whose  interiors cross a  given oriented
		 * line segment, so that the segment itself becomes a new edge
		 * of the CDT.
		 *
		 * \param e1 A  pointer to an edge whose origin  vertex is the
		 * origin vertex of the oriented line segment.
		 * \param e2 A pointer to  an edge whose destination vertex is
		 * the destination vertex of the oriented line segment.
		 * \param elist A reference to a list with the set of vertices
		 * of  the current  CDT  that  are on  the  right-side on  the
		 * oriented line segment and belong  to an edge crossed by the
		 * segment.
		 * \param swapped A  reference to a list of  all edges swapped
		 * away from the  segment. At the beginning  of this function,
		 * this list must be empty.  At the end of this function, this
		 * list must contain all edges that were swapped away from the
		 * segment.
		 *
		 */
		void
			CDT::SwapEdgesAwayFromConstraint(
				Edge* e1,
				Edge* e2,
				EdgeList& elist,
				EdgeList& swapped
			)
		{
			/**
			 * The following code is based on the paper
			 *
			 * Nira  Dyn,  Ifat  Goren,  Shmuel  Rippa.   Transforming
			 * triangulations  in polygonal  domains.  Computer  Aided
			 * Geometric Design, 10(6), p. 531-536, 1993.
			 *
			 * The rationale behind the code  is to loop over the list
			 * of  edges  whose  interiors  cross  the  oriented  line
			 * segment,   and  determines   one  edge   that  can   be
			 * swapped.  Lemmas  2.2  and  2.3 from  the  above  paper
			 * ensures that  such an  edge exists.  The code  tries to
			 * find a  swappable, crossing edge whose  swap gives rise
			 * to an edge that does not  cross the segment. If such an
			 * edge does not exist, then  another result from the same
			 * paper ensures that  there exists an endpoint  of one of
			 * the  remaining  edges  such  that  all  crossing  edges
			 * incident to it can be  swapped (in some order). So, the
			 * idea  is  to  find  this endpoint  and  then  swap  all
			 * crossing edges  incident to  it.  After that,  the code
			 * tries  to find  a swappable,  crossing edge  whose swap
			 * gives rise to an edge  that does not cross the segment.
			 * These two  steps are repeated until  all crossing edges
			 * are swapped away from the constraint.
			 */

#ifdef DEBUGMODE
			 //
			 // Make sure the list of vertices is not empty.
			 //
			if (elist.empty())
			{
				throw std::runtime_error("Could not insert a segment into the triangulation");
			}

			if (!swapped.empty())
			{
				throw std::runtime_error("Could not insert a segment into the triangulation");
			}
#endif

			// Repeatedly traverse  the list  of vertices in  order to
			// find a vertex such that  all crossing edges incident to
			// the vertex can be swapped.
			spPoint p = e1->GetOrigin()->GetPoint();
			spPoint q = e2->GetOrigin()->GetPoint();

			do
			{
				// Get an iterator for the current list of vertices.
				EdgeList::iterator eit = elist.begin();

				Edge* el = nullptr;
				Edge* er = nullptr;
				bool found = false;

				while (!found && (eit != elist.end()))
				{
					// Compute  the angle  defined  by  the two  edges
					// incident on the current vertex that enclose all
					// crossing edges incident on it.
					FindEnclosingEdges(p, q, *eit, el, er);

					spPoint a = el->GetDestination()->GetPoint();
					spPoint b = er->GetOrigin()->GetPoint();
					spPoint c = er->GetDestination()->GetPoint();

					// If the angle is  smaller than 180 degrees, then
					// there exists  a swappable edge incident  to the
					// current vertex. Find such an edge.
					found = _preds.Left(
						a->GetU(),
						a->GetV(),
						b->GetU(),
						b->GetV(),
						c->GetU(),
						c->GetV()
					);

					if (!found)
					{
						++eit;
					}
				}

#ifdef DEBUGMODE
				//
				// Make sure a vertex was found.
				//
				if (!found)
				{
					throw std::runtime_error("Could not insert a segment into the triangulation");
				}
#endif

				// Swap all edges enclosed by \c el and \c er.
				while (el != er->LeftPrev()->Symmetric())
				{
					// Consider the  crossing edge  to the left  of \c
					// er.
					Edge* e = er->LeftPrev()->Symmetric();
					do
					{
						// Get the vertices of edge \c e.
						spPoint a = e->GetOrigin()->GetPoint();
						spPoint c = e->GetDestination()->GetPoint();

#ifdef DEBUGMODE
						//
						// Make sure \c e is a crossing edge.
						//
						if (!CrossSegment(p, q, a, c))
						{
							throw std::runtime_error("Could not insert a segment into the triangulation");
						}
#endif

						// Get two opposite vertices to edge \c e.
						spPoint b = e->Symmetric()->LeftPrev()->GetOrigin()->GetPoint();
						spPoint d = e->LeftPrev()->GetOrigin()->GetPoint();

						// If edge \c e is  swappable, then swap it.
						if (IsConvex(a, b, c, d))
						{
							// Get the edge to the right of \c e.
							Edge* f = e->Symmetric()->LeftNext();

							// Swap edge \c e.
							Swap(e);

							// If the swapped edge  does not cross the
							// constraint, then add it  to the list of
							// swapped edges. Otherwise, do not add.
							if (!CrossSegment(p, q, b, d))
							{
								swapped.push_back(e);
							}

							// Update  the face  pointers of  the data
							// structure.
							e->GetFace()->SetEdge(e);
							e->LeftPrev()->SetFace(e->GetFace());

							e->Symmetric()->GetFace()->SetEdge(e->Symmetric());
							e->Symmetric()->LeftPrev()->SetFace(e->Symmetric()->GetFace());

							// Update the variable \c e.
							e = f;
						}

						// Get the next edge.
						e = e->LeftPrev()->Symmetric();
					} while (e != el);

					// At this  point, all crossing edges  enclosed by
					// \c er and \c el have been checked, and the ones
					// that were swappable during  the check have been
					// swapped. The swap of an edge can make swappable
					// a previously non-swappable  edge. So, the while
					// loop is repeated until all edges enclosed by \c
					// er and  \c el are  swapped.  A lemma  from Dyn,
					// Goren, and Rippa's paper  ensures that the loop
					// ends after finitely many iterations.
				}

				// Remove  the   current  vertex  from  the   list  of
				// vertices.
				EdgeList::iterator itemp = eit;
				++eit;
				elist.erase(itemp);

			} while (!elist.empty());

			return;
		}


		/**
		 * \fn void CDT::RestoreDelaunayProperty(const EdgeList& elist)
		 *
		 * \brief For each  edge of a list  of interior, unconstrained
		 * edges, determines whether  the edge is Delaunay.   If it is
		 * not, swap the edge.
		 *
		 * \param elist A list of unconstrained edges.
		 *
		 */
		void 
			CDT::RestoreDelaunayProperty(
				const EdgeList& elist
			)
		{
			for (
				EdgeIterator eit = elist.begin();
				eit != elist.end();
				++eit
				)
			{
				// Make sure the current edge is unconstrained.
				Edge* e = *eit;

#ifdef DEBUGMODE
				if (e->IsConstrained())
				{
					throw std::runtime_error("Could not insert a segment into the triangulation");
				}

#endif
				// Make sure edge \c e is not a boundary edge.

				Edge* q = e->Symmetric();
				Edge* t = q->OriginPrev();

#ifdef DEBUGMODE
				if (!RightOf(t->GetDestination()->GetPoint(), q))
				{
					throw std::runtime_error("Could not insert a segment into the triangulation");
				}
#endif
				t = e->OriginPrev();

#ifdef DEBUGMODE
				if (!RightOf(t->GetDestination()->GetPoint(), e))
				{
					throw std::runtime_error("Could not insert a segment into the triangulation");
				}
#endif

				// Apply the inCircle test.
				bool res = InCircle(
					e->GetOrigin()->GetPoint(),
					t->GetDestination()->GetPoint(),
					e->GetDestination()->GetPoint(),
					e->OriginNext()->GetDestination()->GetPoint()
				);

				if (res) {
					// Swap edge \c e.
					Swap(e);

					// Update face pointers.
					e->GetFace()->SetEdge(e);
					e->LeftPrev()->SetFace(e->GetFace());

					e->Symmetric()->GetFace()->SetEdge(e->Symmetric());
					e->Symmetric()->LeftPrev()->SetFace(e->Symmetric()->GetFace());
				}
			}

			return;
		}


		/**
		 * \fn bool CDT::IsConvex(spPoint a, spPoint b, spPoint c, spPoint d)
		 *
		 * \brief  Returns the  Boolean value  true if  and only  if a
		 * given quadrilateral \f$ [ a , b , c , d ] \f$ is a strictly
		 * convex quadrilateral.
		 *
		 * \param a First  vertex of the given quadrilateral  in a CCW
		 * traversal of its vertices.
		 * \param b Second vertex of  the given quadrilateral in a CCW
		 * traversal of its vertices.
		 * \param c Third  vertex of the given quadrilateral  in a CCW
		 * traversal of its vertices.
		 * \param d Fourth vertex of  the given quadrilateral in a CCW
		 * traversal of its vertices.
		 *
		 * \return The  Boolean value true if  the given quadrilateral
		 * is strictly  convex; otherwise,  returns the  Boolean value
		 * false.
		 *
		 */
		bool
			CDT::IsConvex(
				spPoint a,
				spPoint b,
				spPoint c,
				spPoint d
			)
		{
			bool res1 = _preds.Left(
				b->GetU(),
				b->GetV(),
				c->GetU(),
				c->GetV(),
				d->GetU(),
				d->GetV()
			);

			bool res2 = _preds.LeftOn(
				b->GetU(),
				b->GetV(),
				a->GetU(),
				a->GetV(),
				d->GetU(),
				d->GetV()
			);

			bool res3 = _preds.Left(
				a->GetU(),
				a->GetV(),
				c->GetU(),
				c->GetV(),
				d->GetU(),
				d->GetV()
			);

			// Return true if and only if [a,b,c,d] is strictly convex.
			return res1 && !res2 && res3;
		}


		/**
		 * \fn bool CDT::CrossSegment(spPoint a, spPoint b, spPoint c, spPoint d)
		 *
		 * \brief Returns  the Boolean value  true if and only  if the
		 * two  endpoints of  a  given oriented  line  segment lie  in
		 * opposite sides  of the oriented supporting  line of another
		 * oriented line segment.
		 *
		 * \param  a Origin vertex of the first segment.
		 * \param  b Destination vertex of the first segment.
		 * \param  c Origin vertex of the second segment.
		 * \param  d Destination vertex of the second segment.
		 *
		 * \return The  Boolean value true  if the two endpoints  of a
		 * given oriented  line segment lie  in opposite sides  of the
		 * oriented supporting line of  another oriented line segment;
		 * otherwise, returns the Boolean value false.
		 *
		 */
		bool
			CDT::CrossSegment(
				spPoint a,
				spPoint b,
				spPoint c,
				spPoint d
			)
		{
			bool res = _preds.Left(
				a->GetU(),
				a->GetV(),
				b->GetU(),
				b->GetV(),
				c->GetU(),
				c->GetV()
			);

			if (res)
			{
				return _preds.Left(
					b->GetU(),
					b->GetV(),
					a->GetU(),
					a->GetV(),
					d->GetU(),
					d->GetV()
				);
			}

			res = _preds.Left(
				b->GetU(),
				b->GetV(),
				a->GetU(),
				a->GetV(),
				c->GetU(),
				c->GetV()
			);

			if (res)
			{
				return _preds.Left(
					a->GetU(),
					a->GetV(),
					b->GetU(),
					b->GetV(),
					d->GetU(),
					d->GetV()
				);
			}

			return false;
		}


		/**
		 * \fn void CDT::FindEnclosingEdges(spPoint p, spPoint q, Edge* e, Edge*& el, Edge*& er)
		 *
		 * \brief Finds the  two enclosing edges of a given  edge \c e
		 * whose  interior crosses  a  given segment  oriented from  a
		 * point \c p to a point \c q.
		 *
		 * \param p Origin vertex of an oriented line segment.
		 * \param q Destination vertex of an oriented line segment.
		 * \param e  A pointer to  an edge whose interior  crosses the
		 * line segment.
		 * \param el  A reference to  a pointer to the  left enclosing
		 * edge.
		 * \param er A  reference to a pointer to  the right enclosing
		 * edge.
		 *
		 */
		void 
			CDT::FindEnclosingEdges(
				spPoint p,
				spPoint q,
				Edge* e,
				Edge*& el,
				Edge*& er
			)
		{
			spPoint s = e->GetOrigin()->GetPoint();
			spPoint t = e->GetDestination()->GetPoint();

#ifdef DEBUGMODE
			// Check pre-condition:
			//
			// [s,t] must cross the interior of [p,q].

			if (!CrossSegment(p, q, s, t))
			{
				throw std::runtime_error("Could not insert a segment into the triangulation");
			}
#endif

			// Find the left "enclosing" edge.
			el = e->LeftPrev()->Symmetric();
			bool found = false;

			do
			{
#ifdef DEBUGMODE
				// Enclosing edge  must share  the same  origin vertex
				// with \c e.
				if (!HaveSameLocation(el->GetOrigin()->GetPoint(), e->GetOrigin()->GetPoint()))
				{
					throw std::runtime_error("Could not insert a segment into the triangulation");
				}
#endif

				// Get the destination vertex of edge \c el.
				t = el->GetDestination()->GetPoint();

				// If the segment \c pq  and the segment \st intersect
				// at a  common interior point of  both segments, then
				// get the  next edge in a  counterclockwise traversal
				// of  the  star  of  the origin  vertex  of  edge  \c
				// e. Else, \c el is the left enclosing edge.
				if (CrossSegment(p, q, s, t))
				{
					el = el->LeftPrev()->Symmetric();
				}
				else {
					found = true;
				}
			} while (!found && (el != e));

			// Find the right "enclosing" edge.
			er = e->Symmetric()->LeftNext();
			found = false;
			do
			{
#ifdef DEBUGMODE   
				// Enclosing edge  must share  the same  origin vertex
				// with \c e.
				if (er->GetOrigin() != e->GetOrigin())
				{
					throw std::runtime_error("Could not insert a segment into the triangulation");
				}
#endif

				// Get the destination vertex of edge \c er.
				t = er->GetDestination()->GetPoint();

				// If the segment \c pq  and the segment \st intersect
				// at a  common interior point of  both segments, then
				// get the  next edge in a  counterclockwise traversal
				// of the star of the origin vertex of \c e.  Else, \c
				// er is the right enclosing edge.
				if (CrossSegment(p, q, s, t))
				{
					er = er->Symmetric()->LeftNext();
				}
				else
				{
					found = true;
				}
			} while (!found && (er != e));

			return;
		}

	}

}

/** @} */ //end of group class.
