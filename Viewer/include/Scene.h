#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Renderer.h"
#include "Model.h"
#include "Light.h"
#include "Camera.h"

using namespace std;

class Scene {
private:
	vector<Model*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Renderer *renderer;
public:

	Scene();
	Scene(Renderer *renderer);
	void transformProjection(int a, int b, int c, int d, int e, int f);
	void transformModel(glm::mat4x4 transform);
	// Loads an obj file into the scene.
	void LoadOBJModel(string fileName);

	void Scene::transformCam(glm::mat4x4 transform);

	void RemoveModel(int num);
	// Draws the current scene.
	void Draw();

	void DrawScene();
	// Draws an example.
	void DrawDemo();

	void drawf();

	void setcur_cam(int i);

	void setcur_model(int i);

	void load_cam(Camera* cam);

	void remove_cam(int i);

	glm::vec4* GetVertex(int mod);

	glm::vec4 GetVertexAvg(int mod);

	const vector<Model*> getModels();
	const vector<Light*> getLights();
	const vector<Camera*> getCameras();


	int ActiveModel;
	int ActiveLight;
	int ActiveCamera;
};