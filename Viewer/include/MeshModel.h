#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Model.h"

using namespace std;

/*
* MeshModel class. Mesh model object represents a triangle mesh (loaded fron an obj file).
*
*/
class MeshModel : public Model
{
protected:
	glm::vec4 *vertexPositions;
	glm::vec4 *normalPositions; //normal per face per vertex
	vector<glm::vec4> normalPositions2; //only normal per vertex - each vertex will have the same value for every face he appears in
	glm::vec4 *faceNormals;
	glm::vec4 *faceAvgs;
	// Add more attributes.
	int vertexPosNum; // 3 * #vertices
	
	
	glm::mat4x4 worldTransform; //transformations according to the world's origin
	glm::mat4x4 modelTransform; //transformations according to model's origin
	glm::mat4x4 normalTransform; //transformations on normals

	void setFaceNormals(); //build the faces' normals - in constructor
	void setBound(); //find max's and min's indices - in constructor 
public:
	void load_normal_per_vertex();
	MeshModel();//for PrimMeshModel()
	~MeshModel();
	bool window_open = 0;
	bool folow_the_mouse = 0;
	bool willDrawTexture = 0;
	bool willDrawBox=0;
	bool willDrawVertexNormal = 0; //normal per face per vertex
	bool willDrawVertexNormal2 = 0; //normal per vertex
	bool willDrawFaceNormal = 0;
	bool TEXTURE = 0;
	float Ambient =1.f;
	float Diffus=1.f;
	float Specular =1.f;
	void setModeltransform(glm::mat4x4 transform);
	MeshModel(const string& fileName);
	void LoadFile(const string& fileName);
	int getVertexPosNum();
	void transformModel(glm::mat4x4 transform);
	void transformWorld(glm::mat4x4 transform);
	vector<glm::vec4> getNormalVertex2();
	glm::vec4* Draw();
	glm::vec4* GetVertex();
	glm::vec4* getNormalVertex();
	glm::vec4* getNormalFace();
	glm::vec4* getFaceAvgs();
	glm::mat4x4& getModelTransform();
	glm::mat4x4& getWorldTransform();
	glm::mat4x4& getNormalTransform();


	//keep the vertices indices that bounds the object
	int xMax;
	int xMin;
	int yMax;
	int yMin;
	int zMax;
	int zMin;
};