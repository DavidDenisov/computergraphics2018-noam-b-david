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

	glm::vec4* vPi = new glm::vec4[36];
	// *a b c d*
	vPi[0] = a; vPi[1] = b; vPi[2] = d;
	vPi[3] = b; vPi[4] = c; vPi[5] = d;
	// *aUp bUp cUp dUp*
	vPi[6] = aUp; vPi[7] = bUp; vPi[8] = dUp;
	vPi[9] = bUp; vPi[10] = cUp; vPi[11] = dUp;
	// *c d dUp cUp*
	vPi[12] = d; vPi[13] = c; vPi[14] = cUp;
	vPi[15] = d; vPi[16] = dUp; vPi[17] = cUp;
	// *a d dUp aUp*
	vPi[18] = a; vPi[19] = d; vPi[20] = dUp;
	vPi[21] = a; vPi[22] = aUp; vPi[23] = dUp;
	// *a b bUp aUp*
	vPi[24] = a; vPi[25] = b; vPi[26] = bUp;
	vPi[27] = a; vPi[28] = aUp; vPi[29] = bUp;
	// *b c cUp dUp*
	vPi[30] = b; vPi[31] = c; vPi[32] = cUp;
	vPi[33] = b; vPi[34] = bUp; vPi[35] = cUp;

	this->vertexPositions = vPi;
	this->vertexPosNum = 36;
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
int MeshModel::getVertexPosNum()
{
	return this->vertexPosNum;
}

void MeshModel::LoadFile(const string& fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdx> faces;
	vector<glm::vec4> vertices;
	//trying to memory problems around stack
	string lineType;
	istringstream* issLine;
	string curLine;

	float x, y, z;

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		curLine = "";
		getline(ifile, curLine);

		// read the type of the line
		issLine = new istringstream(curLine); //used to be "istringstream issLine(curLine)"
		lineType = ""; //used to be "string lineType"

		*issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v") /*BUG*/ //--changed to "v" because it's a vertex
		{
			//read the 3d point and make it 4d (for later trans)
			vertices.push_back( glm::vec4(vec3fFromStream(*issLine),1.0f) );
			
			//std::cout << "vertex\n"; //testing
		}
		else if (lineType == "f") /*BUG*/ //--changed to "f" because it's a face
		{
			faces.push_back(*issLine); //creates faceIdx object and then pushes it
		
			//std::cout << "face\n"; //testing
		}
		else if (lineType == "#" || lineType == "")
		{
			//std::cout << "hello\n"; // comment / empty line
		}
		else
		{
			//cout << "Found unknown line Type \"" << lineType << "\"";
		}
	}


	//Vertex_positions is an array of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then vertexPositions should contain:
	//vertexPositions={v1,v2,v3,v1,v3,v4}

	this->vertexPosNum = FACE_ELEMENTS * faces.size();
	this->vertexPositions = new glm::vec4[FACE_ELEMENTS * faces.size()]; /*BUG*/ //--changed array size
	// iterate through all stored faces and create triangles
	int k=0;
	glm::vec4 *avoidDeleting;
	for (vector<FaceIdx>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		for (int i = 0; i < FACE_ELEMENTS; i++)
		{
			//--get  vertices[  face's vertex's index minus 1 ]
			this->vertexPositions[k++] = vertices[(*it).v[i] - 1]; /*BUG*/ //fixed? 
		}
	}


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

const glm::vec4* MeshModel::Draw()
{
	/*
	should use "DrawTriangles" function(?)
	also, should use "setObjectMatrices"(?) (before drawing, to first transform the points)
	
	maybe just return the transformed points to scene so it would draw?
	meshModel doesn't have a renderer!
	
	*/

	//for testing, dont worry
	return this->vertexPositions;


	return NULL;
}
