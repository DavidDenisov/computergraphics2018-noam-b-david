 #pragma once
#include "ImguiMenus.h"
#include <stdio.h>
#include <stdlib.h>
// open file dialog cross platform https://github.com/mlabbe/nativefiledialog
#include <nfd.h>
//my includes:
#include <iostream>
#include <string>
#include <algorithm>
bool whil = true;
bool ligthWindow = FALSE;
float* scale_step=new float;
float* cam_step = new float;
float *c_f11 = new float; 
float *c_f12 = new float; 
float *c_f13 = new float;
bool* cam_rad = new bool;


float*leftLIST_Frustum = new float;
float* rightLIST_Frustum = new float;
float* bottomLIST_Frustum = new float;
float* topLIST_Frustum = new float;
float* nearLIST_Frustom = new float;
float* farLIST_Frustom = new float;

float*leftLIST_Orto = new float;
float* rightLIST_Orto = new float;
float* bottomLIST_Orto = new float;
float* topLIST_Orto = new float;
float* nearLIST_Orto = new float;
float* farLIST_Orto = new float;
float* c_f1 = new float;
float* c_f2 = new float;
float* c_f3 = new float;
bool* self_prspective = new bool;
bool* frustom = new bool;
bool* orto = new bool;
bool* prespective = new bool;
float* nearLIST_Perpective = new float;
float* farLIST_Perpective = new float;
float* aspectLIST = new float;
float* fovyLIST = new float;

bool* cam_look_at = new bool;
# define PI 3.141592653589793238462643383279502884L /* pi */
int mod_counter=0;
vector<glm::vec3> zero;
vector<glm::vec3> zero_cam;
float *f11, *f12, *f13;
float *f21, *f22, *f23;
float *f31, *f32, *f33;
float d1=0, d2 = 0, d3 = 0;
bool* rad = new bool;
bool* deg = new bool;
bool showDemoWindow = false;
bool* in_place1 =new bool;
bool* in_place = new bool;
bool showAnotherWindow = false;
bool showFile = false;
bool first = TRUE;
bool modelsWindow = false;
bool camerasWindow = false;
bool* camewid = new bool;
bool* showcame = new bool;
bool* modwid = new bool;
bool* Lightswid = new bool;
int camewid_num=0;

int num=0;
glm::vec3 a;
vector<glm::vec3> rotation_cam;
vector<glm::vec3> scale_cam;
vector<glm::vec3> transformLIST_cam;
vector<glm::vec3> rotation;
vector<glm::vec3> scale;
vector<glm::vec3> transformLIST;
bool flat = TRUE,  Phong = FALSE, Gouraud = FALSE;
glm::vec4 clearColor = glm::vec4(0.4f, 0.55f, 0.60f, 1.00f);

glm::vec3 Color = glm::vec4(0.0f, 0.0f, 0.f, 1.00f);
vector<glm::vec3> ligth_rotation;
vector<glm::vec3> ligth_transform;
vector<float> ligth_step;
void zoom(Camera* cam , int place, int projection_type, int zoom)
{
	if (projection_type == 0)
		cam->Frustum(leftLIST_Frustum[place]*zoom, rightLIST_Frustum[place] * zoom, bottomLIST_Frustum[place] * zoom
		,topLIST_Frustum[place] * zoom, nearLIST_Frustom[place] * zoom, farLIST_Frustom[place] * zoom);
	else
	{
		if (projection_type == 1)
		cam->Perspective(fovyLIST[place] * zoom, aspectLIST[place] * zoom, nearLIST_Perpective[place] * zoom
			, farLIST_Perpective[place] * zoom);
		else
		cam->Ortho(leftLIST_Orto[place] * zoom, rightLIST_Orto[place] * zoom, bottomLIST_Orto[place] * zoom
		, topLIST_Orto[place] * zoom, nearLIST_Orto[place] * zoom, farLIST_Orto[place] * zoom);
	}

}
void addLigth(Scene* scene,bool type)
{
	
	int size = scene->getLights().size();
	scene->add_Light();
	Lightswid[size] = FALSE;
	scene->get_Light(size)->type = type;
	ligth_rotation.push_back(glm::vec3(0, 0, 0));
	ligth_transform.push_back(glm::vec3(0, 0, 0));
	ligth_step.push_back(0);
}
void removeLigth(Scene *scene, int place)
{
	scene->remove_Light(place);
	ligth_rotation.erase(ligth_rotation.begin() + place);
	ligth_transform.erase(ligth_transform.begin() + place);
	ligth_step.erase(ligth_step.begin() + place);
}
void add_model(Scene *scene)
{
	num = scene->getModels().size();
	modwid[num] =in_place[num] = FALSE;
	rotation.push_back(glm::vec3(0, 0, 0));
	scale.push_back(glm::vec3(1, 1, 1));
	transformLIST.push_back(glm::vec3(0, 0, 0));
	zero.push_back(glm::vec3(0, 0, 0));
	f11[num] = 0.0f; f12[num] = 0.0f; f13[num] = 0.0f;
	f21[num] = 0.0f; f22[num] = 0.0f; f23[num] = 0.0f;
	f31[num] = 1.0f; f32[num] = 1.0f; f33[num] = 1.0f;
	deg[num] = TRUE;
	rad[num] = FALSE;
	scale_step[num] = 0.f;
}
void remove_model(Scene *scene,int place)
{
	if ((place < 0) || (place>scene->getModels().size()))
		return;
	scene->RemoveModel(place);
	modwid[place] = FALSE;
	rotation.erase(rotation.begin() + place);
	scale.erase(scale.begin() + place);
	transformLIST.erase(transformLIST.begin() + place);
	zero.erase(zero.begin() + place);
	f11[num] = 0.0f; f12[num] = 0.0f; f13[num] = 0.0f;
	f21[num] = 0.0f; f22[num] = 0.0f; f23[num] = 0.0f;
	f31[num] = 1.0f; f32[num] = 1.0f; f33[num] = 1.0f;
	deg[num] = TRUE;
	rad[num] = FALSE;
	scale_step[num] = 0.f;
	rotation_cam.push_back(glm::vec3(0,0,0));
	scale_cam.push_back(glm::vec3(1, 1, 1));
	transformLIST_cam.push_back(glm::vec3(0, 0, 0));
	c_f1[0] = c_f2[0] = c_f3[0] = 0.f;
}
/*
void rotate_by_key(Scene *scene,Camera* cam,int key,bool in_place3,float f,int i)
{
	glm::mat4x4 mat = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);

	float x = zero[i][0], y = zero[i][1], z = zero[i][2];

	if (in_place3)
		mat=cam->GetTranslateTransform(-x, -y, -z)*mat;

	if (key == GLFW_KEY_UP)
		mat = cam->GetrotationTransform(f, 1)*mat;

	if (key == GLFW_KEY_DOWN)
		mat = cam->GetrotationTransform(-f, 1)*mat;


	if (key == GLFW_KEY_LEFT)
		mat = cam->GetrotationTransform(f, 2)*mat;

	if (key == GLFW_KEY_RIGHT)
		mat = cam->GetrotationTransform(-f, 2)*mat;

	if (key == 'A')
		mat = cam->GetrotationTransform(f, 0)*mat;


	if (key == 'D')
		mat = cam->GetrotationTransform(-f, 0)*mat;

	
	if (in_place3)
		mat = cam->GetTranslateTransform(x, y, z)*mat;
	
	scene->transformModel(mat);
	zero[i] = glm::vec4(x,y,z,1) * mat;
}
*/
void translate_by_key(Scene *scene, Camera* cam, int key,float f,int i)
{
	glm::mat4x4 mat = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	if (key == 'Y')
		mat = cam->GetTranslateTransform(f, 0, 0)*mat; //right
	if (key == 'R')
		mat = cam->GetTranslateTransform(-f, 0, 0)*mat; //left
	if (key == 'T')
		mat = cam->GetTranslateTransform(0, f, 0)*mat; //up
	if (key == 'G')
		mat = cam->GetTranslateTransform(0, -f, 0)*mat; //down
	if (key == 'F')
		mat = cam->GetTranslateTransform(0, 0,f)*mat; //backward (closer)
	if (key == 'H')
		mat = cam->GetTranslateTransform(0, 0, -f)*mat; //forward (farther)

	scene->transformModel(mat);
	zero[i] = mat*glm::vec4(zero[i][0], zero[i][1], zero[i][2], 1) ;
	transformLIST[i] = mat*glm::vec4(transformLIST[i][0], transformLIST[i][1], transformLIST[i][2], 1);
}
void scale_by_key(Scene *scene, Camera* cam, int key,float f,int i,bool inplace)
{
	if (f == 0)
		return;
	if ((key != 'S') && (key!= 'W'))
		return;
	if (key == 'S')
		f = 1/f;
	
	glm::mat4x4 mat=cam->GetScaleTransform(f, f, f);  //scale-trans ,world
	if (inplace) //model
	{
		glm::mat4x4 transToCenter = cam->GetTranslateTransform(-zero[i][0], -zero[i][1], -zero[i][2]);
		mat = glm::inverse(transToCenter)* cam->GetScaleTransform(f, f, f) * transToCenter;
	}
		
	scene->transformModel(mat); //do the trans
	
	
	//update zero & scale data
	zero[i] = mat * glm::vec4(zero[i][0],zero[i][1],zero[i][2],1);
	scale[i] = mat * glm::vec4(scale[i][0], scale[i][1], scale[i][2], 1);
}

