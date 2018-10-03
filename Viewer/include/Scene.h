<<<<<<< HEAD
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
	vector<glm::vec3> AMcolors_model;
	vector<glm::vec3> AMcolors_camera;
	vector<glm::vec3> Difcolors_model;
	vector<glm::vec3> Difcolors_camera;
	vector<glm::vec3> SPECTcolors_model;
	vector<glm::vec3> SPECTcolors_camera;
	Renderer *renderer;

public:
	float get_sampel_size();
	void set_sampel_size(float f);
	float get_zFar();
	void set_zFar(float f);
	bool get_fog();
	void set_SuperSampling(bool x);
	bool get_SuperSampling();
	void set_fog(bool x);
	bool get_auto_color(); 
	void set_auto_color(bool x); 
	void setModeltransform(glm::mat4x4 transform);
	void set_renderer(Renderer *renderer);
	void add_Light() { lights.push_back(new Light()); }
	void set_Light(Light * l, int pos) { lights[pos] = l; }
	Light* get_Light(int pos) { return lights[pos];}
	void remove_Light(int pos) { lights.erase(lights.begin() + pos); }
	void draw(string s);
	~Scene(){}
	Scene();
	Scene(Renderer *renderer);
	void transformProjection(int a, int b, int c, int d, int e, int f);
	void setColor(int i,glm::vec3 color,int type,int type_ligth);
	glm::vec3  getColor(int i, int type, int type_ligth);
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
	int type=0;
	glm::vec3 ambient= glm::vec3(0,0,0);
	float strengte_ambient = 0;
=======
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
	vector<glm::vec3> AMcolors_model;
	vector<glm::vec3> AMcolors_camera;
	vector<glm::vec3> Difcolors_model;
	vector<glm::vec3> Difcolors_camera;
	vector<glm::vec3> SPECTcolors_model;
	vector<glm::vec3> SPECTcolors_camera;
	Renderer *renderer;
public:

	float get_sampel_size();
	void set_sampel_size(float f);
	float get_zFar();
	void set_zFar(float f);
	bool getSuperSampling();
	void set_superSampling(bool x);
	bool get_fog();
	void set_fog(bool x);

	bool get_auto_color(); 
	void set_auto_color(bool x);
	bool get_texture();
	void set_texture(bool x);
	void setModeltransform(glm::mat4x4 transform);
	void set_renderer(Renderer *renderer);
	void add_Light() { lights.push_back(new Light()); }
	void set_Light(Light * l, int pos) { lights[pos] = l; }
	Light* get_Light(int pos) { return lights[pos];}
	void remove_Light(int pos) { lights.erase(lights.begin() + pos); }
	void draw(string s);
	~Scene(){}
	Scene();
	Scene(Renderer *renderer);
	void transformProjection(int a, int b, int c, int d, int e, int f);
	void setColor(int i,glm::vec3 color,int type,int type_ligth);
	glm::vec3  getColor(int i, int type, int type_ligth);
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
	int type=0;
	glm::vec3 ambient= glm::vec3(0,0,0);
	float strengte_ambient = 0;
>>>>>>> 017297312cfcc5bedaa0b74f4d3cbea5e70acb96
};