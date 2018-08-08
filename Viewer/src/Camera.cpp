#include "Camera.h"
#include <iostream>
#include "PrimMeshModel.h"
#include <glm/gtc/matrix_transform.hpp>

# define PI 3.141592653589793238462643383279502884L /* pi */

using namespace std;
void Camera::reset_projection()
{
	projection = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
}
void Camera::set_camBox(MeshModel* p)
{
	camBox = p;
}
void Camera::set_projection(glm::mat4x4 projection)
{
	this->projection = projection;
}
void Camera::update_transform(glm::mat4x4 transform)
{
	this->cTransform = transform * cTransform;
}
void Camera::camrotationTransform(double deg, int axis)
{
	cTransform = cTransform * rotationTransform(deg,axis);
}
void Camera::camScaleTransform(double x_scale, double y_scale, double z_scale)
{
	cTransform = cTransform * ScaleTransform(x_scale,y_scale,z_scale);
}
void Camera::camTranslateTransform(double x_scale, double y_scale, double z_scale)
{
	cTransform = cTransform * TranslateTransform(x_scale, y_scale, z_scale);
}
void Camera::Ortho(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar)
{ 
	glm::mat4x4 moveToOrigin = TranslateTransform(-(right + left) / 2, -(top + bottom) / 2, -(zNear + zFar) / 2);
	glm::mat4x4 scale = ScaleTransform(2 / (right - left), 2 / (top - bottom), 2 / (zFar-zNear));
	projection = scale * moveToOrigin;
}

void Camera::Perspective(const float fovy, const float aspect,
	const float zNear, const float zFar)
{
	float degToRad = 2 * PI / 360;
	float tg = tan(degToRad * fovy / 2);

	float top = zNear * tg;
	float right = aspect * top;
	
	float left = -right, bottom = -top;


	this->Frustum(left, right, bottom, top, zNear, zFar);
}

void Camera::Frustum(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar)
{

	glm::vec4 firstCol(2 * zNear / (right - left), 0.0f, 0.0f, 0.0f);
	glm::vec4 secondCol(0.0f, 2 * zNear / (top - bottom), 0.0f, 0.0f);
	glm::vec4 thirdCol((right + left) / (right - left),
		(top + bottom) / (top - bottom), -(zFar + zNear) / (zFar - zNear), -1.0f);
	glm::vec4 fourthCol(0.0f, 0.0f, -2 * zFar*zNear / (zFar - zNear), 0.0f);

	glm::mat4x4 frutsum(firstCol, secondCol, thirdCol, thirdCol);
	projection = frutsum;
}

void Camera::Transform(glm::mat4x4 t)
{
	cTransform = t*cTransform;
}
glm::mat4x4 Camera::creatTransform(glm::vec3 Scale_val
	, glm::vec3 Translate_val,glm::vec2 rotat_val)
{

	glm::mat4x4 scale, translate, rotate,d;
	scale = ScaleTransform(Scale_val[0], Scale_val[1], Scale_val[2]);
	translate = TranslateTransform(Translate_val[0], Translate_val[1], Translate_val[2]);
	rotate = rotationTransform(rotat_val[0], rotat_val[1]);
	d= translate * scale*rotate; // shouldn't be b * a * c? because translate you do last...
	return d;
}
glm::mat4x4 Camera::GetrotationTransform(long double deg, int axis)
{
	return rotationTransform(deg,axis);
}
glm::mat4x4 Camera::GetScaleTransform(long double x_scale, long double y_scale, long double z_scale)
{
	return ScaleTransform(x_scale, y_scale, z_scale);
}
glm::mat4x4 Camera::GetTranslateTransform(long double x_scale, long double y_scale, long double z_scale)
{
	return TranslateTransform(x_scale, y_scale, z_scale);
}

MeshModel* Camera::getCamBox()
{
	return this->camBox;
}

glm::mat4x4 Camera::TranslateTransform(long double x_scale, long double y_scale, long double z_scale)
{
	return glm::mat4x4(1, 0, 0,0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x_scale, y_scale, z_scale, 1);
}
glm::mat4x4 Camera::ScaleTransform(long double x_scale, long double y_scale, long double z_scale)
{
	return glm::mat4x4(x_scale, 0, 0, 0,
		0, y_scale, 0, 0,
		0, 0, z_scale, 0,
		0, 0, 0, 1);
}
glm::mat4x4 Camera::rotationTransform(long double rad, int axis)
{
	if (axis == 2)
	{
		return glm::mat4x4(cos(rad), sin(rad), 0, 0,
			-sin(rad), cos(rad), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);	
	}
	if (axis==0)
	{

		 return glm::mat4x4(1, 0, 0, 0,
			0, cos(rad), sin(rad), 0,
			0, -sin(rad), cos(rad), 0,
			0, 0, 0, 1);
	}
	if (axis == 1)
	{
		return glm::mat4x4(cos(rad), 0, -sin(rad), 0,
			0, 1, 0, 0,
			sin(rad), 0, cos(rad), 0,
			0,0,0, 1);
	}
}
Camera::Camera()
{
	num=0;
	projection = cTransform = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	pos = glm::vec4(0, 0, 1, 1);
	up = glm::vec4(0, 1, 0, 1);
	camBox = new PrimMeshModel();
}
Camera::Camera(Camera* c)
{
	num=c->num+1;
	projection = c->get_projection();
	cTransform = c->get_Transform();
	pos = glm::vec4(0, 0, 1, 1);
	up = glm::vec4(0, 1, 0, 1);
	camBox = new PrimMeshModel();
}
glm::mat4x4 Camera::get_projection() { return projection; }
glm::mat4x4 Camera::get_Transform() { return cTransform; }
Camera::~Camera()
{
}

void Camera::LookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up)
{
	glm::vec4 n = glm::normalize(eye - at);
	//no cross for vec4: convert to vec3 and then again to vec4
	glm::vec4 u = glm::normalize( glm::vec4(glm::cross(glm::vec3(up), glm::vec3(n)), 0.0f) );
	//just like above
	glm::vec4 v = glm::normalize(glm::vec4(glm::cross(glm::vec3(n), glm::vec3(u)), 0.0f)); 
	glm::vec4 w = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	
	//each vector is a column
	glm::mat4x4 c = glm::mat4x4(u, v, n, w);
	glm::mat4x4 translate //translate(eye) -- for camera is translate(eye), for model is c^-1 (-eye)
	(
		glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
		glm::vec4(-eye.x, -eye.y, -eye.z, 1.0f)
	);
	this->cTransform = c * translate;
}




