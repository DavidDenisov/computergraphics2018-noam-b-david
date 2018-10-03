#include "Light.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>


Light::Light()
{
	Specularity_exponent = 1;
	ambient= 
	 difus =
	specalar =glm::vec3(0.f, 0.f, 0.f);
	direction = glm::vec3(0.f, 0.f, 1.f);
	Position = glm::vec4(0.f, 0.f, 0.f, 1.f);
	strengte_ambient =
	strengte_difus =
	strengte_specalar = 
	1.f;
	type = true;
	transromMatrix = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}

glm::vec3 Light::getPosition()
{
	return transromMatrix * Position;
}
void Light::transformPosition(glm::mat4 transform)
{
	transromMatrix = transform * transromMatrix;
}
void Light::resetPosition()
{
	transromMatrix = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}
Light::~Light()
{
}
