#include "MeshModel.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include <sstream>
#define FACE_ELEMENTS 3

using namespace std;
void MeshModel::transformModel(glm::mat4x4 transform)
{

	modelTransform = transform* modelTransform;
	//for (int i = 0; i < getVertexPosNum(); i++)
		//vertexPositions[i] = vertexPositions[i] * transform;
}
void MeshModel::transformWorld(glm::mat4x4 transform)
{
	worldTransform = transform * worldTransform;
}

MeshModel::~MeshModel()
{
	/*
	glm::vec4 *vertexPositions;
	glm::vec4 *normalPositions;
	glm::vec4 *faceNormals;
	glm::vec4 *faceAvgs;
	*/

	delete[] vertexPositions;
	delete[] normalPositions;
	//delete[] faceNormals;
	//delete[] faceAvgs;
}


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
	setBound(); //bounding box
	setFaceNormals(); //faces' normals
	willDrawBox = 0;
	willDrawFaceNormal = 0;
	willDrawFaceNormal = 0;
}


void MeshModel::setFaceNormals()
{
	//calculate normals' direction (n) and starting point (avg)

	glm::vec4 *vP = this->vertexPositions;
	glm::vec4 *vN = this->normalPositions;
	//first, calculate avgs

	//#vertexPositions divide by 3 - #faces
	int size = this->vertexPosNum / 3;
	this->faceAvgs = new glm::vec4[size];

	glm::vec4 avg = glm::vec4();
	for (int f = 0; f < size; f++)
	{
		avg.x = (vP[(3 * f) + 0].x + vP[(3 * f) + 1].x + vP[(3 * f) + 2].x) / 3;
		avg.y = (vP[(3 * f) + 0].y + vP[(3 * f) + 1].y + vP[(3 * f) + 2].y) / 3;
		avg.z = (vP[(3 * f) + 0].z + vP[(3 * f) + 1].z + vP[(3 * f) + 2].z) / 3;
		avg.w = 1.0f; //dont forget the w!
		this->faceAvgs[f] = avg;
	}


	//second, calculate normals
	this->faceNormals = new glm::vec4[size];

	//glm::vec3 normal3 = glm::vec3();
	glm::vec4 normal4 = glm::vec4();
	//glm::vec3 ab = glm::vec3(), ac = glm::vec3();
	for (int f = 0; f < size; f++)
	{
		/*
		// normal = AB x AC --- multiply them by A LOT so it wouldn't reach 0
		ab = 1000.0f * vP[3 * f + 1] - 1000.0f * vP[3 * f]; //b - a
		ac = 1000.0f * vP[3 * f + 2] - 1000.0f * vP[3 * f]; //c - a
		normal3 = glm::cross(ab, ac); //ab x ac
		
		normal4 = glm::vec4(normal3.x, normal3.y, normal3.z, 1.0f); //w=1

		this->faceNormals[f] = normal4;
		*/

		normal4.x = (vN[3 * f + 0].x + vN[3 * f + 1].x + vN[3 * f + 2].x) / 3;
		normal4.y = (vN[3 * f + 0].y + vN[3 * f + 1].y + vN[3 * f + 2].y) / 3;
		normal4.z = (vN[3 * f + 0].z + vP[3 * f + 1].z + vN[3 * f + 2].z) / 3;
		normal4.w = 1.0f; //dont forget the w!
		this->faceNormals[f] = normal4;

	}

}
void MeshModel::setBound()
{
	glm::vec4* vP = vertexPositions;
	
	float xMaxV = vP[0].x, xMinV = vP[0].x;
	float yMaxV = vP[0].y, yMinV = vP[0].y;
	float zMaxV = vP[0].z, zMinV = vP[0].z;
	
	for (int i = 0; i < vertexPosNum; i++)
	{
		//x:
		if (vP[i].x >= xMaxV)
		{
			xMaxV = vP[i].x;
			this->xMax = i;
		}
		if (vP[i].x <= xMinV)
		{
			xMinV = vP[i].x;
			this->xMin = i;
		}
		//y:
		if (vP[i].y >= yMaxV)
		{
			yMaxV = vP[i].y;
			this->yMax = i;
		}
		if (vP[i].y <= yMinV)
		{
			yMinV = vP[i].y;
			this->yMin = i;
		}
		//z:
		if (vP[i].z >= zMaxV)
		{
			zMaxV = vP[i].z;
			this->zMax = i;
		}
		if (vP[i].z <= zMinV)
		{
			zMinV = vP[i].z;
			this->zMin = i;
		}

	}
	int test = 0;
}


int MeshModel::getVertexPosNum()
{
	return this->vertexPosNum;
}
glm::vec4* MeshModel::getNormalFace()
{
	return this->faceNormals;
}
glm::vec4* MeshModel::getFaceAvgs()
{
	return this->faceAvgs;
}
glm::mat4x4& MeshModel::getModelTransform()
{
	return this->modelTransform;
}
glm::mat4x4& MeshModel::getWorldTransform()
{
	return this->worldTransform;
}
glm::mat4x4& MeshModel::getNormalTransform()
{
	return this->normalTransform;
}

void MeshModel::LoadFile(const string& fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdx> faces;
	vector<glm::vec4> vertices;
	vector<glm::vec4> normals;
	//trying to memory problems around stack
	string lineType;
	istringstream* issLine;
	string curLine;

	float x, y, z;

	//take nameModel from fileName
	int lastIndex = fileName.find_last_of("/");
	// -5 so it won't have the .obj suffix
	this->nameModel = fileName.substr(lastIndex + 1, fileName.length() - lastIndex - 5);

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
			//read the 3d point and make it 4d (homogenous)
			vertices.push_back( glm::vec4(vec3fFromStream(*issLine), 1.0f) );
		}
		else if (lineType == "vn")
		{
			//should have w = 0?
			normals.push_back( glm::vec4(vec3fFromStream(*issLine), 1.0f) );
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
	this->normalPositions = new glm::vec4[FACE_ELEMENTS * faces.size()];
	// iterate through all stored faces and create triangles
	int k=0;
	glm::vec4 *avoidDeleting;
	
	for (vector<FaceIdx>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		for (int i = 0; i < FACE_ELEMENTS; i++)
		{
			//--get  vertices[  face's vertex's index minus 1 ]
			this->vertexPositions[k] = vertices[(*it).v[i] - 1]; /*BUG*/ //fixed?
			//--get  normals[  face's normal's index minus 1 ]
			this->normalPositions[k] = normals[(*it).vn[i] - 1]; //**********

			k++;

			//now each normal is in the same place of his vertex (k)
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

	this->modelTransform = glm::mat4x4
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
		0.0f, 0.0f, 0.0f, 0.0f
	);
}
glm::vec4* MeshModel::GetVertex()
{
	return vertexPositions;
}
glm::vec4* MeshModel::getNormalVertex()
{
	return this->normalPositions;
}
glm::vec4* MeshModel::Draw()
{
	/*
	should use "DrawTriangles" function(?)
	also, should use "setObjectMatrices"(?) (before drawing, to first transform the points)
	maybe just return the transformed points to scene so it would draw?
	meshModel doesn't have a renderer!
	*/
	//for testing, dont worry
	glm::vec4* transVertexPositions = new glm::vec4[(unsigned)(this->getVertexPosNum())];
	for (int i = 0; i < (int)(this->getVertexPosNum()); i++)
		transVertexPositions[i] = modelTransform * vertexPositions[i];
	return transVertexPositions;
}
