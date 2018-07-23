#include "MeshModel.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define FACE_ELEMENTS 3


using namespace std;

// A struct for processing a single line in a wafefront obj file:
// https://en.wikipedia.org/wiki/Wavefront_.obj_file
struct FaceIdx
{
	// For each of the following 
	// Saves vertex indices
	int v[FACE_ELEMENTS];
	// Saves vertex normal indices
	int vn[FACE_ELEMENTS];
	// Saves vertex texture indices
	int vt[FACE_ELEMENTS];

	FaceIdx()
	{
		for (int i = 0; i < FACE_ELEMENTS + 1; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdx(std::istream& issLine)
	{
		for (int i = 0; i < FACE_ELEMENTS + 1; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		// v0/vt0/vn0 v1/vt1/vn1 
		for(int i = 0; i < FACE_ELEMENTS; i++)  
		{
			issLine >> std::ws >> v[i] >> std::ws;
			if (issLine.peek() != '/')
			{
				continue;
			}
			issLine >> c >> std::ws;  //reads the '/'
			if (issLine.peek() == '/')
			{
				issLine >> c >> std::ws >> vn[i];
				continue;
			}
			else
			{
				issLine >> vt[i];
			}
			if (issLine.peek() != '/')
			{
				continue;
			}
			issLine >> c >> vn[i];
		}
	}
};

glm::vec3 vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

MeshModel::MeshModel(const string& fileName)
{
	LoadFile(fileName);
}

MeshModel::MeshModel() //primitive MeshModel - task2
{
	//let's build a primitive MeshModel, a cube!

	/*
	the (0,0,0) point is in the center of the cube.

	  
	   a'_______  b'
	d'/|_____c'/|
	  ||      | |
	  ||a_____|_|b
	  |/______|/
	  d       c
	*/

	glm::vec4 a(-0.5f, -0.5f, -0.5f, 0.0f), b(0.5f, -0.5f, -0.5f, 0.0f),
		c(0.5f, -0.5f, 0.5f, 0.0f), d(-0.5f, -0.5f, 0.5f, 0.0f);
	
	glm::vec4 aUp(-0.5f, 0.5f, -0.5f, 0.0f), bUp(0.5f, 0.5f, -0.5f, 0.0f),
		cUp(0.5f, 0.5f, 0.5f, 0.0f), dUp(-0.5f, 0.5f, 0.5f, 0.0f);

	glm::vec4 vertexPosi[] =
	{
		// *a b c d*
		a, b, d,
		b, c ,d,
		// *aUp bUp cUp dUp*
		aUp, bUp, dUp,
		bUp, cUp, dUp,
		// *c d dUp cUp*
		d, c, cUp,
		d, dUp, cUp,
		// *a d dUp aUp*
		a, d, dUp,
		a, aUp, dUp,
		// *a b bUp aUp*
		a, b, bUp,
		a, aUp, bUp,
		// *b c cUp dUp*
		b, c, cUp,
		b, bUp, cUp
	};

	this->vertexPositions = vertexPosi;
	//object in the center of the world, identity trans
	this->worldTransform = glm::mat4x4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	//i dont know what it is used for, but we probably dont need it (I hope so)
	this->normalTransform = glm::mat4x4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

MeshModel::~MeshModel()
{
}

void MeshModel::LoadFile(const string& fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdx> faces;
	vector<glm::vec4> vertices;
	
	float x, y, z;

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

		// read the type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v") /*BUG*/ //--changed to "v" because it's a vertex
		{
			//read the 3d point and make it 4d (for later trans)
			vertices.push_back( glm::vec4(vec3fFromStream(issLine),1.0f) );
		}
		else if (lineType == "f") /*BUG*/ //--changed to "f" because it's a face
		{
			faces.push_back(issLine); //creates faceIdx object and then pushes it
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}


	//Vertex_positions is an array of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then vertexPositions should contain:
	//vertexPositions={v1,v2,v3,v1,v3,v4}

	vertexPositions = new glm::vec4[FACE_ELEMENTS * faces.size()]; /*BUG*/ //--changed array size
	// iterate through all stored faces and create triangles
	int k=0;
	for (vector<FaceIdx>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		for (int i = 0; i < FACE_ELEMENTS; i++)
		{
			//--get face's i-point's value from vertices array
			vertexPositions[k++] = vertices[(*it).v[i]]; /*BUG*/ //fixed? 
		}
	}
}

const vector<glm::vec4>* MeshModel::Draw()
{
	/*
	should use "DrawTriangles" function(?)
	also, should use "setObjectMatrices"(?) (before drawing, to first transform the points)
	
	maybe just return the transformed points to scene so it would draw?
	meshModel doesn't have a renderer!
	
	*/



	return NULL;
}