void cam_translate_by_key(Scene *scene, Camera* cam, int key, float f, int i, bool inplace)
{
	glm::mat4x4 mat = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	if (key == 'Y')
		mat = cam->GetTranslateTransform(f, 0, 0)*mat; //right
	if (key == 'R')
		mat = cam->GetTranslateTransform(-f, 0, 0)*mat; //left
	if (key == 'T')
		mat = cam->GetTranslateTransform(0, f, 0)*mat; //up
	if (key == 'G')
		mat = cam->GetTranslateTransform(0, -f, 0)*mat; //down
	if (key == 'F')
		mat = cam->GetTranslateTransform(0, 0, f)*mat; //backward (closer)
	if (key == 'H')
		mat = cam->GetTranslateTransform(0, 0, -f)*mat; //forward (farther)

	cam->update_transform(mat);

	//don't think it'll matter, but shouldn't change what shouldn't be changed
	if (inplace)
	{
		cam->getCamBox()->transformModel(mat); //model
		cam->update_camModelTransform(mat);
	}

	else
	{
		cam->getCamBox()->transformWorld(mat); //world
		cam->update_camWorldTransform(mat);
	}


	if (!inplace) //if inplace, don't change his pos & at
		cam->pos = mat * cam->pos; //move the center
	
	



	//look at update
	if (cam_look_at[i]) //if he should look on the active model
	{
		const glm::vec4 atModel = glm::vec4(
			zero[scene->ActiveModel].x, zero[scene->ActiveModel].y, zero[scene->ActiveModel].z, 1.0f);
		cam->LookAt(cam->pos, atModel, cam->up);
	}


	//zero_cam & transformLIST update

	zero_cam[i] = mat * glm::vec4(zero_cam[i][0], zero_cam[i][1], zero_cam[i][2], 1);
	transformLIST_cam[i] = mat * glm::vec4(transformLIST_cam[i][0], transformLIST_cam[i][1],
		transformLIST_cam[i][2], 1);
}
void cam_scale_by_key(Scene *scene, Camera* cam, int key, float f, int i, bool inplace)
{
	if (f == 0)
		return;
	if ((key != 'S') && (key != 'W'))
		return;
	if (key == 'S')
		f = 1 / f;

	glm::mat4x4 mat = cam->GetScaleTransform(f, f, f);  //scale-trans ,world

	//camBox update
	if (inplace)
	{
		cam->getCamBox()->transformModel(mat); //model
		cam->update_camModelTransform(mat);
	}

	else
	{
		cam->getCamBox()->transformWorld(mat); //world
		cam->update_camWorldTransform(mat);
	}


	if (inplace) //model
	{
		cam->up = mat * cam->up; //because might use minus

		glm::mat4x4 transToCenter = cam->GetTranslateTransform(-cam->pos.x, -cam->pos.y, -cam->pos.z);
		mat = glm::inverse(transToCenter) * mat * transToCenter;

		cam->update_transform(mat);
		cam->pos = mat * cam->pos;
		
	}
	else //world
	{
		cam->update_transform(mat);
		cam->pos = mat * cam->pos;
		cam->up = mat * cam->up; //because might use minus
	}

	
	

	//look-at update
	if (cam_look_at[i]) //if he should look on the active model
	{
		const glm::vec4 atModel = glm::vec4(
			zero[scene->ActiveModel].x, zero[scene->ActiveModel].y, zero[scene->ActiveModel].z, 1.0f);
		cam->LookAt(cam->pos, atModel, cam->up);
	}

	//update zero & scale data

	//mat update was done above
	zero_cam[i] = mat * glm::vec4(zero_cam[i][0], zero_cam[i][1], zero_cam[i][2], 1);
	scale_cam[i] = mat * glm::vec4(scale_cam[i][0], scale_cam[i][1], scale_cam[i][2], 1);
}

