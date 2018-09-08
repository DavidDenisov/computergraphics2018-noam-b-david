#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "Camera.h"
/*
 * Light class. Holds light source information and data.
 */
class Light
{
private:
	glm::vec4 Position;
	glm::mat4 transromMatrix;
public:
	glm::vec3 getPosition();
	void transformPosition(glm::mat4 transform);
	void resetPosition();
	Light();
	~Light();
	glm::vec3 ambient , difus , specalar, direction;
	float strengte_specalar, strengte_difus, strengte_ambient;
	int Specularity_exponent;
	bool type=true;
};
