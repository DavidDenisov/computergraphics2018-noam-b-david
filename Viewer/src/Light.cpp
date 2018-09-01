#include "Light.h"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>


Light::Light()
{
	ambient= 
	 difus =
	specalar =glm::vec3(0.f, 0.f, 0.f);
	difus_direction= glm::vec3(0.f, 0.f, 1.f);

	strengte_ambient =
	strengte_difus =
	strengte_specalar = 
	1.f;
}


Light::~Light()
{
}