const glm::vec4& GetClearColor()
{
	return clearColor;
}
void loadOBJ(Scene *scene)
{
	nfdchar_t *outPath = NULL;
	nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
	if (result == NFD_OKAY) 
	{
		std::string filename(outPath);
		std::cout << filename << std::endl;
		//replace all '\' to '/' in the path
		replace(filename.begin(), filename.end(), '\\', '/');
		scene->LoadOBJModel(filename);
	}
	else if (result == NFD_CANCEL) {
	}
	else {
	cout << "why...?";
	}
	delete outPath;
}
void DrawImguiMenus(ImGuiIO& io, Scene* scene, GLFWwindow* window)
{
	if (first)
	{
		first = modwid[0] = camewid[0] = showcame[0] = self_prspective[0] = cam_look_at[0] = Lightswid[0] = FALSE;
		rotation.clear();
		rotation.push_back(glm::vec3(0, 0, 0));
		scale.push_back(glm::vec3(1, 1, 1));
		transformLIST.push_back(glm::vec3(0, 0, 0));
		zero.push_back(glm::vec3(0, 0, 0));

		f11 = new float; f12 = new float; f13 = new float;
		f21 = new float; f22 = new float; f23 = new float;
		f31 = new float; f32 = new float; f33 = new float;
		nearLIST_Perpective[num] = bottomLIST_Orto[num] = nearLIST_Orto[num] = leftLIST_Orto[num]
			= nearLIST_Frustom[num] = bottomLIST_Frustum[num] = leftLIST_Frustum[num] = cam_step[num] = 0.f;

		farLIST_Perpective[num] = topLIST_Orto[num] = farLIST_Orto[num] = rightLIST_Orto[num]
			= farLIST_Frustom[num] = topLIST_Frustum[num] = rightLIST_Frustum[num] = 1.f;

		aspectLIST[num] = fovyLIST[num] = 90.f;

		in_place[0] = in_place1[0] = FALSE;
		scale_cam.push_back(glm::vec3(1, 1, 1));
		transformLIST_cam.push_back(glm::vec3(0, 0, -1));
		rotation_cam.push_back(glm::vec3(0, 0, 0));
		c_f1[0] = c_f2[0] = c_f3[0] = 1.f;
		c_f11[0] = c_f12[0] = c_f13[0] = 0.f;
		zero_cam.push_back(glm::vec3(0, 0, -1));
		cam_rad[0] = FALSE;
		
		ligth_rotation.push_back(glm::vec3(0, 0, 0));
		ligth_transform.push_back(glm::vec3(0, 0, 0));
		ligth_step.push_back(0);
	}

	if (scene->getModels().size() > 0)
		for (int i = 0; i < scene->getCameras().size(); i++)
			if (cam_look_at[i])
				scene->getCameras()[i]->LookAt(scene->getCameras()[i]->pos,
					glm::vec4(zero[scene->ActiveModel], 1.0f), scene->getCameras()[i]->up);

	string str;
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Once);
	// 1. Show a simple window.
	// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
	{
		ImGui::Begin("Test Menu");
		static float f = 0.0f;
		static int counter = 0;
		// Display some text (you can use a format string too)
		ImGui::Text("Hello, world!\nHere you can change which model will be viewed and view stats about the model");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", (float*)&clearColor); // Edit 3 floats representing a color

		ImGui::Checkbox("Demo Window", &showDemoWindow);      // Edit bools storing our windows open/close state
		ImGui::Checkbox("Another Window", &showAnotherWindow);
		ImGui::Checkbox("Models Window", &modelsWindow);
		ImGui::Checkbox("Cameras Window", &camerasWindow);
		ImGui::Checkbox("Ligths Window", &ligthWindow);
		if (ImGui::Button("Button")) // Buttons return true when clicked (NB: most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
	if (showAnotherWindow)
	{
		int val[2]; val[0] = io.MousePos.x; val[1] = io.MousePos.y;
		ImGui::Begin("Another Window", &showAnotherWindow);
		ImGui::InputInt2("(x,y)", val, 3);
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			showAnotherWindow = false;
		ImGui::End();
	}

	// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
	if (showDemoWindow)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
		ImGui::ShowDemoWindow(&showDemoWindow);
	}

	// Demonstrate creating a fullscreen menu bar and populating it.
	{
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoFocusOnAppearing;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open", "CTRL+O"))
				{
					nfdchar_t *outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("obj;png,jpg", NULL, &outPath);
					if (result == NFD_OKAY) {
						ImGui::Text("Hello from another window!");
						std::cout << "success\n";

						std::string filename(outPath);
						std::cout << filename << std::endl;
						//replace all '\' to '/' in the path
						std::replace(filename.begin(), filename.end(), '\\', '/');
						scene->LoadOBJModel(filename);

						std::cout << "test LoadOBJ\n";
						std::cout << "active Model: " << scene->ActiveModel << std::endl;
						std::cout << "num of models: " << scene->getModels().size() << std::endl;
						std::cout << "Models[0] " << scene->getModels()[0]->getNameModel() << std::endl;
						std::cout << "Models[1] " << scene->getModels()[1]->getNameModel() << std::endl;


						//free(outPath);
					}
					else if (result == NFD_CANCEL) {
					}
					else {
					std:cout << "why...?";
					}

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Demo Menu")) { showDemoWindow = true; }
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	if (modelsWindow)
	{
		ImGui::Begin("Models", &modelsWindow);

		if (scene->getModels().size() > 0)
		{
			MeshModel* Active = scene->getModels()[scene->ActiveModel];
			str = "the active model is model " +
				Active->getNameModel() + " " + to_string(scene->ActiveModel);
			ImGui::Text(const_cast<char*>(str.c_str()));

			ImGui::Checkbox("show the normals of the vertices of the active MODEL : "
				,&Active->willDrawVertexNormal);

			ImGui::Checkbox("show the normals of the faces of the active MODEL : "
				, &Active->willDrawFaceNormal);

			ImGui::Checkbox("draw the box of active MODEL : "
				, &Active->willDrawBox);
		}

		if (ImGui::Button("ADD MESH Model"))
		{
			add_model(scene);
			loadOBJ(scene);
		}

		if (ImGui::Button("ADD PRIM Model"))
		{
			add_model(scene);
			scene->LoadPrim();
		}

		for (int i = 0; i < scene->getModels().size(); i++)
		{
			str = "show the window of MODEL : "
				+ scene->getModels()[i]->getNameModel() + " " + to_string(i);
			ImGui::Checkbox(const_cast<char*>(str.c_str()), &modwid[i]);
			str = "REMOVE MODEL : " + scene->getModels()[i]->getNameModel()
				+ " " + to_string(i);
			if (ImGui::Button(const_cast<char*>(str.c_str())))
				remove_model(scene, i);
		}


		ImGui::End();

	}
	if (camerasWindow)
	{
		ImGui::Begin("Cameras", &modelsWindow);
		if (ImGui::Button("ADD CAMERA"))
		{

			num = scene->getCameras().size();
			camewid[num] = showcame[num] = frustom[num] = frustom[num] = cam_look_at[num] = orto[num]
				= prespective[num] = self_prspective[num] = FALSE;

			nearLIST_Perpective[num] = bottomLIST_Orto[num] = nearLIST_Orto[num]= leftLIST_Orto[num] 
				= nearLIST_Frustom[num] = bottomLIST_Frustum[num]=leftLIST_Frustum[num] = 0.f;

			farLIST_Perpective[num] =  topLIST_Orto[num] = farLIST_Orto[num] = rightLIST_Orto[num] 
				= farLIST_Frustom[num]=topLIST_Frustum[num]= rightLIST_Frustum[num] = 1.f;
				  
			aspectLIST[num] = fovyLIST[num]=90.f;

			cam_step[num] = 0;
			scale_cam.push_back(glm::vec3(1, 1, 1));
			transformLIST_cam.push_back(glm::vec3(0, 0, 0));
			rotation_cam.push_back(glm::vec3(0, 0, 0));
			c_f1[num] = c_f2[num] = c_f3[num] = 1.f;
			c_f11[num] = c_f12[num] = c_f13[num] = 0.f;
			zero_cam.push_back(glm::vec3(0, 0, 1));
			cam_rad[num] = FALSE;
			scene->load_cam();
		}

		str = "the active camera is camera number " + to_string(
			scene->getCameras()[scene->ActiveModel]->num + 1);
		ImGui::Text(const_cast<char*>(str.c_str()));
		ImGui::Checkbox("show the cameras boxes : ", &scene->willCamerasRender);
		for (int i = 0; i < scene->getCameras().size(); i++)
		{
			num = scene->getCameras()[i]->num + 1;
			str = "show the window of camera number " + to_string(num);
			ImGui::Checkbox(const_cast<char*>(str.c_str()), &camewid[i]);
			str = "REMOVE CAMERA NUMBER " + to_string(num);
			if (scene->getCameras().size() > 1)
				if (ImGui::Button(const_cast<char*>(str.c_str())))
				{
					scene->remove_cam(i);
					camewid[i] = FALSE;
				}
		}
		ImGui::End();

	}
	if (ligthWindow)
	{
		ImGui::Begin("Lights", &ligthWindow);
		if (scene->getLights().size() > 0)
		{
			str = "the active model is Light " +
				 to_string(scene->ActiveLight);
			ImGui::Text(const_cast<char*>(str.c_str()));
		}
		ImGui::ColorEdit3("ambient ligth", (float*)&scene->ambient);
		ImGui::SliderFloat("ambient strength", &scene->strengte_ambient, 0.0f, 1);

		ImGui::Text("flat :");
		ImGui::Checkbox("flat", &flat);
		if (flat)
			Gouraud = Phong = FALSE, scene->type = 0;
		ImGui::Checkbox("Gouraud ", &Gouraud);
		if (Gouraud)
			flat = Phong = FALSE, scene->type = 1;
		ImGui::Checkbox("Phong", &Phong);
		if (Phong)
			flat = Gouraud = FALSE, scene->type = 2;

		if (ImGui::Button("ADD Point Light"))
			addLigth(scene,false);
		if (ImGui::Button("ADD Parallel  Light"))
			addLigth(scene, true);
			

		for (int i = 0; i < scene->getLights().size(); i++)
		{
			str = "show the window of Ligth : " + to_string(i);
			ImGui::Checkbox(const_cast<char*>(str.c_str()), &Lightswid[i]);
			if (scene->getLights().size() > 1)
			{
				str = "REMOVE Ligth : " + to_string(i);
				if (ImGui::Button(const_cast<char*>(str.c_str())))
					removeLigth(scene,i);
			}
		}


		ImGui::End();
		
	}

	for (int i = 0; i < scene->getLights().size(); i++)
	{
		if (Lightswid[i])
		{
			str = "Light " + to_string(i);
			ImGui::Begin(const_cast<char*>(str.c_str()), &Lightswid[i]);
			Light* cur = scene->getLights()[i];
			ImGui::ColorEdit3("Diffuse ligth", (float*)&cur->difus);
			ImGui::SliderFloat("Diffuse strength", &cur->strengte_difus, 0.0f, 1);


			ImGui::ColorEdit3("Specular ligth", (float*)&cur->specalar);
			ImGui::SliderFloat("Specular strength", &cur->strengte_specalar, 0.0f, 1);

			if (scene->getLights()[i]->type)
				ImGui::Text("Parallel  SOURCE");
			else
				ImGui::Text("Point SOURCE");



			ImGui::InputFloat("step :", &ligth_step[i], 0.0f, 0.0f);

			glm::vec3 l_a;
			if (scene->getLights()[i]->type)
			{
				l_a = scene->getLights()[i]->direction;
				str = "Direction : x:" + to_string(l_a.x) + ", y : "
					+ to_string(l_a.y) + ",  z : " + to_string(l_a.z) + " .";
				ImGui::Text(const_cast<char*>(str.c_str()));
			}
			else
			{
				l_a = scene->getLights()[i]->getPosition();
				str = "Position : x:" + to_string(l_a.x) + ", y : "
					+ to_string(l_a.y) + ",  z : " + to_string(l_a.z) + " .";
				ImGui::Text(const_cast<char*>(str.c_str()));
			}
			l_a = ligth_rotation[i];
			str = "Rotation : x:" + to_string(l_a.x) + ", y : "
				+ to_string(l_a.y) + ",  z : " + to_string(l_a.z) + " .";
			ImGui::Text(const_cast<char*>(str.c_str()));

			l_a = ligth_transform[i];
			str = "Transform : x:" + to_string(l_a.x) + ", y : "
				+ to_string(l_a.y) + ",  z : " + to_string(l_a.z) + " .";
			ImGui::Text(const_cast<char*>(str.c_str()));

			if (scene->getLights()[i]->type)
			{
				if (ImGui::Button("Reset direction"))
					scene->getLights()[i]->direction = glm::vec3(0, 0, 1);
			}
			else
			{
				if (ImGui::Button("Reset  position"))
					scene->getLights()[i]->resetPosition();
			}
			ImGui::InputInt("Specularity exponent", &scene->getLights()[i]->Specularity_exponent);
			if (!scene->get_Light(i)->type)
			{
			glm::mat4x4 rot;
			glm::mat4x4 trans;
			Camera cam = scene->getCameras()[0];
			rot = trans = cam.GetrotationTransform(0, 0);
			float step = ligth_step[i];
			float step_r = PI * (step / 180.0);
			if (step != 0.f)
			{
				if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				{
					ligth_rotation[i].y = ligth_rotation[i].y + ligth_step[i];
					rot = cam.GetrotationTransform(step_r, 0)*rot;
				}

				if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				{
					ligth_rotation[i].y = ligth_rotation[i].y - ligth_step[i];
					rot = cam.GetrotationTransform(-step_r, 0)*rot;
				}

				if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //look sideway right (z)
				{
					ligth_rotation[i].z = ligth_rotation[i].z + ligth_step[i];
					rot = cam.GetrotationTransform(step_r, 2)*rot;
				}

				if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //look sideway left (z)
				{
					ligth_rotation[i].z = ligth_rotation[i].z - ligth_step[i];
					rot = cam.GetrotationTransform(-step_r, 2)*rot;
				}
				if (glfwGetKey(window, 'A') == GLFW_PRESS) //look to the left (y)
				{
					ligth_rotation[i].x = ligth_rotation[i].x - ligth_step[i];
					rot = cam.GetrotationTransform(-step_r, 1)*rot;
				}
				if (glfwGetKey(window, 'D') == GLFW_PRESS) //look to the right (y)
				{
					ligth_rotation[i].x = ligth_rotation[i].x + ligth_step[i];
					rot = cam.GetrotationTransform(step_r, 1)*rot;
				}

				if (glfwGetKey(window, 'Y') == GLFW_PRESS)
				{
					ligth_transform[i].x = ligth_transform[i].x + ligth_step[i]; //right
					trans = cam.GetTranslateTransform(step, 0, 0)*trans;
				}
				if (glfwGetKey(window, 'R') == GLFW_PRESS)
				{
					ligth_transform[i].x = ligth_transform[i].x - ligth_step[i]; //left
					trans = cam.GetTranslateTransform(-step, 0, 0)*trans;
				}
				if (glfwGetKey(window, 'T') == GLFW_PRESS)
				{
					ligth_transform[i].y = ligth_transform[i].y + ligth_step[i]; //up
					trans = cam.GetTranslateTransform(0, step, 0)*trans;
				}
				if (glfwGetKey(window, 'G') == GLFW_PRESS)
				{
					ligth_transform[i].y = ligth_transform[i].y - ligth_step[i]; //down
					trans = cam.GetTranslateTransform(0, -step, 0)*trans;
				}
				if (glfwGetKey(window, 'F') == GLFW_PRESS)
				{
					ligth_transform[i].z = ligth_transform[i].z + ligth_step[i];
					trans = cam.GetTranslateTransform(0, 0, -step)*trans;
				}
				if (glfwGetKey(window, 'H') == GLFW_PRESS)
				{
					ligth_transform[i].z = ligth_transform[i].z - ligth_step[i];
					trans = cam.GetTranslateTransform(0, 0, step)*trans;
				}
			}


			while (ligth_rotation[i].x < 0)
				ligth_rotation[i].x += 360;
			while (ligth_rotation[i].y < 0)
				ligth_rotation[i].y += 360;
			while (ligth_rotation[i].z < 0)
				ligth_rotation[i].z += 360;

			while (ligth_rotation[i].x > 360)
				ligth_rotation[i].x -= 360;
			while (ligth_rotation[i].y > 360)
				ligth_rotation[i].y -= 360;
			while (ligth_rotation[i].z > 360)
				ligth_rotation[i].z -= 360;

			
				scene->get_Light(i)->transformPosition(rot*trans);
			}		
			else
			{
				ImGui::SliderFloat("Diffuse direction x", &scene->getLights()[i]->direction.x, -1, 1);
				ImGui::SliderFloat("Diffuse direction y", &scene->getLights()[i]->direction.y, -1, 1);
				ImGui::SliderFloat("Diffuse direction z", &scene->getLights()[i]->direction.z, -1, 1);

			}
			ImGui::End();
		}
	}
	for (int i = 0; i < scene->getModels().size(); i++)
	{
		if (modwid[i])
		{
			str = "MODEL : " + scene->getModels()[i]->getNameModel() + to_string(i);
			ImGui::Begin(const_cast<char*>(str.c_str()), &modelsWindow);
			if (ImGui::Button("make active"))
				scene->ActiveModel = i;

			if (!scene->getModels()[i]->folow_the_mouse)
				if (ImGui::Button("folow the mouse"))
					scene->getModels()[i]->folow_the_mouse = TRUE;

			if (scene->getModels()[i]->folow_the_mouse)
				if (ImGui::Button("STOP FOLOWING THE MOUSE"))
					scene->getModels()[i]->folow_the_mouse = FALSE;


			Color = scene->getColor(i, 0,0);
			ImGui::ColorEdit3("ambient color", (float*)&Color);
			scene->setColor(i, Color, 0,0);

			Color = scene->getColor(i, 0, 1);
			ImGui::ColorEdit3("diffus color", (float*)&Color);
			scene->setColor(i, Color, 0, 1);

			Color = scene->getColor(i, 0, 2);
			ImGui::ColorEdit3("spectecular color", (float*)&Color);
			scene->setColor(i, Color, 0, 2);

			str = "zero: x:" + to_string(zero[i][0]) + ", y : "
				+ to_string(zero[i][1]) + ",  z : " + to_string(zero[i][2]) + " .";
			ImGui::Text(const_cast<char*>(str.c_str()));

			a = scale[i];
			str = "SCALE: ratio of x:" + to_string(a[0]) + "to 1 , ratio of y : "
				+ to_string(a[1]) + "to 1 ,ratio of z : " + to_string(a[2]) + "to 1.";
			ImGui::Text(const_cast<char*>(str.c_str()));

			a = transformLIST[i];
			str = "TRANSPOSE: x:" + to_string(a[0]) + ", y : "
				+ to_string(a[1]) + ",  z : " + to_string(a[2]) + " .";
			ImGui::Text(const_cast<char*>(str.c_str()));

			a = rotation[i];
			str = "ROTATE in degrees: x:" + to_string(a[0]) + " , y : "
				+ to_string(a[1]) + ",  z : " + to_string(a[2]) + ".";

		
			ImGui::Text(const_cast<char*>(str.c_str()));

			if (i == scene->ActiveModel)
			{
				ImGui::InputFloat("step", &scale_step[i], 0.0f, 0.0f);
				float step = scale_step[i];
				float ratio = 1;

				a = rotation[i];
				if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
					f12[i] = f12[i] + step;

				if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
					f12[i] = f12[i] - step;

				if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
					f13[i] = f13[i] + step;

				if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
					f13[i] = f13[i] - step;

				if (glfwGetKey(window, 'A') == GLFW_PRESS)
					f11[i] = f11[i] - step;


				if (glfwGetKey(window, 'D') == GLFW_PRESS)
					f11[i] = f11[i] + step;
				Camera* cam = scene->getCameras()[0];
				if (rad[i])
				{
					if (ImGui::Button("deg"))
					{
						rad[i] = FALSE;
						deg[i] = TRUE;
						scene->setModeltransform(glm::mat4x4(1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1));
						scene->transformModel(cam->GetScaleTransform(scale[i][0], scale[i][1], scale[i][2]));
						scene->transformModel(cam->GetTranslateTransform(
							transformLIST[i][0], transformLIST[i][1], transformLIST[i][2]));
						f11[i] = f12[i] = f13[i] = 0;
						//rotation[i] = glm::vec3(0, 0, 0);
					}
					else
					{
						ImGui::SliderFloat("rotation x rad", &f11[i], 0.0f, 2 * PI);
						d1 = a[0] - f11[i];
						ImGui::SliderFloat("rotation y rad", &f12[i], 0.0f, 2 * PI);
						d2 = a[1] - f12[i];
						ImGui::SliderFloat("rotation z rad", &f13[i], 0.0f, 2 * PI);
						d3 = a[2] - f13[i];
						whil = true;
						while (whil)
						{
							whil = FALSE;
							if (f11[i] < 0.f)
								f11[i] += 2 * PI, whil = TRUE;


							if (f12[i] < 0.f)
								f12[i] += 2 * PI, whil = TRUE;


							if (f13[i] < 0.f)
								f13[i] += 2 * PI, whil = TRUE;


							if (f11[i] > 2 * PI)
								f11[i] -= 2 * PI, whil = TRUE;


							if (f12[i] > 2 * PI)
								f12[i] -= 2 * PI, whil = TRUE;


							if (f13[i] > 2 * PI)
								f13[i] -= 2 * PI, whil = TRUE;


						}
					}
				}
				else
				{
					if (ImGui::Button("rad"))
					{
						deg[i] = FALSE; rad[i] = TRUE;
						scene->setModeltransform(glm::mat4x4(1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							0, 0, 0, 1));
						scene->transformModel(cam->GetScaleTransform(scale[i][0], scale[i][1], scale[i][2]));
						scene->transformModel(cam->GetTranslateTransform(
							transformLIST[i][0], transformLIST[i][1], transformLIST[i][2]));
						f11[i] = f12[i] = f13[i] = 0;
						rotation[i] = glm::vec3(0, 0, 0);
					}
					else
					{
						ImGui::SliderFloat("rotation x degrees", &f11[i], 0.0f, 360.0f);
						d1 = (a[0] - f11[i])*(PI / 180.0);
						ImGui::SliderFloat("rotation y degrees", &f12[i], 0.0f, 360.0f);
						d2 = (a[1] - f12[i])*(PI / 180.0);
						ImGui::SliderFloat("rotation z degrees", &f13[i], 0.0f, 360.0f);
						d3 = (a[2] - f13[i])*(PI / 180.0);
						ratio = PI / 180.0;
						whil = true;
						while (whil)
						{
							whil = FALSE;
							if (f11[i] < 0.f)
								f11[i] += 360.f, whil = TRUE;


							if (f12[i] < 0.f)
								f12[i] += 360.f, whil = TRUE;


							if (f13[i] < 0.f)
								f13[i] += 360.f, whil = TRUE;


							if (f11[i] > 360.f)
								f11[i] -= 360.f, whil = TRUE;


							if (f12[i] > 360.f)
								f12[i] -= 360.f, whil = TRUE;


							if (f13[i] > 360.f)
								f13[i] -= 360.f, whil = TRUE;

						}
					}

				}

				ImGui::Checkbox("transform in place", &in_place[i]);
				glm::mat4x4 mat =
					cam->GetrotationTransform(d1, 0)*
					cam->GetrotationTransform(d2, 1)*
					cam->GetrotationTransform(d3, 2);
				//rotations - model & world -- works
				if (in_place[i])
				{
					scene->transformModel(
						cam->GetTranslateTransform(zero[i][0], zero[i][1], zero[i][2]) * mat *
						cam->GetTranslateTransform(-zero[i][0], -zero[i][1], -zero[i][2])
					);
					//it doesn't affect...? he's zero anyway... (he is not a zero he represents the zero of the model)
					zero[i] = cam->GetTranslateTransform(zero[i][0], zero[i][1], zero[i][2])
						*mat*
						cam->GetTranslateTransform(-zero[i][0], -zero[i][1], -zero[i][2])*
						glm::vec4(zero[i][0], zero[i][1], zero[i][2], 1);

				}
				else
				{
					scene->transformModel(mat);
					zero[i] = mat * glm::vec4(zero[i][0], zero[i][1], zero[i][2], 1);
				}

				//scaling & translations:

				if (glfwGetKey(window, 'S') == GLFW_PRESS)
					scale_by_key(scene, cam, 'S', step, i, in_place[i]);


				if (glfwGetKey(window, 'W') == GLFW_PRESS)
					scale_by_key(scene, cam, 'W', step, i, in_place[i]);

				if (glfwGetKey(window, 'Y') == GLFW_PRESS)
					translate_by_key(scene, cam, 'Y', step, i);

				if (glfwGetKey(window, 'R') == GLFW_PRESS)
					translate_by_key(scene, cam, 'R', step, i);

				if (glfwGetKey(window, 'T') == GLFW_PRESS)
					translate_by_key(scene, cam, 'T', step, i);

				if (glfwGetKey(window, 'G') == GLFW_PRESS)
					translate_by_key(scene, cam, 'G', step, i);

				if (glfwGetKey(window, 'F') == GLFW_PRESS)
					translate_by_key(scene, cam, 'F', step, i);

				if (glfwGetKey(window, 'H') == GLFW_PRESS)
					translate_by_key(scene, cam, 'H', step, i);

				rotation[i] = glm::vec3(f11[i], f12[i], f13[i]);
				/*
						if (in_place[i])
						{
						glm::vec3 auo = glm::vec3(f21[i], f22[i], f23[i]);
						ImGui::InputFloat("transpose x", &f21[i], 0.0f, 0.0f);

						ImGui::InputFloat("transpose y", &f22[i], 0.0f, 0.0f);

						ImGui::InputFloat("transpose z", &f23[i], 0.0f, 0.0f);

						zero[i] = glm::vec3(zero[i][0] + f21[i] - auo[0],
							zero[i][1] + f22[i] - auo[1], zero[i][2] + f23[i] - auo[2]);

						scene->transformModel(cam->GetTranslateTransform
						(f21[i] - auo[0], f22[i] - auo[1], f23[i] - auo[2]));
						transformLIST[i] = glm::vec3(transformLIST[i][0] + f21[i] - auo[0]
							, transformLIST[i][1] + f22[i] - auo[1], transformLIST[i][2] + f23[i] - auo[2]);

						auo = glm::vec3(f31[i], f32[i], f33[i]);

						ImGui::InputFloat("scale x", &f31[i], 0.0f, 0.0f);
						ImGui::InputFloat("scale y", &f32[i], 0.0f, 0.0f);
						ImGui::InputFloat("scale z", &f33[i], 0.0f, 0.0f);

						if (((f31[i] != 0.f) && (f32[i] != 0.f) && (f33[i] != 0.f)))
						{
							scene->transformModel(cam->GetScaleTransform(f31[i] / auo[0],
								f32[i] / auo[1], f33[i] / auo[2]));
							zero[i] = cam->GetScaleTransform(f31[i] / auo[0], f32[i] / auo[1],
								f33[i] / auo[2])*glm::vec4(zero[i][0], zero[i][1], zero[i][2], 1);

							scale[i] = glm::vec3(f31[i], f32[i], f33[i]);
						}
						else
							f31[i] = auo.x, f32[i] = auo.y, f33[i] = auo.z;
					}
					else
					{
						glm::vec3 auo = glm::vec3(f21[i], f22[i], f23[i]);
						ImGui::InputFloat("transpose x", &f21[i], 0.0f, 0.0f);

						ImGui::InputFloat("transpose y", &f22[i], 0.0f, 0.0f);

						ImGui::InputFloat("transpose z", &f23[i], 0.0f, 0.0f);

						zero[i] = glm::vec3(zero[i][0] + f21[i] - auo[0],
							zero[i][1] + f22[i] - auo[1], zero[i][2] + f23[i] - auo[2]);

						scene->transformModel(cam->GetTranslateTransform
						(f21[i] - auo[0], f22[i] - auo[1], f23[i] - auo[2]));
						transformLIST[i] = glm::vec3(transformLIST[i][0] + f21[i] - auo[0]
							, transformLIST[i][1] + f22[i] - auo[1], transformLIST[i][2] + f23[i] - auo[2]);

						auo = glm::vec3(f31[i], f32[i], f33[i]);

						ImGui::InputFloat("scale x", &f31[i], 0.0f, 0.0f);
						ImGui::InputFloat("scale y", &f32[i], 0.0f, 0.0f);
						ImGui::InputFloat("scale z", &f33[i], 0.0f, 0.0f);

						if (((f31[i] != 0.f) && (f32[i] != 0.f) && (f33[i] != 0.f)))
						{
							scene->transformModel(cam->GetScaleTransform(f31[i] / auo[0],
								f32[i] / auo[1], f33[i] / auo[2]));
							zero[i] = cam->GetScaleTransform(f31[i] / auo[0], f32[i] / auo[1],
								f33[i] / auo[2])*glm::vec4(zero[i][0], zero[i][1], zero[i][2], 1);

							transformLIST[i][0] = transformLIST[i][0] * (f31[i] / auo[0]);
							transformLIST[i][1] = transformLIST[i][1] * (f32[i] / auo[1]);
							transformLIST[i][2] = transformLIST[i][2] * (f33[i] / auo[2]);

							f21[i] = f21[i] * (f31[i] / auo[0]);
							f22[i] = f22[i] * (f32[i] / auo[1]);
							f23[i] = f23[i] * (f33[i] / auo[2]);

							scale[i] = glm::vec3(f31[i], f32[i], f33[i]);
						}
						else
							f31[i] = auo.x, f32[i] = auo.y, f33[i] = auo.z;
							*/

			}
			ImGui::End();
		}

	}
	for (int i = 0; i < scene->getCameras().size(); i++)
	{
		if (camewid[i])
		{
			str = "camera number : " + to_string(i);
			ImGui::Begin(const_cast<char*>(str.c_str()), &modelsWindow);
			if (ImGui::Button("make active"))
				scene->ActiveCamera = i;


			Color = scene->getColor(i, 1, 0);
			ImGui::ColorEdit3("ambient color", (float*)&Color);
			scene->setColor(i, Color, 1, 0);

			Color = scene->getColor(i, 1, 1);
			ImGui::ColorEdit3("diffus color", (float*)&Color);
			scene->setColor(i, Color, 1, 1);

			Color = scene->getColor(i, 1, 2);
			ImGui::ColorEdit3("spectecular color", (float*)&Color);
			scene->setColor(i, Color, 1, 2);


			ImGui::Checkbox("auto look at", &cam_look_at[i]);
			ImGui::Checkbox("self prspective", &self_prspective[i]);
			Camera* cam = scene->getCameras()[i];
			if (self_prspective[i])
			{
				ImGui::Text("up :");
				ImGui::SliderFloat("up x :", &cam->up[0], -1.0f, 1.0f);
				ImGui::SliderFloat("up y :", &cam->up[1], -1.0f, 1.0f);
				ImGui::SliderFloat("up z :", &cam->up[2], -1.0f, 1.0f);

				ImGui::Text("position :");
				ImGui::InputFloat("position x :", &cam->pos[0]);
				ImGui::InputFloat("position y :", &cam->pos[1]);
				ImGui::InputFloat("position z :", &cam->pos[2]);

				ImGui::Text("projection :");
				ImGui::Checkbox("frustom", &frustom[i]);
				if (frustom[i])
					orto[i] = prespective[i] = FALSE;
				ImGui::Checkbox("orto", &orto[i]);
				if (orto[i])
					frustom[i] = prespective[i] = FALSE;
				ImGui::Checkbox("prespective", &prespective[i]);
				if (prespective[i])
					frustom[i] = orto[i] = FALSE;
				if (frustom[i])
				{
					ImGui::InputFloat("NEAR :", &nearLIST_Frustom[i]);
					ImGui::InputFloat("FAR :", &farLIST_Frustom[i]);

					ImGui::InputFloat("TOP:", &topLIST_Frustum[i]);
					ImGui::InputFloat("BOTTOM :", &bottomLIST_Frustum[i]);

					ImGui::InputFloat("LEFT:", &leftLIST_Frustum[i]);
					ImGui::InputFloat("RIGHT :", &rightLIST_Frustum[i]);
					cam->Ortho(leftLIST_Frustum[i], rightLIST_Frustum[i], bottomLIST_Frustum[i],
						topLIST_Frustum[i], nearLIST_Frustom[i], farLIST_Frustom[i]);
				}
				if (prespective[i])
				{
					ImGui::InputFloat("NEAR :", &nearLIST_Perpective[i]);
					ImGui::InputFloat("FAR :", &farLIST_Perpective[i]);

					ImGui::InputFloat("FOVY :", &fovyLIST[i]),
						ImGui::InputFloat("ASPECT :", &aspectLIST[i]);
					cam->Perspective(fovyLIST[i], aspectLIST[i], nearLIST_Perpective[i], farLIST_Perpective[i]);
				}
				if (orto[i])
				{
					ImGui::InputFloat("NEAR :", &nearLIST_Orto[i]);
					ImGui::InputFloat("FAR :", &farLIST_Orto[i]);

					ImGui::InputFloat("TOP:", &topLIST_Orto[i]);
					ImGui::InputFloat("BOTTOM :", &bottomLIST_Orto[i]);

					ImGui::InputFloat("LEFT:", &leftLIST_Orto[i]);
					ImGui::InputFloat("RIGHT :", &rightLIST_Orto[i]);
					cam->Ortho(leftLIST_Orto[i], rightLIST_Orto[i], bottomLIST_Orto[i],
						topLIST_Orto[i], nearLIST_Orto[i], farLIST_Orto[i]);
				}
			}
			else
			{
				//projections
				ImGui::Text("projection :");
				ImGui::Checkbox("frustom", &frustom[i]);

				int w, h;
				glfwGetWindowSize(window, &w, &h);
				float wF = float(w), hF = float(h);
				if (frustom[i])
				{
					orto[i] = prespective[i] = FALSE;
					cam->Frustum(-(wF) / hF, (wF) / hF, -1, 1, -2, -1);
				}

				ImGui::Checkbox("orto", &orto[i]);
				if (orto[i])
				{
					frustom[i] = prespective[i] = FALSE;
					cam->Ortho(-(wF)/hF, (wF) / hF, -1, 1, 1, -1);
				}

				ImGui::Checkbox("prespective", &prespective[i]);
				if (prespective[i])
				{
					frustom[i] = orto[i] = FALSE;
					cam->Frustum(-(wF) / hF, (wF) / hF, -1, 1, -2, -1);
				}


				//cam->reset_projection();
				
				ImGui::InputFloat("step :", &cam_step[i], 0.0f, 0.0f);

				// zoom in/out
				if (cam_step[i] != 0.f)
				{
					float step = cam_step[i];
					if (ImGui::Button("zoom in"))
					{
						if (orto[i])
							zoom(cam, i, 0, 1 / step);
						if (prespective[i])
							zoom(cam, i, 1, 1 / step);
						if (frustom[i])
							zoom(cam, i, 2, 1 / step);
					}

					if (ImGui::Button("zoom out"))
					{
						if (orto[i])
							zoom(cam, i, 0, step);
						if (prespective[i])
							zoom(cam, i, 1, step);
						if (frustom[i])
							zoom(cam, i, 2, step);
					}
				}


				//printing data
				a = scale_cam[i];
				str = "SCALE: ratio of x:" + to_string(a[0]) + "to 1 , ratio of y : "
					+ to_string(a[1]) + "to 1 ,ratio of z : " + to_string(a[2]) + "to 1.";
				ImGui::Text(const_cast<char*>(str.c_str()));

				a = transformLIST_cam[i];
				str = "TRANSPOSE: x:" + to_string(a[0]) + ", y : "
					+ to_string(a[1]) + ",  z : " + to_string(a[2]) + " .";
				ImGui::Text(const_cast<char*>(str.c_str()));

				a = rotation_cam[i];
				str = "ROTATE in degrees: x:" + to_string(a[0]) + " , y : "
					+ to_string(a[1]) + ",  z : " + to_string(a[2]) + ".";
				ImGui::Text(const_cast<char*>(str.c_str()));

				/*
					glm::vec3 auo = glm::vec3(transformLIST_cam[i][0]
					,transformLIST_cam[i][1], transformLIST_cam[i][2]);

					ImGui::InputFloat("transpose x", &transformLIST_cam[i][0], 0.0f, 0.0f);

					ImGui::InputFloat("transpose y", &transformLIST_cam[i][1], 0.0f, 0.0f);

					ImGui::InputFloat("transpose z", &transformLIST_cam[i][2], 0.0f, 0.0f);


					zero_cam[i] = glm::vec3(zero_cam[i][0] + transformLIST_cam[i][0] - auo[0],
				zero_cam[i][1] + transformLIST_cam[i][1] - auo[1], zero_cam[i][2] + transformLIST_cam[i][2] - auo[2]);
					cam->camTranslateTransform(transformLIST_cam[i][0] - auo[0], transformLIST_cam[i][1] - auo[1],
					transformLIST_cam[i][2] - auo[2]);



					auo = glm::vec3(c_f1[i], c_f2[i], c_f3[i]);
					ImGui::InputFloat("scale x", &c_f1[i], 0.0f, 0.0f);
					ImGui::InputFloat("scale y", &c_f2[i], 0.0f, 0.0f);
					ImGui::InputFloat("scale z", &c_f3[i], 0.0f, 0.0f);

					if (((c_f1[i] != 0.f) && (c_f2[i] != 0.f) && (c_f3[i] != 0.f)))
					{
						cam->camScaleTransform(c_f1[i] / auo[0],
							c_f2[i] / auo[1], c_f3[i] / auo[2]);
						zero_cam[i] = cam->GetScaleTransform(c_f1[i] / auo[0], c_f2[i] / auo[1],
							c_f3[i] / auo[2])*glm::vec4(zero_cam[i][0], zero_cam[i][1], zero_cam[i][2], 1);
						transformLIST_cam[i][0] = transformLIST_cam[i][0]/(c_f1[i] / auo[0]);
						transformLIST_cam[i][1] = transformLIST_cam[i][1]/(c_f1[i] / auo[0]);
						transformLIST_cam[i][2] = transformLIST_cam[i][2]/(c_f1[i] / auo[0]);
						scale_cam[i] = glm::vec3(c_f1[i], c_f2[i], c_f3[i]);
					}
					else
						c_f1[i] = auo.x, c_f2[i] = auo.y, c_f3[i] = auo.z;
					*/
				float step = cam_step[i];
				float ratio = 1;
				a = rotation_cam[i];
				if (step != 0.f && scene->ActiveCamera == i)
				{
					if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
						c_f12[i] = c_f12[i] + step;

					if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
						c_f12[i] = c_f12[i] - step;

					if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //look sideway right (z)
						c_f13[i] = c_f13[i] + step;

					if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //look sideway left (z)
						c_f13[i] = c_f13[i] - step;

					if (glfwGetKey(window, 'A') == GLFW_PRESS) //look to the left (y)
						c_f11[i] = c_f11[i] - step;

					if (glfwGetKey(window, 'D') == GLFW_PRESS) //look to the right (y)
						c_f11[i] = c_f11[i] + step;

					if (glfwGetKey(window, 'S') == GLFW_PRESS)
						cam_scale_by_key(scene, cam, 'S', step, i, in_place1[i]); //scale

					if (glfwGetKey(window, 'W') == GLFW_PRESS)
						cam_scale_by_key(scene, cam, 'W', step, i, in_place1[i]); //scale

					if (glfwGetKey(window, 'Y') == GLFW_PRESS)
						cam_translate_by_key(scene, cam, 'Y', step, i, in_place1[i]); //right

					if (glfwGetKey(window, 'R') == GLFW_PRESS)
						cam_translate_by_key(scene, cam, 'R', step, i, in_place1[i]); //left

					if (glfwGetKey(window, 'T') == GLFW_PRESS)
						cam_translate_by_key(scene, cam, 'T', step, i, in_place1[i]); //up

					if (glfwGetKey(window, 'G') == GLFW_PRESS)
						cam_translate_by_key(scene, cam, 'G', step, i, in_place1[i]); //down

					if (glfwGetKey(window, 'F') == GLFW_PRESS)
						cam_translate_by_key(scene, cam, 'F', step, i, in_place1[i]);

					if (glfwGetKey(window, 'H') == GLFW_PRESS)
						cam_translate_by_key(scene, cam, 'H', step, i, in_place1[i]);

				}
				if (cam_rad[i])
				{
					if (ImGui::Button("deg"))
					{
						cam_rad[i] = FALSE;
						c_f11[i] = c_f12[i] = c_f13[i] = 0;
						rotation[i] = glm::vec3(0, 0, 0);
					}
					else
					{
						ImGui::SliderFloat("rotation x rad", &c_f11[i], 0.0f, 2 * PI);
						d1 = c_f11[i]-a[0];
						ImGui::SliderFloat("rotation y rad", &c_f12[i], 0.0f, 2 * PI);
						d2 = c_f12[i]-a[1];
						ImGui::SliderFloat("rotation z rad", &c_f13[i], 0.0f, 2 * PI);
						d3 = c_f13[i]-a[2];
						whil = true;
						while (whil)
						{
							whil = FALSE;
							if (c_f11[i] < 0.f)
								c_f11[i] += 2 * PI, whil = TRUE;


							if (c_f12[i] < 0.f)
								c_f12[i] += 2 * PI, whil = TRUE;

							if (c_f13[i] < 0.f)
								c_f13[i] += 2 * PI, whil = TRUE;


							if (c_f11[i] > 2 * PI)
								c_f11[i] -= 2 * PI, whil = TRUE;

							if (c_f12[i] > 2 * PI)
								c_f12[i] -= 2 * PI, whil = TRUE;


							if (c_f13[i] > 2 * PI)
								c_f13[i] -= 2 * PI, whil = TRUE;
						}
					}
				}
				else
				{
					if (ImGui::Button("rad"))
					{
						cam_rad[i] = TRUE;
						c_f11[i] = c_f12[i] = c_f13[i] = 0.f;
						rotation_cam[i] = glm::vec3(0, 0, 0);
					}
					else
					{
						ImGui::SliderFloat("rotation x degrees", &c_f11[i], 0.0f, 360.0f);
						d1 = (c_f11[i]-a[0]) * (PI / 180.0);
						ImGui::SliderFloat("rotation y degrees", &c_f12[i], 0.0f, 360.0f);
						d2 =(c_f12[i]-a[1]) * (PI / 180.0);
						ImGui::SliderFloat("rotation z degrees", &c_f13[i], 0.0f, 360.0f);
						d3 = (c_f13[i]-a[2]) * (PI / 180.0);
						ratio = PI / 180.0;
						whil = true;
						while (whil)
						{
							whil = FALSE;
							if (c_f11[i] < 0.f)
								c_f11[i] += 360.f, whil = TRUE;

							if (c_f12[i] < 0.f)
								c_f12[i] += 360.f, whil = TRUE;

							if (c_f13[i] < 0.f)
								c_f13[i] += 360.f, whil = TRUE;

							if (c_f11[i] > 360.f)
								c_f11[i] -= 360.f, whil = TRUE;

							if (c_f12[i] > 360.f)
								c_f12[i] -= 360.f, whil = TRUE;


							if (c_f13[i] > 360.f)
								c_f13[i] -= 360.f, whil = TRUE;

						}
					}
				}

				ImGui::Checkbox("rotate in place", &in_place1[i]);
				glm::mat4x4 mat =
					cam->GetrotationTransform(d1, 0)*
					cam->GetrotationTransform(d2, 1)*
					cam->GetrotationTransform(d3, 2);

				rotation_cam[i] = glm::vec3(c_f11[i], c_f12[i], c_f13[i]);

				//do the transformation!
				if (in_place1[i])
				{
					glm::mat4x4 moveToCenter = cam->GetTranslateTransform(
						-cam->pos.x, -cam->pos.y, -cam->pos.z);
					cam->update_transform(glm::inverse(moveToCenter) * mat * moveToCenter);

					cam->up = mat * cam->up; //update "up", he's already at the center
				}
				else
				{
					cam->update_transform(mat);
					cam->pos = mat * cam->pos; //update center
					cam->up = mat * cam->up; //update "up", he's already at the center
				}
				
				//camBox
				if (in_place1[i])
				{
					cam->getCamBox()->transformModel(mat); //model
					cam->update_camModelTransform(mat);
				}
					
				else
				{
					cam->getCamBox()->transformWorld(mat); //world
					cam->update_camWorldTransform(mat);
				}
					

				

				//look-at update
				if (cam_look_at[i]) //if he should look on the active model
				{
					const glm::vec4 atModel = glm::vec4(
						zero[scene->ActiveModel].x, zero[scene->ActiveModel].y, zero[scene->ActiveModel].z, 1.0f);
					cam->set_camWorldTransform(cam->LookAt(cam->pos, atModel, cam->up));
					glm::mat4x4 identity
					(
						1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f
					);
					cam->set_camModelTransform(identity);
				}


				//update zero_cam (just like pos)
				if (in_place1[i])
				{
					zero_cam[i] = cam->GetTranslateTransform(zero_cam[i][0], zero_cam[i][1], zero_cam[i][2])
						* mat *
						cam->GetTranslateTransform(-zero_cam[i][0], -zero_cam[i][1], -zero_cam[i][2])*
						glm::vec4(zero_cam[i][0], zero_cam[i][1], zero_cam[i][2], 1);
				}
				else
					zero_cam[i] = mat * glm::vec4(zero_cam[i][0], zero_cam[i][1], zero_cam[i][2], 1);

			}
			ImGui::End();
		}
	}

}
