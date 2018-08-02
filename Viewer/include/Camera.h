#pragma once
#include <glm/glm.hpp>

/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 */
class Camera
{
private:
	glm::mat4x4 cTransform;
	glm::mat4x4 projection;
	glm::mat4x4 rotationTransform(double deg, int axis);
	glm::mat4x4 ScaleTransform(double x_scale, double y_scale, double z_scale);
	glm::mat4x4 TranslateTransform(double x_scale, double y_scale, double z_scale);
public:
	Camera();
	Camera(Camera* c);
	~Camera();
	glm::mat4x4 GetrotationTransform(double deg, int axis);
	glm::mat4x4 GetScaleTransform(double x_scale, double y_scale, double z_scale);
	glm::mat4x4 GetTranslateTransform(double x_scale, double y_scale, double z_scale);
	glm::mat4x4 creatTransform(glm::vec3 Scale_val
		, glm::vec3 Translate_val,glm::vec2 rotat_val);
	glm::mat4x4 get_projection();
	glm::mat4x4 get_Transform();
	void Transform(glm::mat4x4 t);

	// Sets the camera transformations with relation to world coordinates
	void SetTransformation(const glm::mat4x4& transform);

	// Creates lookAt transformation.
	void LookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up );

	// Sets orthographic projection matrix.
	void Ortho( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );

	// Sets frustum view volume for clipping stage.
	void Frustum( const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar );

	// Sets perspective projection matrix.
	void Perspective( const float fovy, const float aspect,
		const float zNear, const float zFar);

};
