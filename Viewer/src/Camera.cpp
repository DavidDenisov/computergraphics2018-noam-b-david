#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>


glm::mat4x4 Camera::creatTransform(glm::vec3 Scale_val
	, glm::vec3 Translate_val,glm::vec2 rotat_val)
{
	glm::mat4x4 a, b, c,d;
	a = ScaleTransform(Scale_val[0], Scale_val[1], Scale_val[2]);
	b = TranslateTransform(Translate_val[0], Translate_val[1], Translate_val[2]);
	c = rotationTransform(rotat_val[0], rotat_val[1]);
	d= a * b*c;
	for (int i = 0; i < 4; i++)
	{	
		for (int j = 0; j < 4; j++)
			std::cout<<d[i][j] << " ";
		std::cout << "\n";
	}
	return d;
}
glm::mat4x4 Camera::TranslateTransform(int x_scale, int y_scale, int z_scale)
{
	return glm::mat4x4(1, 0, 0, x_scale,
		0, 1, 0, y_scale,
		0, 0, 1, z_scale,
		0, 0, 0, 1);
}

glm::mat4x4 Camera::ScaleTransform(int x_scale, int y_scale, int z_scale)
{
	return glm::mat4x4(x_scale, 0, 0, 0,
		0, y_scale, 0, 0,
		0, 0, z_scale, 0,
		0, 0, 0, 1);
}
glm::mat4x4 Camera::rotationTransform(int deg, int axis)
{
	//if (deg < 2)// we belive that a angles smaller than 2 is considerd small
	//convert from deg to radian
	if (axis == 0)
	{
		return glm::mat4x4(cos(deg), -sin(deg), 0, 0,
			sin(deg), cos(deg), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);	
	}
	if (axis==1)
	{
		return glm::mat4x4(1,0,0,0,
			0,cos(deg), -sin(deg), 0,
			0,sin(deg), cos(deg),0,
			0, 0, 0, 1);
	}
	if (axis == 2)
	{
		return glm::mat4x4(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0,cos(deg), -sin(deg),
			0,0,sin(deg), cos(deg)
			);
	}
}
Camera::Camera()
{
	projection = cTransform = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}
Camera::Camera(Camera* c)
{
	projection = c->get_projection();
	cTransform = c->get_Transform();
}
glm::mat4x4 Camera::get_projection() { return projection; }
glm::mat4x4 Camera::get_Transform() { return cTransform; }
Camera::~Camera()
{
}

void Camera::LookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up)
{
	glm::vec4 n = glm::normalize(at - eye);
	//no cross for vec4: convert to vec3 and then again to vec4
	glm::vec4 u = glm::normalize( glm::vec4(glm::cross(glm::vec3(up), glm::vec3(n)), 1.0f) );
	//just like above
	glm::vec4 v = glm::normalize(glm::vec4(glm::cross(glm::vec3(u), glm::vec3(n)), 1.0f)); 
	glm::vec4 w = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//each vector is a column
	glm::mat4x4 c = glm::mat4x4(u, v, n, w);
	glm::mat4x4 translate //translate(-eye)
	(
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		-eye
	);

	this->cTransform = c * translate;


}




