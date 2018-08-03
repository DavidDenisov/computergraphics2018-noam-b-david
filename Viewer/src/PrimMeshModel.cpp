#include "PrimMeshModel.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#define FACE_ELEMENTS 3



PrimMeshModel::PrimMeshModel() //primitive MeshModel - task2
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
	//build vertices
	{ 
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
	}
	this->vertexPositions = vPi;
	this->vertexPosNum = 36;


	glm::vec4
		upN(0.0f, 1.0f, 0.0f, 0.0f),
		downN(0.0f, -1.0f, 0.0f, 0.0f),
		rightN(1.0f, 0.0f, 0.0f, 0.0f),
		leftN(-1.0f, 0.0f, 0.0f, 0.0f),
		forwardN(0.0f, 0.0f, 1.0f, 0.0f),
		backwardN(0.0f, 0.0f, -1.0f, 0.0f);

	glm::vec4* norm = new glm::vec4[36];
	//build normals
	{ 
		// *a b c d*
		norm[0] = downN, norm[1] = downN, norm[2] = downN;
		norm[3] = downN, norm[4] = downN, norm[5] = downN;
		// *aUp bUp cUp dUp* 
		norm[6] = upN, norm[7] = upN, norm[8] = upN;
		norm[9] = upN, norm[10] = upN, norm[11] = upN,
			// *c d dUp cUp*
			norm[12] = forwardN, norm[13] = forwardN, norm[14] = forwardN;
		norm[15] = forwardN, norm[16] = forwardN, norm[17] = forwardN;
		// *a d dUp aUp*
		norm[18] = leftN, norm[19] = leftN, norm[20] = leftN;
		norm[21] = leftN, norm[22] = leftN, norm[23] = leftN;
		// *a b bUp aUp*
		norm[24] = backwardN, norm[25] = backwardN, norm[26] = backwardN;
		norm[27] = backwardN, norm[28] = backwardN, norm[29] = backwardN;
		// *b c cUp dUp*
		norm[30] = rightN, norm[31] = rightN, norm[32] = rightN;
		norm[33] = rightN, norm[34] = rightN, norm[35] = rightN;
	}

	this->normalPositions = norm;
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


	setBound();
	willDrawBox = 0;
}