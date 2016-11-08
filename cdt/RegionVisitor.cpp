/**
 * \file RegionVisitor.cpp
 *
 * \brief Implementation of some methods of class RegionVisitor, which
 * is a visitor to identify all trimmed and non-trimmed regions of the
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

#include "RegionVisitor.h"   // RegionVisitor

#include <stdexcept>         // std::runtime_error


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
		 * \fn void RegionVisitor:: Visit(CDT& cdt)
		 *
		 * \brief A visitor  method to assign a Boolean  value to each
		 * triangle of a single region of the CDT, where a region is a
		 * maximal, connected set  of bounded faces of  the CDT, which
		 * is  bounded  by  closed, polygonal  chains  of  constrained
		 * and/or boundary edges.
		 *
		 * \param  cdt  A  CDT   represented  by  the  quad-edge  data
		 * structure.
		 *
		 */
		void
			RegionVisitor::Visit(
				CDT& cdt
			)
		{
			// Visit all outer trimmed away regions.
			VisitOuterTrimmedRegion(cdt);

			// Visit all outer non-trimmed away regions.
			VisitOuterNonTrimmedRegion(cdt);

			// Visit all interior regions.
			VisitInteriorRegions(cdt);

			return;
		}


		/**
		 * \fn void RegionVisitor::VisitOuterTrimmedRegion(CDT& cdt)
		 *
		 * \brief Visits all outer, trimmed regions of the CDT.
		 *
		 * \param  cdt  A  CDT   represented  by  the  quad-edge  data
		 * structure.
		 *
		 */
		void
			RegionVisitor::VisitOuterTrimmedRegion(
				CDT& cdt
			)
		{
			// Collect  all unconstrained  boundary  edges of  bounded
			// faces.
			std::list<Edge*> listOfUnconstrainedBoundaryEdges;
			CollectUnconstrainedBoundaryEdges(
				cdt,
				listOfUnconstrainedBoundaryEdges
			);

			// Visit all  regions that  are incident to  the collected
			// edges.
			_trimmedFlag = true;
			for (
				std::list<Edge*>::const_iterator eit = listOfUnconstrainedBoundaryEdges.begin();
				eit != listOfUnconstrainedBoundaryEdges.end();
				eit++
				)
			{
				// Get the bounded face to which the edge belongs.
				Face* f = (*eit)->GetFace();

				// Make sure the face is bounded.
				if (!f->IsBounded())
				{
					throw std::runtime_error("Attempt to access an unbounded face when a bounded face is expected");
				}

				// Visit the region the current face belongs to if the
				// region  has not  been visited  before.  If  it has,
				// then skip the region visit.
				if (_visitedTriangles.find(f) == _visitedTriangles.end())
				{
					VisitFacesInRegion(f);
				}
			}

			return;
		}


		/**
		 * \fn void RegionVisitor::VisitOuterNonTrimmedRegion(CDT& cdt)
		 *
		 * \brief Visits all outer, non-trimmed regions of the CDT.
		 *
		 * \param  cdt  A  CDT   represented  by  the  quad-edge  data
		 * structure.
		 *
		 */
		void
			RegionVisitor::VisitOuterNonTrimmedRegion(
				CDT& cdt
			)
		{
			// Collect  all  constrained  boundary  edges  of  bounded
			// faces.
			std::list<Edge*> listOfConstrainedBoundaryEdges;
			CollectConstrainedBoundaryEdges(
				cdt,
				listOfConstrainedBoundaryEdges
			);

			// Visit all outer non-trimmed away regions.
			_trimmedFlag = false;
			for (
				std::list<Edge*>::const_iterator eit = listOfConstrainedBoundaryEdges.begin();
				eit != listOfConstrainedBoundaryEdges.end();
				eit++
				)
			{
				// Get the bounded face to which the edge belongs.
				Face* f = (*eit)->GetFace();

				// Make sure the face is bounded.
				if (!f->IsBounded())
				{
					throw std::runtime_error("Attempt to access an unbounded face when a bounded face is expected");
				}

				// Visit the region the current face belongs to if the
				// region  has not  been visited  before.  If  it has,
				// then skip the region visit.
				if (_visitedTriangles.find(f) == _visitedTriangles.end())
				{
					VisitFacesInRegion(f);
				}
			}

			return;
		}


		/**
		 * \fn void RegionVisitor::VisitInteriorRegions(CDT& cdt)
		 *
		 * \brief Visits all interior regions of the CDT.
		 *
		 * \param  cdt  A  CDT   represented  by  the  quad-edge  data
		 * structure.
		 *
		 */
		void
			RegionVisitor::VisitInteriorRegions(
				CDT& cdt
			)
		{
			// Create a  list with the remaining  constrained edges of
			// bounded faces;  that is,  the last of  all constrained,
			// interior edges of the CDT.
			std::list<Edge*> listOfConstrainedInteriorEdges;
			CollectConstrainedInteriorEdges(
				cdt,
				listOfConstrainedInteriorEdges
			);

			// Visit all inner regions of the CDT.
			bool done;
			do
			{
				// Set the flag to end the search.
				done = true;

				// Visit all regions that  contain an edge incident on
				// a previously  visited region (which is  region that
				// contains the edge mate).
				for (
					std::list<Edge*>::const_iterator eit = listOfConstrainedInteriorEdges.begin();
					eit != listOfConstrainedInteriorEdges.end();
					eit++
					)
				{
					// Get the bounded face to which the edge belongs.
					Face* f = (*eit)->GetFace();

					// Make sure the face and its mate are bounded.
					if (!f->IsBounded() || !(*eit)->Symmetric()->GetFace()->IsBounded())
					{
						throw std::runtime_error("Attempt to access an unbounded face when a bounded face is expected");
					}

					// Visit the region the current face belongs to if
					// the region has not  been visited before.  If it
					// has, then skip the visit.
					if (_visitedTriangles.find(f) == _visitedTriangles.end())
					{
						// Get the bounded face containing the mate of
						// the edge.
						Face* fmate = (*eit)->Symmetric()->GetFace();

						// Check  whether the  face  has already  been
						// visited.
						if (_visitedTriangles.find(fmate) != _visitedTriangles.end())
						{
							// Current  region  is  a  neighbor  of  a
							// previously  visited  region.   So,  the
							// current region  can be visited  at this
							// point.

							// The neighboring regions  cannot be both
							// trimmed regions or non-trimmed regions.
							// That  is,  all  neighbors of  the  same
							// trimmed  region   must  be  non-trimmed
							// regions and vice-versa.
							_trimmedFlag = !fmate->IsMarked();

							// Visit region.
							VisitFacesInRegion(f);
							done = false;
						}
					}
				}
			} while (!done);

			return;
		}


		/**
		 * \fn void RegionVisitor::CollectConstrainedBoundaryEdges(CDT& cdt, std::list<Edge*>& listOfConstrainedBoundaryEdges)
		 *
		 * \brief Collects all constrained  boundary edges that belong
		 * to bounded faces of a given CDT.
		 *
		 * \param  cdt  A  CDT   represented  by  the  quad-edge  data
		 * structure.
		 * \param listOfConstrainedBoundaryEdges A reference to a list
		 * of edges.
		 *
		 */
		void
			RegionVisitor::CollectConstrainedBoundaryEdges(
				CDT& cdt,
				std::list<Edge*>& listOfConstrainedBoundaryEdges
			)
			const
		{
			// Loop over all edges in the list of edges of this CDT.
			for (CDT::EdgeIterator eit = cdt.EdgesBegin(); eit != cdt.EdgesEnd(); ++eit)
			{
				// Get the current edge.
				Edge* e = *eit;

				// If the edge belongs to an unbounded face, ignore it.
				if (e->GetFace()->IsBounded())
				{
					// If the  edge is a  contrained edge and  it also
					// belongs to  the boundary of the  convex hull of
					// the CDT, then insert it into the list.
					if (!e->Symmetric()->GetFace()->IsBounded() && e->IsConstrained())
					{
						listOfConstrainedBoundaryEdges.push_back(e);
					}
				}
			}

			return;
		}


		/**
		 * \fn void RegionVisitor::CollectUnconstrainedBoundaryEdges(CDT& cdt, std::list<Edge*>& listOfUnconstrainedBoundaryEdges)
		 *
		 * \brief  Collects  all  unconstrained  boundary  edges  that
		 * belong to bounded faces of a given CDT.
		 *
		 * \param  cdt  A  CDT   represented  by  the  quad-edge  data
		 * structure.
		 * \param  listOfUnconstrainedBoundaryEdges A  reference to  a
		 * list of edges.
		 *
		 */
		void
			RegionVisitor::CollectUnconstrainedBoundaryEdges(
				CDT& cdt,
				std::list<Edge*>& listOfUnconstrainedBoundaryEdges
			)
			const
		{
			// Loop over all edges in the list of edges of this CDT.
			for (CDT::EdgeIterator eit = cdt.EdgesBegin(); eit != cdt.EdgesEnd(); ++eit)
			{
				// Get the current edge.
				Edge* e = *eit;

				// If the  edge belongs  to an unbounded  face, ignore
				// it.
				if (e->GetFace()->IsBounded())
				{
					// If  the edge  belongs  to the  boundary of  the
					// convex hull of the CDT, then insert it into one
					// of the two temporary lists.
					if (!e->Symmetric()->GetFace()->IsBounded() && !e->IsConstrained())
					{
						listOfUnconstrainedBoundaryEdges.push_back(e);
					}
				}
			}

			return;
		}


		/**
		 * \fn void RegionVisitor::CollectConstrainedInteriorEdges(CDT& cdt, std::list<Edge*>& listOfConstrainedInteriorEdges) const
		 *
		 * \brief Collects all constrained,  interior edges of a given
		 * CDT.
		 *
		 * \param  cdt  A  CDT   represented  by  the  quad-edge  data
		 * structure.
		 * \param listOfConstrainedInteriorEdges A reference to a list
		 * of edges.
		 *
		 */
		void 
			RegionVisitor::CollectConstrainedInteriorEdges(
				CDT& cdt,
				std::list<Edge*>& listOfConstrainedInteriorEdges
			)
			const
		{
			// Loop over all edges in the list of edges of this CDT.
			for (CDT::EdgeIterator eit = cdt.EdgesBegin(); eit != cdt.EdgesEnd(); ++eit)
			{
				// Get the current edge.
				Edge* e = *eit;

				// If the edge is  a constrained interior edge, insert
				// it into the list.
				if (e->GetFace()->IsBounded() && e->Symmetric()->GetFace()->IsBounded() && e->IsConstrained())
				{
					listOfConstrainedInteriorEdges.push_back(e);
				}
			}

			return;
		}


		/**
		 * \fn void RegionVisitor::VisitFacesInRegion(Face* f)
		 *
		 * \brief Visits all faces in the same region of a given face.
		 *
		 * \param f A pointer to a face that has not been visited yet.
		 *
		 */
		void
			RegionVisitor::VisitFacesInRegion(
				Face* f
			)
		{
			// Make sure the face is bounded.
			if (!f->IsBounded())
			{
				throw std::runtime_error("Attempt to access an unbounded face when a bounded face is expected");
			}

			// Create a set of reachable triangles.
			TRIANGLESET reachedTriangles;

			// Visit  all faces  in  a breadth-first  traverse of  the
			// region faces.
			std::list<Face*> queue;
			queue.push_back(f);

			std::pair<TRIANGLESET::iterator, bool> ret =
				reachedTriangles.insert(f);

			if (!ret.second)
			{
				throw std::runtime_error("Attempt to insert the same face in a dictionary twice");
			}

			while (!queue.empty())
			{
				// Get the first face of queue.
				Face* firstFace = queue.front();
				queue.pop_front();

#ifdef DEBUGMODE
				if (_visitedTriangles.find(firstFace) != _visitedTriangles.end())
				{
					throw std::runtime_error("Attempt to visit the same face for the second time");
				}
#endif

				// Visit the face.
				firstFace->Accept(this);

				// Get the three edges of the face.
				Edge* e1 = firstFace->GetEdge();

#ifdef DEBUGMODE
				if ((e1 == nullptr) || (e1->GetFace() != firstFace))
				{
					throw std::runtime_error("Quadedge has inconsistent information");
				}
#endif

				Edge* e2 = e1->LeftNext();

#ifdef DEBUGMODE
				if ((e2 == nullptr) || (e2->GetFace() != firstFace) || (e2->LeftPrev() != e1))
				{
					throw std::runtime_error("Quadedge has inconsistent information");
				}
#endif

				Edge* e3 = e2->LeftNext();

#ifdef DEBUGMODE
				if ((e3 == nullptr) || (e3->GetFace() != firstFace) || (e3->LeftNext() != e1) || (e3->LeftPrev() != e2))
				{
					throw std::runtime_error("Quadedge has inconsistent information");
				}
#endif

				if (!e1->IsConstrained())
				{
					Face* nextFace = e1->Symmetric()->GetFace();
					if (nextFace->IsBounded())
					{
						if (reachedTriangles.find(nextFace) == reachedTriangles.end())
						{
							queue.push_back(nextFace);
#ifdef DEBUGMODE
							// Make  sure face  has  not been  visited
							// before.
							std::pair<TRIANGLESET::iterator, bool> ret = reachedTriangles.insert(nextFace);
							if (!ret.second)
							{
								throw std::runtime_error("Attempt to visit the same face for the second time");
							}
#else
							reachedTriangles.insert(nextFace);
#endif
						}
					}
				}

				if (!e2->IsConstrained())
				{
					Face* nextFace = e2->Symmetric()->GetFace();
					if (nextFace->IsBounded())
					{
						if (reachedTriangles.find(nextFace) == reachedTriangles.end())
						{
							queue.push_back(nextFace);
#ifdef DEBUGMODE
							// Make  sure face  has  not been  visited
							// before.
							std::pair<TRIANGLESET::iterator, bool> ret = reachedTriangles.insert(nextFace);
							if (!ret.second)
							{
								throw std::runtime_error("Attempt to visit the same face for the second time");
							}
#else
							reachedTriangles.insert(nextFace);
#endif
						}
					}
				}

				if (!e3->IsConstrained())
				{
					Face* nextFace = e3->Symmetric()->GetFace();
					if (nextFace->IsBounded())
					{
						if (reachedTriangles.find(nextFace) == reachedTriangles.end())
						{
							queue.push_back(nextFace);
#ifdef DEBUGMODE
							// Make  sure face  has  not been  visited
							// before.
							std::pair<TRIANGLESET::iterator, bool> ret = reachedTriangles.insert(nextFace);
							if (!ret.second)
							{
								std::pair<TRIANGLESET::iterator, bool> ret = reachedTriangles.insert(nextFace);
							}
#else
							reachedTriangles.insert(nextFace);
#endif
						}
					}
				}
			}

			return;
		}

	}

}

/** @} */ //end of group class.
