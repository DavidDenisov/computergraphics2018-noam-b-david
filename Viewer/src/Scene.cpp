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
	renderer->SetDemoBuffer();
	renderer->SwapBuffers();
}

void Scene::drawline(int i, int j, int i2, int j2)
{
	renderer->drawline(i, j, i2, j2);
	renderer->SwapBuffers();
}
