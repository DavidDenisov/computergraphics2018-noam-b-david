#pragma once
#include <glm/glm.hpp>
#include "MeshModel.h"
/*
 * Camera class. This class takes care of all the camera transformations and manipulations.
 */
class Camera 
{
private:
	glm::mat4x4 cTransform;
	glm::mat4x4 projection;
	MeshModel* camBox;
	
	//last minute changes - try to transform camera like model
	glm::mat4x4 camModelTransform;
	glm::mat4x4 camWorldTransform;


	glm::mat4x4 rotationTransform(long double deg, int axis);
	glm::mat4x4 ScaleTransform(long double x_scale, long double y_scale, long double z_scale);
	glm::mat4x4 TranslateTransform(long double x_scale, long double y_scale, long double z_scale);
public:
	void set_camBox(MeshModel* p);
	void set_projection(glm::mat4x4 projection);
	glm::vec4 up = glm::vec4(0, 1, 0, 1);
	glm::vec4 pos = glm::vec4(0, 0, 1, 1);
	//fake "at". will keep track on where does the camera look at
	glm::vec4 at = glm::vec4(0, 0, 0, 1); 
	int num;


	Camera();
	Camera(Camera* c);
	~Camera();
	void reset_projection();
	glm::mat4x4 GetrotationTransform(long double deg, int axis);
	glm::mat4x4 GetScaleTransform(long double x_scale, long double y_scale, long double z_scale);
	glm::mat4x4 GetTranslateTransform(long double x_scale, long double y_scale, long double z_scale);
	void update_transform(glm::mat4x4 transform);
	void camrotationTransform(double deg, int axis);
	void camScaleTransform(double x_scale, double y_scale, double z_scale);
	void camTranslateTransform(double x_scale, double y_scale, double z_scale);
	glm::mat4x4 creatTransform(glm::vec3 Scale_val
		, glm::vec3 Translate_val,glm::vec2 rotat_val);
	glm::mat4x4 get_projection();
	glm::mat4x4 get_Transform();

	glm::mat4x4 get_camModelTransform();
	glm::mat4x4 get_camWorldTransform();
	void update_camModelTransform(glm::mat4x4 transform);
	void update_camWorldTransform(glm::mat4x4 transform);
	void set_camModelTransform(glm::mat4x4 transform);
	void set_camWorldTransform(glm::mat4x4 transform);


	MeshModel* getCamBox();

	void Transform(glm::mat4x4 t);

	// Sets the camera transformations with relation to world coordinates
	void SetTransformation(const glm::mat4x4& transform);

	// Creates lookAt transformation.
	glm::mat4x4 LookAt(const glm::vec4& eye, const glm::vec4& at, const glm::vec4& up );


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
