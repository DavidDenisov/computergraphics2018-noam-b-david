#include "Scene.h"
#include "MeshModel.h"
#include <string>

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

	renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm
	renderer->SwapBuffers();
}

