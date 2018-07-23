#include "Scene.h"
#include "MeshModel.h"
#include <string>

//for checking filename
#include <iostream>
#include <fstream>
#include <sstream>


using namespace std;
void Scene::LoadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
}

void Scene::Draw()
{
	// 1. Send the renderer the current camera transform and the projection
	// 2. Tell all models to draw themselves

	renderer->SwapBuffers();
}

void Scene::DrawDemo()
{
	MeshModel* primitive = new MeshModel(); //testing
	string fileName = "C:/Users/nir blagovsky/Documents/Noam/TEXTFILE.txt";
	fileName = "C:/Users/Nir blagovsky/Documents/Noam/גרפיקה ממוחשבת/GitHub/computergraphics2018-noam-b-david/Data/camera.obj";
	MeshModel* testOBJ = new MeshModel(fileName);

	//renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm
	renderer->SwapBuffers();
}
void Scene::drawf()
{
	vector <glm::vec3> *c =new vector <glm::vec3>;
	c->clear();
	c->push_back(glm::vec3(100, 100, 0));
	c->push_back(glm::vec3(200, 100, 0));
	c->push_back(glm::vec3(100, 200, 0));
	renderer->DrawTriangles(c); //Bresenham algorithm
	renderer->SwapBuffers();
}
