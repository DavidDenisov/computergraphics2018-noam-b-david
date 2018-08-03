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
	MeshModel() {} //for PrimMeshModel()
	glm::vec4 *vertexPositions;
	glm::vec4 *normalPositions;
	
	//keep the vertices indices that bounds the object
	int xMax;
	int xMin;
	int yMax;
	int yMin;
	int zMax;
	int zMin;
	
	
	// Add more attributes.
	int vertexPosNum; // 3 * #vertices
	
	
	glm::mat4x4 worldTransform; //transformations according to the world's origin
	glm::mat4x4 modelTransform; //transformations according to model's origin
	glm::mat4x4 normalTransform; //transformations on normals

	void setBound(); //find max's and min's indices

public:
	bool folow_the_mouse = 0;
	bool willDrawBox;
	MeshModel(const string& fileName);
	void LoadFile(const string& fileName);
	int getVertexPosNum();
	void transformModel(glm::mat4x4 transform);
	void transformWorld(glm::mat4x4 transform);
	const glm::vec4* Draw();
	glm::vec4* GetVertex();
	glm::mat4x4& getWorldTransform();
	glm::mat4x4& getNormalTransform();
};