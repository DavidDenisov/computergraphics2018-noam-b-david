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

	Scene(){};
	Scene(Renderer *renderer) : renderer(renderer){};

	// Loads an obj file into the scene.
	void LoadOBJModel(string fileName);

	// Draws the current scene.
	void Draw();

	void DrawScene();
	// Draws an example.
	void DrawDemo();

	void drawf();

	void setcur_cam(int i);

	void setcur_model(int i);

	int ActiveModel=0;
	int ActiveLight=0;
	int ActiveCamera=0;
};