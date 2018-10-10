#include "MeshModel.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Scene.h"
#include <sstream>
#define FACE_ELEMENTS 3


#include <glm/gtc/type_ptr.hpp> //for uniform setting
int find(vector<glm::vec4> vec, glm::vec4 val)
{
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == val)
			return i;
	return -1;
}
using namespace std;
MeshModel::MeshModel()
{
	
}
void MeshModel::setModeltransform(glm::mat4x4 transform)
{
	modelTransform = transform ;
}
void MeshModel::transformModel(glm::mat4x4 transform)
{
	modelTransform = transform* modelTransform;
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
	delete[] faceNormals;
	delete[] faceAvgs;
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
	//just to make sure, the vector is defined
	

	LoadFile(fileName);
	setBound(); //bounding box
	setFaceNormals(); //faces' normals
	willDrawBox = 0;
	willDrawFaceNormal = 0;
	willDrawFaceNormal = 0;

	
	initVaoModel(); //make our model ready to be drawn!!!

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
	norm_num = 0;
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
			norm_num++;
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
	
	load_normal_per_vertex();
	
}
void MeshModel::load_normal_per_vertex()
{
	normalPositions2=new glm::vec4[vertexPosNum];
	vector<glm::vec4> norm_ver;
	vector<glm::vec4> sum_ver;
	vector<int> count_ver;
	vector<glm::vec4> vertex;
	for (int i = 0; i < this->vertexPosNum; i++)
	{
		//now each normal is in the same place of his vertex (k)
		glm::vec4 norm = normalPositions[i];
		glm::vec4 ver = vertexPositions[i];
		int place = find(vertex, ver);
		if (place >= 0)
			sum_ver[place] += norm, count_ver[place]++;
		else
		{
			vertex.push_back(ver);
			sum_ver.push_back(norm);
			count_ver.push_back(1);
		}
	}

	for (int i = 0; i < this->vertexPosNum; i++)
	{
		glm::vec4 ver = vertexPositions[i];
		int place = find(vertex, ver);
		glm::vec4 norm = sum_ver[place] / float(count_ver[place]);
		this->normalPositions2[i]=norm;
	}
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

void MeshModel::DrawOpenGL(unsigned int shaderProgram, int index, Scene* scene, glm::mat4 cameraTrans, glm::mat4 camProject)
{
	/*
	openGL pipeline: --to make the vertices NDC (normalized Device Coordinates)
	1. modelTransform
	2. cameraTrans^-1
	3. worldTransform
	4. projection
	5. divide by .w (will be done in the shader)
	*/

	/*  vertex trans  */
	// --------------
	glm::mat4 view = this->worldTransform * glm::inverse(cameraTrans);
	glm::mat4 model = this->modelTransform;
	glm::mat4 finalTRANS = camProject * view * model;

	//sending our finalTRANS to the vshader
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transformPos");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalTRANS));

	/* normal trans  */
	//--------------
	glm::mat4x4 mv = view * model;
	glm::mat4x4 finalNormalMatrix = glm::transpose(glm::inverse(mv)); // (M^-1)^T
	
	transformLoc = glGetUniformLocation(shaderProgram, "transformNorm");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(finalNormalMatrix));
	
	
	/*  light data  */
	/*
	uniform vec3 exp[15];
	uniform int active_ligths_arry_size;
	uniform vec3 pos_dir[15];
	uniform vec3 view_dir;
	*/
	glm::vec3 am_color, diff_color, spect_color;
	unsigned int uniformLoc;

	am_color = scene->AMcolors_model[index];
	uniformLoc = glGetUniformLocation(shaderProgram, "am_color");
	glUniform3f(uniformLoc, am_color.x, am_color.y, am_color.z);

	diff_color = scene->Difcolors_model[index];
	uniformLoc = glGetUniformLocation(shaderProgram, "dif_color");
	glUniform3f(uniformLoc, diff_color.x, diff_color.y, diff_color.z);

	spect_color = scene->SPECTcolors_model[index];
	uniformLoc = glGetUniformLocation(shaderProgram, "spec_color");
	glUniform3f(uniformLoc, spect_color.x, spect_color.y, spect_color.z);

	glm::vec3 am_ligth = scene->ambient * scene->strengte_ambient;
	uniformLoc = glGetUniformLocation(shaderProgram, "am_ligth");
	glUniform3f(uniformLoc, am_ligth.x, am_ligth.y, am_ligth.z);

	glm::vec3 dif_ligth[15], spec_ligth[15];
	for (int i = 0; i < fmin(scene->lights.size(), 15); i++)
	{
		dif_ligth[i] = scene->lights[i]->difus * scene->lights[i]->strengte_difus;
		spec_ligth[i] = scene->lights[i]->specalar * scene->lights[i]->strengte_specalar;
	}
	uniformLoc = glGetUniformLocation(shaderProgram, "dif_ligth");
	glUniform3fv(uniformLoc, 15, glm::value_ptr(dif_ligth[0]));
	uniformLoc = glGetUniformLocation(shaderProgram, "spec_ligth");
	glUniform3fv(uniformLoc, 15, glm::value_ptr(spec_ligth[0]));

	uniformLoc = glGetUniformLocation(shaderProgram, "exp");
	glUniform1i(uniformLoc, Specularity_exponent);
	
	uniformLoc = glGetUniformLocation(shaderProgram, "active_ligths_arry_size");
	glUniform1i(uniformLoc, fmin(scene->lights.size(), 15));

	glm::vec3 pos_dir[15];
	GLint ligth_type[15];
	for (int i = 0; i < fmin(scene->lights.size(), 15); i++)
	{
		ligth_type[i] = scene->lights[i]->type;

		if (scene->lights[i]->type)
		{
			pos_dir[i] =  glm::vec4(scene->lights[i]->direction, 0);
			pos_dir[i] = glm::normalize(-pos_dir[i]);
			//std::cout << pos_dir[i].x  <<" "<<  pos_dir[i].y  << " " << pos_dir[i].z <<'\n';
		}
		else
			pos_dir[i] =glm::vec4(scene->lights[i]->getPosition(),1);
	}
	uniformLoc = glGetUniformLocation(shaderProgram, "pos_dir");
	glUniform3fv(uniformLoc, 15, glm::value_ptr(pos_dir[0]));
	uniformLoc = glGetUniformLocation(shaderProgram, "ligth_type");
	glUniform1iv(uniformLoc, 15, ligth_type);


	/* 
	glm::vec4 v55 = glm::inverse(models[i]->getModelTransform())
			*(cameras[ActiveCamera]->pos - cameras[ActiveCamera]->at);
	*/


	glm::vec4 view_dir = scene->cameras[scene->ActiveCamera]->pos - scene->cameras[scene->ActiveCamera]->at;
	view_dir = glm::normalize(view_dir);
	uniformLoc = glGetUniformLocation(shaderProgram, "view_dir"); 
	glUniform3f(uniformLoc, view_dir.x, view_dir.y, view_dir.z);


	uniformLoc = glGetUniformLocation(shaderProgram, "norm_as_color");
	glUniform1i(uniformLoc,norm_as_color);

	
	glUniform1i(glGetUniformLocation(shaderProgram, "phong"),scene->type);
	glUniform1i(glGetUniformLocation(shaderProgram, "auto_texture"), this->TEXTURE);
	//glUniform1i(glGetUniformLocation(shaderProgram, "phong_f"), scene->type);

	//we're ready. now call the shaders!!!
	glBindVertexArray(VAO); //bind our vao
	glDrawArrays(GL_TRIANGLES, 0, vertexPosNum);
	glBindVertexArray(0); //unbind our vao


}


//openGL help functions

//this will create our vao with all the vertex buffers
void MeshModel::initVaoModel()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, buffers);
	glBindVertexArray(VAO); //bind this for vao's buffers init

	GLint sizeVertices = getVertexPosNum();

	//  vertex buffer:
	/*-----------------*/
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sizeVertices, &(vertexPositions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); //position attributes
	glEnableVertexAttribArray(0); //#0 attribute


	//  normal buffer:
	//-----------------
	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * sizeVertices, &(this->normalPositions2[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); //normal per vertex attributes
	glEnableVertexAttribArray(1); //#1 attribute
	

	//  tex' coor buffer:
	/*-----------------*/
	//have to support text' in LoadFile for this code... :/
	



	//finally, unbind our vao & vbo, just incase.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}
//this will just bind our vao :)
void MeshModel::bindVaoModel()
{
	glBindVertexArray(this->VAO); //just bind. it has all of our buffers!
}
