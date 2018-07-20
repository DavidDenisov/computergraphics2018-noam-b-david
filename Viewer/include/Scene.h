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

	Scene() {};
	Scene(Renderer *renderer) : renderer(renderer) {};

	// Loads an obj file into the scene.
	void LoadOBJModel(string fileName);

	// Draws the current scene.
	void Draw();

	// Draws an example.
	void DrawDemo();

	//new function for home work on , please work
	void newfunc();
	
	//new function for home work on , please work
	void drawline(int i, int j, int i2, int j2);

	int ActiveModel;
	int ActiveLight;
	int ActiveCamera;
};