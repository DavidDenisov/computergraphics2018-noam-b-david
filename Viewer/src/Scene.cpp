
#include "Scene.h"
#include "MeshModel.h"
#include <string>
#include "PrimMeshModel.h"
#define GLM_SWIZZLE

//for checking filename
#include <iostream>
#include <fstream>
#include <sstream>
/*
int ActiveModel=0;
int ActiveLight=0;
int ActiveCamera=0;
*/

using namespace std;
/*
void  Scene::transformcam(glm::mat4x4 transform, int place)
{
	this->cameras[place]->transform_transform(transform);
}*/

void  Scene::transformProjectionCam(glm::mat4x4 transform, int place)
{
	this->cameras[place]->set_projection(transform);
}

void Scene::setColor(int i,glm::vec4 color,int type)
{
	if (type == 0)
		colors_model[i] = color;
	else
		colors_camera[i] = color;
}
void Scene::RemoveModel(int num)
{

	delete[] models[num];
	//free(models[num]);
	cout << 3;
	models.erase(models.begin()+num);
	if (ActiveModel == num)
		ActiveModel = 0;
	colors_model.erase(colors_model.begin() + num);
}
glm::vec4 Scene::GetVertexAvg(int mod)
{
	long long s = models[mod]->getVertexPosNum();
	const glm::vec4 * a = models[mod]->Draw();
	glm::vec4 avg= glm::vec4(0,0,0,0);
	for (long long i = 0; i < s; i++)
		avg = avg + a[i];

	for (int i = 0; i < 4; i++)
		avg[i] = float(avg[i] /float(s));
	delete a;
	return avg;
}

glm::vec4* Scene::GetVertex(int mod)
{
	return models[mod]->GetVertex();
}
void Scene::setModeltransform(glm::mat4x4 transform)
{
	models[ActiveModel]->setModeltransform(transform);
}
void Scene::transformModel(glm::mat4x4 transform)
{
	models[ActiveModel]->transformModel(transform);
}
void Scene::transformWorld(glm::mat4x4 transform)
{
	models[this->ActiveModel]->transformWorld(transform);
}
void Scene::transformProjection(int a, int b, int c, int d, int e, int f)
{
	//cameras[ActiveCamera]->Frustum(a, b, c, d, e, f);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			cout << cameras[ActiveCamera]->get_projection()[i][j] << " ";
		cout << endl;
	}
}
void Scene::transformCam(glm::mat4x4 transform)
{
	cameras[ActiveCamera]->Transform(transform);
}
void Scene::load_cam(Camera* cam)
{
	Camera* c = new Camera(cam);
	colors_camera.push_back(glm::vec4(0, 0, 0, 1));
	c->num = num;
	num++;
	cameras.push_back(c);
}
void Scene::load_cam()
{
	Camera* c = new Camera();
	colors_camera.push_back(glm::vec4(0, 0, 0, 1));
	c->num = num;
	num++;
	cameras.push_back(c);
}
void Scene::remove_cam(int i)
{
	cameras.erase(cameras.begin()+i);
	if (ActiveCamera == i)
		ActiveCamera = 0;
	colors_camera.erase(colors_camera.begin() + i);
}
Scene::Scene() : ActiveModel(0), ActiveLight(0), ActiveCamera(0)
{
	num = 0;
	Camera* c = new Camera();
	
	colors_camera.push_back(glm::vec4(0, 0, 0, 1));
	//colors_model.push_back(glm::vec4(0, 0, 0, 1));

	MeshModel *primModel = new PrimMeshModel();
	c->set_camBox(primModel);
	cameras.push_back(c);
};

Scene::Scene(Renderer *renderer) : renderer(renderer),
ActiveModel(0), ActiveLight(0), ActiveCamera(0)
{
	num = 0;
	if(int(cameras.size())==0)
	{
		Camera* c = new Camera();
		cameras.push_back(c);
		colors_camera.push_back(glm::vec4(0, 0, 0, 1));
		//colors_model.push_back(glm::vec4(0, 0, 0, 1));
	}
};

glm::vec4 Scene::getColor(int i,int type)
{
	if (type == 0)
		return colors_model[i];
	else
		return colors_camera[i];
}
void Scene::LoadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
	colors_model.push_back(glm::vec4(0, 0, 0, 1));
}
void Scene::LoadPrim()
{
	MeshModel *primModel = new PrimMeshModel();
	models.push_back(primModel);
	colors_model.push_back(glm::vec4(0, 0, 0, 1));
}



void Scene::setcur_cam(int i)
{
	ActiveCamera = i;
}

void Scene::setcur_model(int i)
{
	ActiveModel = i;
}
void Scene::DrawScene(float w,float h)
{

	// 1. Send the renderer the current camera transform and the projection
	
	renderer->SetCameraTransform(cameras.at(ActiveCamera)->get_Transform()); // ** update by lookat!
	renderer->SetProjection(cameras.at(ActiveCamera)->get_projection());
	glm::mat4x4 windowresizing = cameras[0]->GetTranslateTransform(w/2,h/2,0) 
		*cameras[0]->GetScaleTransform(w / 2, h / 2, 1); //window coordinates

	// 2. Tell all models to draw themselves
	//renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	//renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm
	for (int i = 0; i < models.size(); i++)
	{
		//first set worldTransformation & nTransformation of the object in renderer
		renderer->SetObjectMatrices(models.at(i)->getWorldTransform(),
			models.at(i)->getNormalTransform());
		renderer->DrawTriangles(models.at(i)->Draw(), models.at(i)->getVertexPosNum()
			,colors_model[i], w, h, windowresizing, models.at(i));
	}

	//render cameras as well, if needed
	
	if (willCamerasRender == 1)
	{
		//exacly the same code like models' drawing just "cameras[i]->getCamBox()" for models[i] 
		//and not rendering the active camera
		for (int i = 0; i < cameras.size(); i++)
		{
			if (ActiveCamera != i)
			{
				renderer->SetObjectMatrices(cameras[i]->getCamBox()->getWorldTransform(),
					cameras[i]->getCamBox()->getNormalTransform());
				renderer->DrawTriangles(cameras[i]->getCamBox()->Draw(), cameras[i]->getCamBox()->getVertexPosNum()
					, colors_camera[i], w, h, windowresizing, cameras[i]->getCamBox());
			}
		}
	}

	renderer->SwapBuffers();
}
/*
void Scene::Draw()
{
	// 1. Send the renderer the current camera transform and the projection
	glm::mat4x4 default = glm::mat4x4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	renderer->SetCameraTransform(default);
	renderer->SetProjection(default);
	// 2. Tell all models to draw themselves
	//renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	//renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm
	renderer->DrawTriangles(models.at(ActiveModel)->Draw(),
	models.at(ActiveModel)->getVertexPosNum());
	renderer->SwapBuffers();
}
*/

void Scene::draw(string s)
{
	LoadOBJModel(s);
	int i = models.size();
	//renderer->DrawTriangles(models[i]->Draw(),
		//models[i]->getVertexPosNum(), colors[0], 1280.0, 720.0);
	this->RemoveModel(i);
	//renderer->SwapBuffers();
}
const vector<MeshModel*> Scene::getModels()
{
	return this->models;
}
const vector<Light*> Scene::getLights()
{
	return this->lights;
}
const vector<Camera*> Scene::getCameras()
{
	return this->cameras;
}
