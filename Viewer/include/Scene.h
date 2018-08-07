#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Renderer.h"
#include "Model.h"
#include "MeshModel.h"
#include "Light.h"
#include "Camera.h"

using namespace std;

class Scene {
private:
	int num;
	vector<MeshModel*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	vector<glm::vec4> colors_model;
	vector<glm::vec4> colors_camera;
	Renderer *renderer;
public:
	void set_renderer(Renderer *renderer);
	void draw(string s);
	~Scene(){}
	Scene();
	Scene(Renderer *renderer);
	void transformProjection(int a, int b, int c, int d, int e, int f);
	void setColor(int i,glm::vec4 color,int type);
	glm::vec4  getColor(int i, int type);
	void transformModel(glm::mat4x4 transform);
	void transformWorld(glm::mat4x4 transform);
	// Loads an obj file into the scene.
	void LoadOBJModel(string fileName);
	void Scene::load_cam();
	void LoadPrim();

	void transformProjectionCam(glm::mat4x4 transform,int place);

	void transformCam(glm::mat4x4 transform);

	void RemoveModel(int num);
	// Draws the current scene.
	void Draw();

	void DrawScene(float w, float h);
	// Draws an example.
	void DrawDemo();

	void drawf();

	void setcur_cam(int i);

	void setcur_model(int i);

	void load_cam(Camera* cam);

	void remove_cam(int i);

	glm::vec4* GetVertex(int mod);

	glm::vec4 GetVertexAvg(int mod);

	const vector<MeshModel*> getModels();
	const vector<Light*> getLights();
	const vector<Camera*> getCameras();


	bool willCamerasRender = 0;

	int ActiveModel;
	int ActiveLight;
	int ActiveCamera;
};