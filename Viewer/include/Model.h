#pragma once
#include <vector>
#include <glm/glm.hpp>

using namespace std;
/*
 * Model class. An abstract class for all types of geometric data.
 */
class Model
{
protected:
	std::string nameModel;
public:
	virtual ~Model() {};
	bool folow_the_mouse = 0;
	virtual int getVertexPosNum() = 0;
	virtual const glm::vec4* Draw() = 0; //changed it from vector<glm::vec4>* to glm::vec4*
	virtual void transformModel(glm::mat4x4 transform) = 0;
	virtual glm::vec4* GetVertex()=0;
	std::string getNameModel()
	{
		return this->nameModel;
	}
};
