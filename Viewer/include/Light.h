#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
/*
 * Light class. Holds light source information and data.
 */
class Light
{
public:
	Light();
	~Light();
	glm::vec3 ambient , difus , specalar, difus_direction;
	float strengte_specalar, strengte_difus, strengte_ambient;
	int type=0;
};
