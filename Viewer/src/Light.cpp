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
glm::vec3 CalcDirLight(Light light, glm::vec3 normal, glm::vec3 am, glm::vec3 viewDir,float textur
,glm::vec3 am_c, glm::vec3 dif_c, glm::vec3 spec_c)
{
	glm::vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = fmax(dot(normal, lightDir), 0.0);
	// specular shading
	glm::vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(fmax(dot(viewDir, reflectDir), 0.0), light.Specularity_exponent);
	// combine results
	glm::vec3 ambient = glm::vec3(0, 0, 0);
	glm::vec3 diffuse = glm::vec3(0, 0, 0);
	glm::vec3 specular = glm::vec3(0, 0, 0);
	if (textur) 
	{
		//glm::vec3 ambient = am * vec3(texture(material.diffuse, TexCoords));
		//glm::vec3 diffuse = light.difus*light.strengte_difus * diff * vec3(texture(material.diffuse, TexCoords));
		//glm::vec3 specular = light.specalar*light.strengte_ambient * spec * vec3(texture(material.specular, TexCoords));
	}
	else
	{
		ambient = am * am_c;
		diffuse = light.difus*light.strengte_difus*diff *dif_c;
		specular = light.specalar*light.strengte_ambient * spec*spec_c;
	}
	return (ambient + diffuse + specular);
}