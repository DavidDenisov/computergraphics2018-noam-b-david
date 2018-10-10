
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

float Scene::get_sampel_size() { return sampel_size; }
void Scene::set_sampel_size(float f) { sampel_size = f; }
float Scene::get_zFar() { return zFar; }
void Scene::set_zFar(float f) { zFar = f; }
bool Scene::getSuperSampling() { return superSampling; }
void Scene::set_superSampling(bool x) { superSampling = x; }
bool Scene::get_fog() { return fog; }
void Scene::set_fog(bool x) { fog = x; }



/*
void  Scene::transformcam(glm::mat4x4 transform, int place)
{
	this->cameras[place]->transform_transform(transform);
}*/
template<class T>
const T& maxs(const T& a, const T& b)
{
	return (a < b) ? b : a;
}
float norm2(const glm::vec3 v)
{
	//return pow((abs(v.x) + abs(v.y) + abs(v.z)),0.5);
	float ans = maxs(v.x, 0.f) + maxs(v.y, 0.f) + maxs(v.z, 0.f);
	ans = pow(ans, 0.5);
	if (ans > 1.f)
		return 1.f;
	return ans;
}
bool Scene::get_auto_color() { return auto_color; }
void Scene::set_auto_color(bool x) { auto_color = x; }
bool Scene::get_texture()
{	
if (models.size()>0)
	return models[ActiveModel]->TEXTURE;
return FALSE;
}
void Scene::set_texture(bool x) 
{ 
if (models.size() > 0)
	models[ActiveModel]->TEXTURE = x; 
}
void  Scene::transformProjectionCam(glm::mat4x4 transform, int place)
{
	this->cameras[place]->set_projection(transform);
}

void Scene::setColor(int i,glm::vec3 color,int type, int type_ligth)
{
	if (type == 0)
	{
		switch (type_ligth)
		{
		case 0:
		{
			AMcolors_model[i] = color;
			break;
		}
		case 1:
		{
			Difcolors_model[i] = color;
			break;
		}
		case 2:
		{
			SPECTcolors_model[i] = color;
			break;
		}
		}
	}
	else
	{
		switch (type_ligth)
		{
		case 0:
		{
			AMcolors_camera[i] = color;
			break;
		}
		case 1:
		{
			Difcolors_camera[i] = color;
			break;
		}
		case 2:
		{
			SPECTcolors_camera[i] = color;
			break;
		}
		}
	}
}
void Scene::RemoveModel(int num)
{

	delete[] models[num];
	//free(models[num]);
	cout << 3;
	models.erase(models.begin()+num);
	if (ActiveModel == num)
		ActiveModel = 0;
	AMcolors_model.erase(AMcolors_model.begin() + num);
	Difcolors_model.erase(Difcolors_model.begin() + num);
	SPECTcolors_model.erase(SPECTcolors_model.begin() + num);
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
	AMcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	Difcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	SPECTcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	c->num = num;
	num++;
	cameras.push_back(c);
}
void Scene::load_cam()
{
	Camera* c = new Camera();
	AMcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	Difcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	SPECTcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	c->num = num;
	num++;
	cameras.push_back(c);
}
void Scene::remove_cam(int i)
{
	cameras.erase(cameras.begin()+i);
	if (ActiveCamera == i)
		ActiveCamera = 0;
	AMcolors_camera.erase(AMcolors_camera.begin() + i);
	Difcolors_camera.erase(Difcolors_camera.begin() + i);
	SPECTcolors_camera.erase(SPECTcolors_camera.begin() + i);
}
Scene::Scene() : ActiveModel(0), ActiveLight(0), ActiveCamera(0)
{
	num = 0;
	Camera* c = new Camera();
	
	AMcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	Difcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	SPECTcolors_camera.push_back(glm::vec4(0, 0, 0, 1));
	//colors_model.push_back(glm::vec4(0, 0, 0, 1));

	MeshModel *primModel = new PrimMeshModel();
	c->set_camBox(primModel);
	cameras.push_back(c);
	lights.push_back(new Light());
};



glm::vec3 Scene::getColor(int i,int type, int type_ligth)
{
	if (type == 0)
	{
		switch (type_ligth)
		{
		case 0:
			return AMcolors_model[i] ;

		case 1:
			return Difcolors_model[i] ;

		case 2:
			return SPECTcolors_model[i] ;
		}
	}
	else
	{
		switch (type_ligth)
		{
		case 0:
			return AMcolors_camera[i];

		case 1:
			return Difcolors_camera[i];
			
		case 2:
			return SPECTcolors_camera[i];
		
		}
	}
}
void Scene::LoadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
	AMcolors_model.push_back(glm::vec4(0, 0, 0, 1));
	Difcolors_model.push_back(glm::vec4(0, 0, 0, 1));
	SPECTcolors_model.push_back(glm::vec4(0, 0, 0, 1));
}
void Scene::LoadPrim()
{
	MeshModel *primModel = new PrimMeshModel();
	models.push_back(primModel);
	AMcolors_model.push_back(glm::vec4(0, 0, 0, 1));
	Difcolors_model.push_back(glm::vec4(0, 0, 0, 1));
	SPECTcolors_model.push_back(glm::vec4(0, 0, 0, 1));
}



void Scene::setcur_cam(int i)
{
	ActiveCamera = i;
}

void Scene::setcur_model(int i)
{
	ActiveModel = i;
}

