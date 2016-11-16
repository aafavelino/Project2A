/**
 * \file main.cpp
 *
 * \brief Explains what your program does
 *
 * \author
 * Adelino Afonso e Irene Ginani \n
 * Universidade Federal do Rio Grande do Norte, \n
 * Instituto Metr&oacute;pole Digital, \n
 * Adelino Afonso at imd (dot) ufrn (dot) br, \n
 * Irene Ginani at imd (dot) ufrn (dot) br
 *
 * \version 1.0
 * \date October 2016
 *
 * \attention This program is distributed WITHOUT ANY WARRANTY, and it
 *            may be freely redistributed under the condition that the
 *            copyright notices  are not removed,  and no compensation
 *            is received. Private, research, and institutional use is
 *            free. Distribution of this  code as part of a commercial
 *            system  is permissible ONLY  BY DIRECT  ARRANGEMENT WITH
 *            THE AUTHOR.
 */

// -------------------------------------------------------------------
//
// POR FAVOR, NÃO EDITE ESTE  ARQUIVO, exceto na parte indicada abaixo
// na qual eu defino a superfície que será amostrada. Você pode trocá-
// por outra. Para tal, não esqueça  de incluir o arquivo que define a
// outra superfície.
//
// -------------------------------------------------------------------

#include <iostream>                 // std::cout, std::endl, std::cerr
#include <fstream>                  // std::ifstream, std::ofstream
#include <string>                   // std::string
#include <cstdlib>                  // exit, atoi, EXIT_SUCCESS, EXIT_FAILURE
#include <iomanip>                  // std::setprecision
#include <cassert>                  // assert
#include <typeinfo>                 // std::bad_cast
#include <vector>                   // std::vector
#include <memory>                   // std::shared_ptr
#include <exception>                // std::exception


#include "Cdt.h"                    // cdt::CDT
#include "DtPoint.h"                // cdt::DtPoint
#include "FaceVisitor.h"            // cdt::FaceVisitor

#include "SamplePoint.h"            // MAT309::SamplePoint
#include "PDSampler.h"              // MAT309::PDSampler

using MAT309::cdt::CDT;
using MAT309::cdt::DtPoint;
using MAT309::cdt::FaceVisitor;
using MAT309::SamplePoint;
using MAT309::PDSampler;


/********************************************************************/

// -------------------------------------------------------------------
// Se  você quiser  testar  seu  código com  uma  superfície que  você
// definiu, comente  o "include" abaixo  e o adicione o  "include" que
// contém o arquivo  no qual a sua superfície  está definida. Descubra
// como definir a sua superfície examinando o código de MyCylinder.h.
// -------------------------------------------------------------------

//#include "MyCylinder.h"             // MAT309::MyCylinder
//#include "Esfera.h"				// MAT309::Esfera	
#include "Cone.h"					// MAT309::Cone
// -------------------------------------------------------------------
// Opte por inserir um "using" para a sua classe de superfície.
// -------------------------------------------------------------------

using MAT309::Cone;
//using MAT309::Esfera;
//using MAT309::MyCylinder;

/********************************************************************/


/**
 * \typedef spPoint
 *
 * \brief Alias for a shared pointer to DtPoint objects.
 *
 */
typedef DtPoint::spPoint spPoint;


/**
 * \typedef spSamplePoint
 *
 * \brief Alias for a shared pointer to SamplePoint objects.
 *
 */
typedef SamplePoint::spSamplePoint spSamplePoint;


/**
 * \fn void WritePatchTriangulation(const std::string& filename, size_t numberOfTriangles, const std::vector<size_t>& triangles, size_t numberOfVertices, const std::vector<spPoint>& vertices)
 *
 * \brief Write the surface patch triangulation to an OFF file.
 *
 * \param filename The name of a file to be written to.
 * \param numberOfTriangles Total number of triangles of the CDT.
 * \param triangles An array with vertex indices of all triangles.
 * \param numberOfVertices Total number of vertices of the CDT.
 * \param vertices An array to store all vertices of the CDT.
 *
 */
void WritePatchTriangulation(
	const std::string& filename,
	size_t numberOfTriangles,
	const std::vector<size_t>& triangles,
	size_t numberOfVertices,
	const std::vector<spPoint>& vertices
);


/**
 * \fn int main( int argc , char* argv[]  )
 *
 * \brief Computes  the constrained Delaunay triangulation  (CDT) of a
 * given set of points and  segments connecting two points. The points
 * were obtained from a NURBS  surface patch, and the segments connect
 * points  sampled  from  the   boundary  curves  (including  trimming
 * curves).
 *
 * \param argc The number of command-line arguments.
 * \param argv An array with the command-line arguments.
 *
 * \return An integer number.
 */
