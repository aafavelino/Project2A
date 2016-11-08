/** 
 * \file Visitor.h
 *  
 * \brief  This class  defines  a generic  visitor  for a  constrained
 * Delaunay  triangulation (CDT)  represented  by  the quad-edge  data
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


/**
 * \defgroup MAT309NameSpace Namespace MAT309.
 * @{
 */

/**
 * \namespace MAT309
 *
 * \brief Global namespace for Velo3D software.
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
		 * Forward declaration of classes.
		 */
		class QuadEdge;
		class Edge;
		class Vertex;
		class Face;


		/**
		 * \class Visitor
		 *
		 * \brief  This  class   is  a  generic  visitor   for  a  CDT
		 * represented  by  an  augmentation  of  the  quad-edge  data
		 * structure.
		 *
		 */
		class Visitor
		{
		public:

			// -------------------------------------------------------
			//
			// Public methods
			//
			// -------------------------------------------------------

			/**
			 * \fn Visitor()
			 *
			 * \brief Creates an instance of this class.
			 *
			 */
			Visitor()
			{
			}

			/**
			 * \fn virtual void Visit (Face* f) = 0
			 *
			 * \brief A generic visitor method to visit a face.
			 *
			 * \param f A pointer to a face.
			 *
			 */
			virtual 
			void 
				Visit(
					Face* f
				)
				= 0;


			/**
			 * \fn virtual void Visit (QuadEdge* qe) = 0
			 *
			 * \brief A generic visitor method to visit a quad-edge.
			 *
			 * \param qe A pointer to a quad-edge.
			 *
			 */
			virtual 
			void
				Visit(
					QuadEdge* qe
				) 
				= 0;


			/**
			 * \fn virtual void Visit (Edge* e) = 0
			 *
			 * \brief A generic visitor method to visit an edge.
			 *
			 * \param e A pointer to an edge.
			 *
			 */
			virtual
			void
				Visit(
					Edge* e
				)
				= 0;


			/**
			 * \fn virtual void Visit (Vertex* v) = 0
			 *
			 * \brief A generic visitor method to visit a vertex.
			 *
			 * \param v A pointer to a vertex.
			 *
			 */
			virtual
			void
				Visit(
					Vertex* v
				)
				= 0;

		};

	}

}

/** @} */ //end of group class.
