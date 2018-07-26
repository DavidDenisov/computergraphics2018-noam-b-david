
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

void Scene::load_cam(Camera* cam)
{
	Camera* c = new Camera(cam);
	cameras.push_back(c);
}
void Scene::remove_cam(int i)
{
	cameras.erase(cameras.begin()+i);
}
Scene::Scene() : ActiveModel(0), ActiveLight(0), ActiveCamera(0)
{
	Camera* c = new Camera();
	cameras.push_back(c);
};

Scene::Scene(Renderer *renderer) : renderer(renderer),
ActiveModel(0), ActiveLight(0), ActiveCamera(0)
{
	if(int(cameras.size())==0)
	{
		Camera* c = new Camera();
		cameras.push_back(c);
	}
};


void Scene::LoadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
}

void Scene::setcur_cam(int i)
{
	ActiveCamera = i;
}

void Scene::setcur_model(int i)
{
	ActiveModel = i;
}
void Scene::DrawScene()
{

	// 1. Send the renderer the current camera transform and the projection
	
	renderer->SetCameraTransform(cameras.at(ActiveCamera)->get_Transform());
	renderer->SetProjection(cameras.at(ActiveCamera)->get_projection());
	// 2. Tell all models to draw themselves

	//renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	//renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm

	renderer->DrawTriangles(models.at(ActiveModel)->Draw(),
		models.at(ActiveModel)->getVertexPosNum());
	renderer->SwapBuffers();
}

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

void Scene::DrawDemo()
{

	MeshModel* primitive = new PrimMeshModel(); //testing

	string fileName = "C:/Users/nir blagovsky/Documents/Noam/TEXTFILE.txt";
	//LoadFile of camera instead?
	MeshModel* testOBJ = new MeshModel(fileName); //a cube?

	const glm::vec4* verPos = testOBJ->Draw();
	//renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	//renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm
	
	//draw first triangle :O
	glm::vec2 a(0.0f, 0.0f), b(0.0f, 0.0f), c(0.0f, 0.0f);
	for (int face = 0; face < testOBJ->getVertexPosNum() - 2; face = face + 3)
	{
		a.x = verPos[face].x*16+100;
		a.y = verPos[face].y * 16 + 100;

		b.x = verPos[face + 1].x * 16 + 100;
		b.y = verPos[face + 1].y * 16 + 100;

		c.x = verPos[face + 2].x * 16 + 100;
		c.y = verPos[face + 2].y * 16 + 100;

		renderer->drawLine(a, b);
		renderer->drawLine(b, c);
		renderer->drawLine(c, a);
	}

	
	renderer->SwapBuffers();
}
void Scene::drawf()
{
	glm::vec4 *c =new glm::vec4;
	c[0] = (glm::vec4(100, 100, 0,0));
	c[1] = (glm::vec4(200, 100, 0,0));
	c[2] = (glm::vec4(100, 200, 0,0));
	renderer->DrawTriangles(c,3); //Bresenham algorithm
	renderer->SwapBuffers();
}