//void Scene::DrawScene(float w, float h)
/*

void Scene::DrawScene(float w,float h)
{
	// 1. Send the renderer the current camera transform and the projection
	if (w < 3 || h < 3)
		return;
	renderer->SetCameraTransform(cameras.at(ActiveCamera)->get_Transform()); // ** update by lookat!
	renderer->SetProjection(cameras.at(ActiveCamera)->get_projection());
	glm::mat4x4 windowresizing = cameras[0]->GetTranslateTransform(w/2.0,h/2,0.0) 
		*cameras[0]->GetScaleTransform(w / 2.0, h / 2.0, 1.0); //window coordinates

	renderer->zBuffer = new float*[w];
	for (int i = 0; i < w; i++)
	{
		renderer->zBuffer[i] = new float[h];
		for (int j = 0; j < h; j++)
			renderer->zBuffer[i][j] = -INFINITY;
	}

	// 2. Tell all models to draw themselves
	//renderer->SetDemoBuffer();
	//renderer->printLineNaive(); //Naive draw line
	//renderer->drawLine(glm::vec2(0.0, 0.0), glm::vec2(700.0, 700.0)); //Bresenham algorithm

		
	// Specular = lights[ActiveLight]->difus*lights[ActiveLight]->strengte_difus;
	vector<glm::vec3> diffus;
	vector<glm::vec3> position;
	vector<glm::vec3> directions;
	vector<glm::vec3> sp_ligth_colors;
	vector<bool> ligth_type;
	vector<int> sp_exp;
	for (int i = 0; i < lights.size(); i++)
	{
		diffus.push_back(lights[i]->difus*lights[i]->strengte_difus);
		ligth_type.push_back(lights[i]->type);
		sp_exp.push_back(lights[i]->Specularity_exponent);
		sp_ligth_colors.push_back((lights[i]->specalar)*(lights[i]->strengte_specalar));
		//directions.push_back(lights[i]->direction);
	}
	for (int i = 0; i < models.size(); i++)
	{
		directions.clear();
		position.clear();
		for (int j = 0; j < lights.size(); j++)
		{
			glm::vec4 v55= 
				glm::inverse(models[i]->getModelTransform())
				*glm::vec4((lights[j]->direction),1);
			v55 = glm::normalize(v55);
			directions.push_back(v55);

			
			v55 = 
					glm::inverse(models[i]->getModelTransform())
				*glm::vec4((lights[j]->getPosition()), 1);
			position.push_back(v55);
		}
		glm::vec4 v55 = glm::inverse(models[i]->getModelTransform())
			*(cameras[ActiveCamera]->pos - cameras[ActiveCamera]->at);
		//first set worldTransformation & nTransformation of the object in renderer
		renderer->SetObjectMatrices(models.at(i)->getWorldTransform(),
			models.at(i)->getNormalTransform());
		renderer->DrawTriangles(models.at(i)->Draw(), models.at(i)->getVertexPosNum()
			, AMcolors_model[i], Difcolors_model[i], SPECTcolors_model[i],
			w, h, windowresizing, models.at(i), cameras[this->ActiveCamera]
			, ambient * strengte_ambient, diffus, position, directions, ligth_type
			, v55,sp_exp, sp_ligth_colors,type, i);
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
				directions.clear();
				position.clear();
				for (int j = 0; j < lights.size(); j++)
				{
					glm::vec4 v55 =
						glm::inverse(models[i]->getModelTransform())
						*glm::vec4((lights[j]->direction), 1);
					v55 = glm::normalize(v55);
					directions.push_back(v55);


					v55 =
						glm::inverse(models[i]->getModelTransform())
						*glm::vec4((lights[j]->getPosition()), 1);
					position.push_back(v55);
				}
				glm::vec4 v55 = glm::inverse(models[i]->getModelTransform())
					*(cameras[ActiveCamera]->pos - cameras[ActiveCamera]->at);
				//first set worldTransformation & nTransformation of the object in renderer
				renderer->SetObjectMatrices(cameras[i]->getCamBox()->getWorldTransform(),
					cameras[i]->getCamBox()->getNormalTransform());
				renderer->DrawTriangles(cameras[i]->getCamBox()->Draw(),
					cameras[i]->getCamBox()->getVertexPosNum()
					, AMcolors_model[i], Difcolors_model[i], SPECTcolors_model[i],
					w, h, windowresizing, cameras[i]->getCamBox(), cameras[this->ActiveCamera]
					, ambient * strengte_ambient, diffus, position, directions, ligth_type
					, v55, sp_exp, sp_ligth_colors, type, i + models.size());
			}
		}
	}

	renderer->SwapBuffers();
	for (int i = 0; i < w; i++)
		delete[] renderer->zBuffer[i];
	delete[] renderer->zBuffer;
}
*/

void Scene::DrawOpenGL(unsigned int shaderProgram)
{
	//myCameraTransform = activeCam->get_camWorldTransform() * activeCam->get_camModelTransform();
	//renderer->SetProjection(cameras.at(ActiveCamera)->get_projection());
	//build cameraTrans
	Camera* activeCam = cameras.at(ActiveCamera);
	glm::mat4 myCameraTransform = activeCam->get_camWorldTransform() * activeCam->get_camModelTransform();
	glm::mat4 camProject = cameras.at(ActiveCamera)->get_projection();

	for (int i = 0; i < models.size(); i++)
	{
		models[i]->DrawOpenGL(shaderProgram, i, this, myCameraTransform, camProject);
	}
}



//should stay under /*  */ !
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

void Scene::LoadTexture(string filename, int place)
{
	if (place < this->models.size())
		models[place]->LoadTexture(filename);
}