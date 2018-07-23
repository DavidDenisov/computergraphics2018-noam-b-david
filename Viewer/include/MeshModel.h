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
protected :
	//MeshModel() {} - primitive MeshModel - part of task2
	glm::vec4 *vertexPositions; //changed vec3 to vec4
	// Add more attributes.
	
	//is it neccesery?
	//vector<glm::vec3> modelVertices; 

	//***added above
	glm::mat4x4 worldTransform;
	glm::mat4x4 normalTransform;

public:
	MeshModel(const string& fileName);
	MeshModel(); //primitive MeshModel - task2
	~MeshModel();
	void LoadFile(const string& fileName);
	const vector<glm::vec3>* Draw();
};
