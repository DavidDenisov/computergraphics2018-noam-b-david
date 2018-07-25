#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
{
	
}


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