int main(int argc, char* argv[])
{
	// Check command-line arguments.
	if (argc != 2)
	{
		std::cerr << "Usage: "
				<< std::endl
				<< "\t\t CDT arg1"
				<< std::endl
				<< "\t\t arg1: name of the output file describing the triangulation of the sample points."
				<< std::endl
				<< std::endl;
		return EXIT_FAILURE;
	}

	// Sample the surface.
	std::cout	<< std::endl
				<< "Sampling a surface using Poisson Disk Sampling..."
				<< std::endl;
	std::cout.flush();

	size_t numberOfPoints;               // The number of sample points
	std::vector<spPoint> points;         // The information associated with the points (e.g., coordinates)



	/****************************************************************/

	// ---------------------------------------------------------------
	// Se você  quiser testar seu  código com uma superfície  que você
	// definiu, comente a definição que cria um share pointer para uma
	// instância de MyCylinder e defina  o apontador mySurface como um
	// shared pointer para uma instância da classe de sua superfície.
	// ---------------------------------------------------------------
	
	// This  is   the  surface   you  will   sample  using   your  PDS
	// algorithm. You can replace this  surface with other surface you
	// define.
	std::shared_ptr<Cone> mySurface = std::make_shared<Cone>();
	//std::shared_ptr<Esfera> mySurface = std::make_shared<Esfera>();
	//std::shared_ptr<MyCylinder> mySurface = std::make_shared<MyCylinder>();

	// Define the sampler.
	PDSampler sampler;

	// Sample the surface.
	try
	{
		sampler.sample(
					   mySurface,
					   numberOfPoints,
					   points
					  );
	}
	catch (const std::exception& xpt)
	{
	    std::cerr << std::endl
	              << "ERROR: "
		          << xpt.what()
				  << std::endl
		          << std::endl;
	    return EXIT_FAILURE;
	}

	// Compute the CDT of the given set of points and segments.
	std::cout << "Compute the Constrained Delaunay Triangulation (CDT)... "
		<< std::endl;
	std::cout.flush();

	CDT* myCdt = nullptr;
	try
	{
	  myCdt = new CDT(
					  numberOfPoints,
					  points
					 );
	}
	catch (const std::exception& xpt)
	{
	    std::cerr << std::endl
	              << "ERROR: "
		          << xpt.what()
				  << std::endl
		          << std::endl;
	    return EXIT_FAILURE;
	}

	// Obtain all edges and vertices of the CDT.
	std::cout << "Collecting all edges and vertices of the CDT..."
			  << std::endl;
	std::cout.flush();

	size_t numberOfVertices;
	std::vector<spPoint> vertices;

	size_t numberOfEdges;
	std::vector<size_t> edges;

	size_t  numberOfTriangles;
	std::vector<size_t> triangles;

	// Won't use this variable, but we must declare it!
	std::vector<FaceVisitor::EdgeType> edgeType;

	FaceVisitor visitor;
	visitor.Visit(
		*myCdt,
		numberOfTriangles,
		triangles,
		numberOfEdges,
		edges,
		edgeType,
		numberOfVertices,
		vertices
	);

	// Write the patch triangulation to an OFF output file.
	std::cout	<< "Writing patch triangulation to an OFF file..."
				<< std::endl;
	std::cout.flush();

	std::string fn(argv[1]);

	WritePatchTriangulation(
		fn,
		numberOfTriangles,
		triangles,
		numberOfVertices,
		vertices
	);

	// Release memory
	std::cout	<< "Releasing memory..."
				<< std::endl;
	std::cout.flush();

	delete myCdt;

	std::cout	<< "Finished."
				<< std::endl
				<< std::endl
				<< std::endl;
	std::cout.flush();

	return EXIT_SUCCESS;
}


/**
 * \fn void WritePatchTriangulation(const std::string& filename, size_t numberOfTriangles, const std::vector<size_t>& triangles, size_t numberOfVertices, const std::vector<spPoint>& vertices)
 *
 * \brief Write the NURBS surface patch triangulation to an OFF file.
 *
 * \param filename The name of a file to be written to.
 * \param numberOfTriangles Total number of triangles of the CDT.
 * \param triangles An array with vertex indices of all triangles.
 * \param numberOfVertices Total number of vertices of the CDT.
 * \param vertices An  array to store all vertices of the CDT.
 *
 */
void WritePatchTriangulation(
	const std::string& filename,
	size_t numberOfTriangles,
	const std::vector<size_t>& triangles,
	size_t numberOfVertices,
	const std::vector<spPoint>& vertices
)
{
	// Change the file extension.
	std::string offFileName(filename);
	std::string extension("vtk");

	if (
		(extension.size() <= offFileName.size())
		&&
		std::equal(extension.rbegin(), extension.rend(), offFileName.rbegin())
	   )
	{
		offFileName.replace(offFileName.end() - 3, offFileName.end(), "off");
	}
	else
	{
		std::cerr << std::endl
			<< "ERROR: output filename does not have extension .vtk"
			<< std::endl
			<< std::endl;
		exit(EXIT_FAILURE);
	}

	std::ofstream ou(offFileName.c_str());

	if (!ou.is_open())
	{
		std::cerr << std::endl
			<< "ERROR: output file cannot be created."
			<< std::endl
			<< std::endl;
		exit(EXIT_FAILURE);
	}

	// Print out the edge and vertex information to an OFF file.
	ou << std::setprecision(18) << std::fixed;

	// Print out the file header.
	ou << "OFF"
		<< std::endl
		<< numberOfVertices
		<< '\t'
		<< numberOfTriangles
		<< "\t0"
		<< std::endl;

	// Print out the vertex coordinates.
	for (size_t i = 0; i < numberOfVertices; i++)
	{
		spSamplePoint p = std::dynamic_pointer_cast<SamplePoint>(vertices[i]);

		if (p == nullptr)
		{
			std::cerr << "Failed to dynamically cast a shared pointer"
				<< std::endl;
			exit(EXIT_FAILURE);
		}

		ou << p->GetPosition()._x
			<< '\t'
			<< p->GetPosition()._y
			<< '\t'
			<< p->GetPosition()._z
			<< std::endl;
	}

	// Print out the face information.
	for (size_t i = 0; i < numberOfTriangles; i++)
	{
		ou << "3 "
			<< triangles[3 * i]
			<< '\t'
			<< triangles[3 * i + 1]
			<< '\t'
			<< triangles[3 * i + 2]
			<< std::endl;
	}

	// Close file
	ou.close();

	return;
}
