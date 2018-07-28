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
	glm::vec4 *vertexPositions; //changed vec3 to vec4
								// Add more attributes.
	int vertexPosNum; // 3 * #vertices
					  //is it neccesery?
					  //vector<glm::vec3> modelVertices; 
	
					  //***added above
	glm::mat4x4 worldTransform;
	glm::mat4x4 normalTransform;

public:
	MeshModel(const string& fileName);
	~MeshModel();
	void LoadFile(const string& fileName);
	int getVertexPosNum();
	void transformModel(glm::mat4x4 transform);
	const glm::vec4* Draw();
	glm::vec4* GetVertex();
};