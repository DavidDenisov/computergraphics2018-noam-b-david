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
	// Add more attributes.
	int vertexPosNum; // 3 * #vertices
	
	
	glm::mat4x4 worldTransform; //transformations according to the world's origin
	glm::mat4x4 modelTransform; //transformations according to model's origin
	glm::mat4x4 normalTransform; //transformations on normals

public:
	MeshModel(const string& fileName);
	~MeshModel();
	void LoadFile(const string& fileName);
	int getVertexPosNum();
	void transformModel(glm::mat4x4 transform);
	const glm::vec4* Draw();
	glm::vec4* GetVertex();
};