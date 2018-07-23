#include "Scene.h"
#include "MeshModel.h"
#include <string>
#define GLM_SWIZZLE

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
	//LoadFile of camera instead
	//fileName = "C:/Users/Nir blagovsky/Documents/Noam/גרפיקה ממוחשבת/GitHub/computergraphics2018-noam-b-david/Data/camera.obj";
	MeshModel* testOBJ = new MeshModel(); //a cube?


	//renderer->printLineNaive(); //Naive draw line
	//renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm
	
	const glm::vec3* verPos = testOBJ->Draw();
	//draw first triangle :O
	glm::vec2 a(0.0f, 0.0f), b(0.0f, 0.0f), c(0.0f, 0.0f);

	for (int face = 0; face < 33; face = face + 3)
	{
		a.x = verPos[face].x;
		a.y = verPos[face].y;

		b.x = verPos[face + 1].x;
		b.y = verPos[face + 1].y;

		c.x = verPos[face + 2].x;
		c.y = verPos[face + 2].y;

		renderer->drawLine(a * 100.0f +100.f , b * 100.0f + 100.f);
		renderer->drawLine(b * 100.0f + 100.f, c * 100.0f + 100.f);
		renderer->drawLine(c * 100.0f + 100.f, a * 100.0f + 100.f);
	}

	
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
