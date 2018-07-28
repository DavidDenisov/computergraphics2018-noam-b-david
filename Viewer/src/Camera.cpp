#include "Camera.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
# define PI 3.141592653589793238462643383279502884L /* pi */
using namespace std;
glm::mat4x4 Camera::creatTransform(glm::vec3 Scale_val
	, glm::vec3 Translate_val,glm::vec2 rotat_val)
{
	glm::mat4x4 a, b, c,d;
	a = ScaleTransform(Scale_val[0], Scale_val[1], Scale_val[2]);
	b = TranslateTransform(Translate_val[0], Translate_val[1], Translate_val[2]);
	c = rotationTransform(rotat_val[0], rotat_val[1]);
	d= a *b*c; // shouldn't be b * a * c? because translate you do last...

	return d;
}
glm::mat4x4 Camera::GetrotationTransform(double deg, int axis)
{
	return rotationTransform(deg,axis);
}
glm::mat4x4 Camera::GetScaleTransform(double x_scale, double y_scale, double z_scale)
{
	return ScaleTransform(x_scale, y_scale, z_scale);
}
glm::mat4x4 Camera::GetTranslateTransform(double x_scale, double y_scale, double z_scale)
{
	return TranslateTransform(x_scale, y_scale, z_scale);
}
glm::mat4x4 Camera::TranslateTransform(double x_scale, double y_scale, double z_scale)
{
	return glm::mat4x4(1, 0, 0, x_scale,
		0, 1, 0, y_scale,
		0, 0, 1, z_scale,
		0, 0, 0, 1);
}

glm::mat4x4 Camera::ScaleTransform(double x_scale, double y_scale, double z_scale)
{
	return glm::mat4x4(x_scale, 0, 0, 0,
		0, y_scale, 0, 0,
		0, 0, z_scale, 0,
		0, 0, 0, 1);
}
glm::mat4x4 Camera::rotationTransform(double deg, int axis)
{
	long double rad;
	rad = deg / 180.0L;
	rad = rad * PI;
	//convert from deg to radian
	if (axis == 0)
	{
		return glm::mat4x4(cos(rad), -sin(rad), 0, 0,
			sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);	
	}
	if (axis==1)
	{
		return glm::mat4x4(1,0,0,0,
			0,cos(rad), -sin(rad), 0,
			0,sin(rad), cos(rad),0,
			0, 0, 0, 1);
	}
	if (axis == 2)
	{
		return glm::mat4x4(cos(rad), 0, sin(rad), 0,
			0, 1, 0, 0,
			-sin(rad), 0, cos(rad), 0,
			0,0,0, 1
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




