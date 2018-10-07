
#include "Renderer.h"
#include "InitShader.h"
#include <imgui/imgui.h>
#include <iostream>
#include <cmath>
#include "Camera.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

bool Renderer::get_SuperSampling(){return SuperSampling;}
void Renderer::set_SuperSampling(bool x){SuperSampling=x;}

float Renderer::get_zFar(){return zFar;}
void Renderer::set_zFar(float f) {zFar=f;}

bool Renderer::get_fog() { return fog; }
void Renderer::set_fog(bool x) { fog = x; }

bool Renderer::get_auto_color() { return auto_color; }
void Renderer::set_auto_color(bool x) { auto_color = x; }

int find(vector<glm::vec2> vec, glm::vec2 val)
{
	for (int i = 0; i < vec.size(); i++)
		if (vec[i] == val)
			return i;
	return -1;
}
float twoD_interp(float v0, float v1, float t)
{
	//return (1 - t) * v0 + t * v1;
	return 5;
}
float sign(float f)
{
	if (f < 0.f)
		return -1;
	else
		return 1;
}
glm::vec3 absc(glm::vec3 color)
{
	float x1, y1, z1;
	x1 = fmax(0, color.x);
	y1 = fmax(0, color.y);
	z1 = fmax(0, color.z);
	return glm::vec3(x1,y1,z1);
}
template<class T>
const T& max(const T& a, const T& b)
{
	return (a < b) ? b : a;
}
glm::vec3 negative(glm::vec3 vec)
{
	return glm::vec3(-vec.x, -vec.y, -vec.z);
}
float sum(const glm::vec3 v)
{
	return v.x + v.y + v.z;
}
float norm45(const glm::vec3 v)
{
	float ans = pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2);
	ans = pow(ans, 0.5);
	if (ans > 1.f)
		return 1.f;
	return ans;
}
float norm(glm::vec3 v)
{
		//return pow((abs(v.x) + abs(v.y) + abs(v.z)),0.5);
		v= glm::vec3(fmax(v.x, 0.f) , fmax(v.y, 0.f) , fmax(v.z, 0.f));
		return norm45(v);
}
void Renderer::SetCameraTransform(const glm::mat4x4& cTransform)
{
	myCameraTransform = cTransform;
}
void Renderer::SetProjection(const glm::mat4x4& projection)
{
	myProjection = projection;
}
float Noise(glm::vec2 p)
{
	int x1, y1;
	x1 = p.x;
	y1 = p.y;
	if(x1==p.x&&y1==p.y)
		return rand() /10000;
	else
	{
		float x2= rand() / 10000, y2 = rand() / 10000;
		//twoD_interp(x2,y2,...)
		return (x2 + y2)/0.5;
	}
}
float turbulence(glm::vec2 p,int N)
{
	float t = 0;
	float scale = 1;
	for(int i=0;i<N;i++)
		t += Noise(p / scale)*scale;
	scale /= 2;
	return t;
}
glm::vec3 wood(glm::vec2 p)
{
	float x1 = (p.x + p.y) + turbulence(p,10);
	//return woodcolor(sin(x1));
	return glm::vec3(x1, x1, x1);
	//return glm::vec3(1, 0, 0);
}
Renderer::Renderer() : width(1280), height(720),
myCameraTransform(1.0f), myProjection(1.0f), worldTransform(1.0f), nTransform(1.0f)
{
	initOpenGLRendering();
	createBuffers(1280,720);
	textur_map.clear();
	/*
	vector<glm::vec3> v;
	for (int x = 0; x < 2000; x++)
	{
		v.clear();
		for (int y = 0; y <4000; y++)
			v.push_back(wood(glm::vec2(x, y)));
		textur_map.push_back(v);
	}
	*/
}

Renderer::Renderer(int w, int h) : width(w), height(h),
myCameraTransform(1.0f), myProjection(1.0f), worldTransform(1.0f), nTransform(1.0f)
{
	initOpenGLRendering();
	createBuffers(w,h);
	textur_map.clear();
	/*vector<glm::vec3> v;
	for (int x = 0; x < 2000; x++)
	{
		v.clear();
		for (int y = 0; y < 4000; y++)
			v.push_back(wood(glm::vec2(x, y)));
		textur_map.push_back(v);
	}
	*/
}

Renderer::~Renderer()
{
	delete[] colorBuffer;
}
void Renderer::putPixel_no_check2(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= width) return;
	if (j < 0) return; if (j >= height) return;
	colorBuffer[INDEX(width, i, j, 0)] = color.x;
	colorBuffer[INDEX(width, i, j, 1)] = color.y;
	colorBuffer[INDEX(width, i, j, 2)] = color.z;
}
void Renderer::putPixel2(int i, int j)
{
	if (i < 0) return; if (i >= width) return;
	if (j < 0) return; if (j >= height) return;
	colorBuffer3.push_back(glm::vec2(i, j));
	glm::vec3 color;
	color.x =colorBuffer[INDEX(width, i, j, 0)] ;
	color.y = colorBuffer[INDEX(width, i, j, 1)] ;
	color.z = colorBuffer[INDEX(width, i, j, 2)];
	colorBuffer2.push_back(color);
}
void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= width) return;
	if (j < 0) return; if (j >= height) return;
	colorBuffer[INDEX(width, i, j, 0)] = color.x;
	colorBuffer[INDEX(width, i, j, 1)] = color.y;
	colorBuffer[INDEX(width, i, j, 2)] = color.z;
}
void Renderer::putPixel(int i, int j,int z, const glm::vec3& color)
{
	if (i < 0) return; if (i >= width) return;
	if (j < 0) return; if (j >= height) return;
	if (z >= zBuffer[i][j])
	{
		zBuffer[i][j] = z;
		
			colorBuffer[INDEX(width, i, j, 0)] = color.x;
			colorBuffer[INDEX(width, i, j, 1)] = color.y;
			colorBuffer[INDEX(width, i, j, 2)] = color.z;
	
	}
}



//putpixelBADCOLOR - draws the color (if needed) but returns x-value when hits a given y
//if he doesn't hit a given y, return x=-1
float Renderer::putPixelBADCOLOR(int i, int j, const glm::vec3& color, int GIVENy)
{
	//first check
	if (j == GIVENy)
		return (float)i;
	return -INFINITY;
}



//interpolation by 3 points -- I think?
void Renderer::putPixel(int i, int j, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	const glm::vec3& color)
{
	if (i < 0) return; if (i >= width) return;
	if (j < 0) return; if (j >= height) return;
	float dist1 = abs(i - point1.x) + abs(j - point1.y);
	float dist2 = abs(i - point2.x) + abs(j - point2.y);
	float dist3 = abs(i - point3.x) + abs(j - point3.y);
	float sum = dist1 + dist2 + dist3;
	float point_z = point1.z*(dist1 / sum) + point2.z*(dist2 / sum) + point3.z*(dist3 / sum);
	if (point_z >= zBuffer[i][j])
	{
		zBuffer[i][j] = point_z;
		glm::vec3 color2 = color;

		if (fog)
			color2 = color2 * (point_z/zFar);
		colorBuffer[INDEX(width, i, j, 0)] = color2.x;
		colorBuffer[INDEX(width, i, j, 1)] = color2.y;
		colorBuffer[INDEX(width, i, j, 2)] = color2.z;
	
	}
}


void Renderer::putPixel2(int x1, int y1,glm::vec3 point1, glm::vec3 point2, glm::vec3 point3
	, const glm::vec3& color1,const glm::vec3& color2, const glm::vec3& color3)
{
	if (x1 < 0) return; if (x1 >= width) return;
	if (y1 < 0) return; if (y1 >= height) return;
	float dist1 = abs(x1 - point1.x) +abs(y1 - point1.y);
	float dist2 = abs(x1 - point2.x) + abs(y1 - point2.y);
	float dist3 = abs(x1 - point3.x) + abs(y1 - point3.y);
	float sum = dist1 + dist2 + dist3;
	glm::vec3 color = color1 * (dist1 / sum) + color2 * (dist2 / sum) + color3 * (dist3 / sum);
	float point_z = point1.z*(dist1 / sum) + point2.z*(dist2 / sum) + point3.z*(dist3 / sum);
	if (point_z >= zBuffer[x1][y1])
	{
		zBuffer[x1][y1] = point_z;
		glm::vec3 color2 = color;

		if (fog)
		{
			if (point_z < 0)
			{
				point_z = abs(point_z);
				point_z++;
				color2 = color2 / (point_z / zFar);
			}
			else
			{
				point_z = abs(point_z);
				point_z--;
				color2 = color2 * (point_z / zFar);
			}
		}
			
		colorBuffer[INDEX(width, x1, y1, 0)] = color2.x;
		colorBuffer[INDEX(width, x1, y1, 1)] = color2.y;
		colorBuffer[INDEX(width, x1, y1, 2)] = color2.z;
	}
	
}
void Renderer::putPixel3(int x1, int y1, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	glm::vec3 norm1, glm::vec3 norm2, glm::vec3 norm3,
	float Diffus_st, vector<glm::vec3> diffus,
	vector<glm::vec3> direction, vector<glm::vec3> positions, glm::vec3 am_vec,
	glm::vec3 amcolor, glm::vec3 difcolor, glm::vec3 spectcolor, const glm::vec3 & v_direction, const vector<int> & spect_exp,
	const vector<glm::vec3> & ligth_spect_c, const vector<bool> & types, glm::mat4x4 windowresizing_invers)
{
	if (x1 < 0) return; if (x1 >= width) return;
	if (y1 < 0) return; if (y1 >= height) return;

	float dist1 = abs(x1 - point1.x) + abs(y1 - point1.y);
	float dist2 = abs(x1 - point2.x) + abs(y1 - point2.y);
	float dist3 = abs(x1 - point3.x) + abs(y1 - point3.y);
	float sum1 = dist1 + dist2 + dist3;
	glm::vec3 cur_norm = norm1 * (dist1 / sum1) + norm2 * (dist2 / sum1) + norm3 * (dist3 / sum1);
	float point_z = point1.z*(dist1 / sum1) + point2.z*(dist2 / sum1) + point3.z*(dist3 / sum1);
	if (point_z >= zBuffer[x1][y1])
	{
	float x2 = 0.f;

	glm::vec3  am_color = am_vec * amcolor;
	glm::vec3  dif_color = glm::vec3(0, 0, 0);
	glm::vec3  spect_color = glm::vec3(0, 0, 0);
	glm::vec3 pos_normalized = glm::vec3(x1, y1, point_z);
	pos_normalized = windowresizing_invers *glm::vec4(pos_normalized,1);
	for (int i = 0; i < diffus.size(); i++)
	{
		glm::vec3 dir = glm::normalize(positions[i] - pos_normalized);
		if (types[i])
			dir = direction[i];
		dir = glm::normalize(dir);
		glm::vec3 cur_norm2= glm::normalize(cur_norm);
		if ((norm(dir) == 0.f) && (norm(-dir) != 0.f))
		{
			dir = -dir;
			cur_norm2 = -cur_norm2;
		}
	

		x2 = glm::dot(dir, cur_norm2);
		dif_color = dif_color + absc(diffus[i] * x2*Diffus_st);

		dir = -dir;
		glm::vec3 R = 2 * glm::dot(cur_norm2, dir)* dir - cur_norm2;

		R = glm::normalize(R);
		if (glm::dot(R, dir) < 0)
			spect_color = spect_color + absc(ligth_spect_c[i] * glm::pow(abs(glm::dot(R, glm::normalize(v_direction))), spect_exp[i]));
	}
	dif_color = dif_color * difcolor;
	glm::vec3 cur_color = am_color + dif_color + spect_color;

	zBuffer[x1][y1] = point_z;
	colorBuffer[INDEX(width, x1, y1, 0)] = cur_color.x;
	colorBuffer[INDEX(width, x1, y1, 1)] = cur_color.y;
	colorBuffer[INDEX(width, x1, y1, 2)] = cur_color.z;
	}

}
void Renderer::SetObjectMatrices(const glm::mat4x4& worldTransform, const glm::mat4x4& nTransform)
{
	this->worldTransform = worldTransform;
	this->nTransform = nTransform;
}

void Renderer::DrawTriangles(glm::vec4* vertexPositions, int size,
	const glm::vec3 & amcolor1, const glm::vec3 & difcolor1, const glm::vec3 & spectcolor1,
	float w, float h, glm::mat4x4 windowresizing,
	MeshModel* myModel, Camera* activeCam, const glm::vec3 & am_vec, const vector<glm::vec3> & diffus
	, const vector<glm::vec3> & positions, const vector<glm::vec3> & directions,
	const vector<bool> & ligth_type, const glm::vec3 & v_direction, const vector<int> & spect_exp,
	const vector<glm::vec3> & ligth_spect_c,int type,int count)
{
	//we recieve the object to draw with a vector of verticesPositions
	//we will draw these triangles but first will do the transformations

	/*
	float** a_super = new float*[w];
	if (SuperSampling)
	{
		for (int i = 0; i < w; i++)
		{
			a_super[i] = new float[h * 3];
			if(count==0)
				for (int j = 0; j < h; j += 3)
				{
					a_super[i][j] = back_round_color.x;
					a_super[i][j + 1] = back_round_color.y;
					a_super[i][j + 2] = back_round_color.z;
				}
		}


	}
	*/
	//first do the transformations:
	myCameraTransform = activeCam->get_camWorldTransform() * activeCam->get_camModelTransform();
	//the view matrix
	glm::mat4x4 view = worldTransform* glm::inverse(myCameraTransform); // T = M * C^-1

	//for
	//texture
	//we
	//can
	//set all the colors to (1,1,1), and only in put pixel multiplay them by the texture color
	//now the project transformation:
	glm::vec3 amcolor=amcolor1, difcolor=difcolor1, spectcolor=spectcolor1;
	/*
	if (auto_color)
	{
		amcolor = glm::vec3(0, 0, 1);
		difcolor = glm::vec3(0, 1, 0);
		spectcolor = glm::vec3(1, 0, 0);
	}
	*/
	glm::mat4x4 T = myProjection * view; //first transform on the 3d world, then projet it


	glm::vec4* transVerticesPositions = new glm::vec4[size];
	glm::vec2* drawVertexPositions = new glm::vec2[size];


	for (int i = 0; i < size; i++)
	{
		//first transform all the points (including projection)
		transVerticesPositions[i] = T * vertexPositions[i];
		transVerticesPositions[i] = transVerticesPositions[i]
			/ transVerticesPositions[i].w; //normallize them

		//now the points are NDC. normalized Device Coordinates
		//now do window coordinates transformation
		transVerticesPositions[i] = windowresizing * transVerticesPositions[i];

	}

	//now draw the triangles (and always before put them in vec2) !!!
	glm::vec3 a(0.0f, 0.0f, 0.0f), b(0.0f, 0.0f, 0.0f), c(0.0f, 0.0f, 0.0f);
	for (int face = 0; face < size - 2; face = face + 3)
	{
		/*
		a = sampel_size* transVerticesPositions[face];
		b = sampel_size* transVerticesPositions[face + 1];
		c = sampel_size* transVerticesPositions[face + 2];
		*/
		a =  transVerticesPositions[face];
		b =  transVerticesPositions[face + 1];
		c =  transVerticesPositions[face + 2];

		//draw triangle [a,b,c]
		if (type == 0)//flat
		{

			glm::vec3 avg = (a + b + c) / 3.f;
			avg = glm::inverse(windowresizing) *glm::vec4( avg,1);
			glm::vec3 cface_norm = glm::normalize(myModel->getNormalFace()[face /3]);
			float x6 = 0.f;

			glm::vec3  AMcolor = am_vec * amcolor;
			glm::vec3  Difuscolor = glm::vec3(0, 0, 0);
			glm::vec3  Spectcolor = glm::vec3(0, 0, 0);
			glm::vec3 R, v = glm::normalize(v_direction);
			
			for (int i = 0; i < diffus.size(); i++)
			{

				
				glm::vec3 dir =  
				dir= positions[i]- avg;
				//dir = absc(dir);
				
				
				if (ligth_type[i])
				{
					//dir = glm::inverse(myProjection)*glm::vec4(directions[i], 1);
					
					dir = glm::vec4(directions[i], 1);
					
				}

				glm::vec3 face_norm = cface_norm;
				face_norm = glm::normalize(face_norm);
				dir = glm::normalize(dir);
				if ((norm(dir) == 0.f) && (norm(-dir) != 0.f))
				{
					face_norm = -face_norm;
					dir = -dir;
				}

				x6 = glm::dot(dir, face_norm);
				Difuscolor = Difuscolor + absc(diffus[i] * x6* myModel->Diffus);


				dir = -dir;
				R = 2 * glm::dot(face_norm, dir)* dir - face_norm;

				R = glm::normalize(R);

				if (glm::dot(R, dir) < 0)
					Spectcolor = Spectcolor + absc(ligth_spect_c[i] * glm::pow(abs(glm::dot(R, v)), spect_exp[i]));

			}

				
			glm::vec3 color = AMcolor + (Difuscolor*difcolor) + (Spectcolor*spectcolor);


			//drawTringleFlat(a, b, c, color, w, h);
			drawTringleGouraud(a, b, c, color, color, color, w, h);
			
		}
		if (type == 1)// Gouraud 
		{
			glm::vec4 cv1 = glm::normalize(myModel->getNormalVertex2()[face]);
			glm::vec4 cv2 = glm::normalize(myModel->getNormalVertex2()[face+1]);
			glm::vec4 cv3 = glm::normalize(myModel->getNormalVertex2()[face+2]);

			float x1 = 0.f;
			float x2 = 0.f;
			float x3 = 0.f;
			glm::vec3 a1, b1, c1;

			a1 = glm::inverse(myCameraTransform) *glm::inverse(windowresizing)*glm::vec4(a,1);
			b1 = glm::inverse(myCameraTransform) *glm::inverse(windowresizing)*glm::vec4(b, 1);
			c1 = glm::inverse(myCameraTransform) *glm::inverse(windowresizing)*glm::vec4(c, 1);

			glm::vec3  AMcolor1 = am_vec * amcolor;
			glm::vec3  Difuscolor1 = glm::vec3(0, 0, 0);
			glm::vec3  Spectcolor1 = glm::vec3(0, 0, 0);
			glm::vec3  AMcolor2 = am_vec * amcolor;
			glm::vec3  Difuscolor2 = glm::vec3(0, 0, 0);
			glm::vec3  Spectcolor2 = glm::vec3(0, 0, 0);
			glm::vec3  AMcolor3 = am_vec * amcolor;
			glm::vec3  Difuscolor3 = glm::vec3(0, 0, 0);
			glm::vec3  Spectcolor3 = glm::vec3(0, 0, 0);
			glm::vec3 R1,R2,R3,v = glm::normalize(v_direction);
			for (int i = 0; i < diffus.size(); i++)
			{
				glm::vec3 v1 = cv1, v2 = cv2, v3 = cv3;
				glm::vec3 cur_d1 = glm::normalize(positions[i] - a1);
				glm::vec3 cur_d2 = glm::normalize(positions[i] - b1);
				glm::vec3 cur_d3 = glm::normalize(positions[i] - c1);
				if (ligth_type[i])
				{
					cur_d1 = directions[i];
					cur_d2 = directions[i];
					cur_d3 = directions[i];
				}
				if ((norm(cur_d1) == 0.f) && (norm(-cur_d1) != 0.f))
				{
					v1 = -v1;
					cur_d1 = -cur_d1;
				}
				if ((norm(cur_d2) == 0.f) && (norm(-cur_d2) != 0.f))
				{
					v2 = -v2;
					cur_d2 = -cur_d2;
				}
				if ((norm(cur_d3) == 0.f) && (norm(-cur_d3) != 0.f))
				{
					v3 = -v3;
					cur_d3 = -cur_d3;
				}
				cur_d1 = glm::normalize(cur_d1);
				cur_d2 = glm::normalize(cur_d2);
				cur_d3 = glm::normalize(cur_d3);

				x1 = glm::dot(cur_d1, v1);
				x2 = glm::dot(cur_d2, v2);
				x3 = glm::dot(cur_d3, v3);


				Difuscolor1 = Difuscolor1 + absc((diffus[i] * x1* myModel->Diffus));
				Difuscolor2 = Difuscolor2 + absc((diffus[i] * x2* myModel->Diffus));
				Difuscolor3 = Difuscolor3 + absc((diffus[i] * x3* myModel->Diffus));


				cur_d1 = -cur_d1;
				cur_d2 = -cur_d2;
				cur_d3 = -cur_d3;


				R1 = 2 * glm::dot(cur_d1, v1)*cur_d1 - v1;
				R2 = 2 * glm::dot(cur_d2, v2)* cur_d2 - v2;
				R3 = 2 * glm::dot(cur_d3, v3)*  cur_d3 - v3;

				R1 = glm::normalize(R1);
				R2 = glm::normalize(R2);
				R3 = glm::normalize(R3);
	
				if (glm::dot(R1, cur_d1) < 0)
						Spectcolor1 = Spectcolor1 + absc(ligth_spect_c[i] * glm::pow(abs(glm::dot(R1, v)), spect_exp[i]));

				if (glm::dot(R2, cur_d2) < 0)
					Spectcolor2 = Spectcolor2 + absc(ligth_spect_c[i] * glm::pow(abs(glm::dot(R2, v)), spect_exp[i]));

				if (glm::dot(R3, cur_d3) < 0)
					Spectcolor3 = Spectcolor3 + absc(ligth_spect_c[i] * glm::pow(abs(glm::dot(R3, v)), spect_exp[i]));
				
			}
			glm::vec3 color1 = AMcolor1 + (Difuscolor1 * difcolor) + (Spectcolor1 * spectcolor);
			glm::vec3 color2 = AMcolor2 + (Difuscolor2 * difcolor) + (Spectcolor2 * spectcolor);
			glm::vec3 color3 = AMcolor3 + (Difuscolor3 * difcolor) + (Spectcolor3 * spectcolor);
			drawTringleGouraud(a, b, c, color1, color2, color3, w, h);
		}
		//if (type == 2 && myModel->willDrawVertexNormal == 1)
		if (type == 2)//Phong
		{
			glm::vec4 v1 = myModel->getNormalVertex2()[face];
			glm::vec4 v2 = myModel->getNormalVertex2()[face + 1];
			glm::vec4 v3 = myModel->getNormalVertex2()[face + 2];
			drawTringlePhong(a, b, c, v1,v2,v3, myModel->Diffus,diffus,directions,positions, am_vec,
				amcolor,difcolor, spectcolor,  v_direction, spect_exp, ligth_spect_c, ligth_type,
				glm::inverse(windowresizing), w, h);
		}

		
	}
	//also, draw the Bounding box, if needed
	if (myModel->willDrawBox == 1)
	{

		/*

	    a'_______b'
      d'/|_____c'/|
		||      | |
		||a_____|_|b
		|/______|/
		d       c

		*/



		glm::vec4* bounds = new glm::vec4[8];
		const glm::vec4* vP = myModel->GetVertex(); //get points even before model's transform
		//a,b,c,d
		bounds[0] = glm::vec4(vP[myModel->xMin].x, vP[myModel->yMin].y, vP[myModel->zMin].z, 1.0f); //a
		bounds[1] = glm::vec4(vP[myModel->xMax].x, vP[myModel->yMin].y, vP[myModel->zMin].z, 1.0f); //b
		bounds[2] = glm::vec4(vP[myModel->xMax].x, vP[myModel->yMin].y, vP[myModel->zMax].z, 1.0f); //c
		bounds[3] = glm::vec4(vP[myModel->xMin].x, vP[myModel->yMin].y, vP[myModel->zMax].z, 1.0f); //d
		//a',b',c',d' - exacly the same, just yMin <=> yMax
		bounds[4] = glm::vec4(vP[myModel->xMin].x, vP[myModel->yMax].y, vP[myModel->zMin].z, 1.0f); //a'
		bounds[5] = glm::vec4(vP[myModel->xMax].x, vP[myModel->yMax].y, vP[myModel->zMin].z, 1.0f); //b'
		bounds[6] = glm::vec4(vP[myModel->xMax].x, vP[myModel->yMax].y, vP[myModel->zMax].z, 1.0f); //c'
		bounds[7] = glm::vec4(vP[myModel->xMin].x, vP[myModel->yMax].y, vP[myModel->zMax].z, 1.0f); //d'

		//do model's trans
		for (int i = 0; i < 8; i++)
		{
			bounds[i] = myModel->getModelTransform() * bounds[i];
		}
		//do the same transformations just like on the original points
		for (int i = 0; i < 8; i++)
		{
			bounds[i] = T * bounds[i];
			bounds[i] = bounds[i] / bounds[i].w; //normallize them

			//now the points are NDC. normalized Device Coordinates
			//now do window coordinates transformation
			bounds[i] = windowresizing * bounds[i];
		}

		//now draw them
		{
			//draw a-b-c-d:
			for (int i = 0; i < 4; i++)
			{
				drawLine(bounds[(i) % 4], bounds[(i + 1) % 4], amcolor);
			}
			//draw a'-b'-c'-d'
			for (int i = 4; i < 8; i++)
			{
				drawLine(bounds[i % 4 + 4], bounds[(i + 1) % 4 + 4], amcolor);
			}
			//draw a-a' b-b' c-c' d-d'
			for (int i = 0; i < 4; i++)
			{
				drawLine(bounds[i], bounds[i + 4], amcolor);
			}
		}
		delete[] bounds;
	}
	

	glm::mat4x4 model;
	glm::mat4x4 normalMatrix;
	colorBuffer2.clear();
	colorBuffer3.clear();
	/*
	if(SuperSampling)
		for(int i=0;i<h;i++)
			for (int j = 0; j < w; j++)
			{
				glm::vec3 sum_color = glm::vec3(0, 0, 0);
				glm::vec3 cur_color;
				long count = 0;
				int startw=i*sampel_size, endw= (i+1) * sampel_size,
					starth = j * sampel_size, endh = (j + 1) * sampel_size;
				for(int placew=startw;placew<endw-1; placew++)
					for (int placeh = starth; placeh < endh-1; placeh++)
					{
						sum_color+= glm::vec3(colorBuffer[INDEX(width, placew, placeh, 1)],
							colorBuffer[INDEX(width, placew, placeh, 1)]
							, colorBuffer[INDEX(width, placew, placeh, 1)]);
						count++;
						
					}
				putPixel(i, j, sum_color / float(count));
			}*/
	//also, draw vertices' normals, if needed
	if (myModel->willDrawVertexNormal == 1)
	{
		//calculate Model-View matrix
		model = myModel->getModelTransform();
		glm::mat4x4 mv = view * model;
		normalMatrix = glm::transpose(glm::inverse(mv)); // (M^-1)^T

		normalMatrix = myProjection * normalMatrix; //and projet them as usual

		glm::vec4* normalPositions = myModel->getNormalVertex();

		glm::vec4 *transNormalPositions = new glm::vec4[size];

		//will help track the movement of the normals
		glm::vec4 trackMovement = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //(0,0,0,1)

		for (int i = 0; i < size; i++)
		{
			//first copy the original. don't destory them
			transNormalPositions[i] = normalPositions[i];
			transNormalPositions[i].w = 1; //the normals shouldn't move, but we don't want to lose "w trans"
		}

		for (int i = 0; i < size; i++)
		{
			transNormalPositions[i] = normalMatrix * transNormalPositions[i]; //trans them with w=0?

			//divide by w
			transNormalPositions[i] = transNormalPositions[i] / transNormalPositions[i].w;

			transNormalPositions[i] = windowresizing * transNormalPositions[i];
		}

		//track movement
		trackMovement = normalMatrix * trackMovement;
		trackMovement = trackMovement / trackMovement.w;
		trackMovement = windowresizing * trackMovement;

		//now draw each normal vertex. from vertex to the normal point
		float sizeNormals;
		for (int i = 0; i < size; i++)
		{
			//point
			a.x = transVerticesPositions[i].x;
			a.y = transVerticesPositions[i].y;
			//normal
			b.x = transNormalPositions[i].x;
			b.y = transNormalPositions[i].y;

			//return to the origin (only direction)
			b.x = b.x - trackMovement.x;
			b.y = b.y - trackMovement.y;

			//normalize them? -- let them be the size of 40
			if(!(b.x == 0.f && b.y == 0.f))
			b = glm::normalize(b);

			sizeNormals = 40;

			b.x = sizeNormals * b.x;
			b.y = sizeNormals * b.y;

			//let him start from the point he's normal to
			b.x = b.x + a.x;
			b.y = b.y + a.y;

			drawLine_z(a, b, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)); //faces' normals with GREEN
		}



		delete[] transNormalPositions;
		//delete[] normalPositions;
	}


	//also, draw faces' normals, if needed
	if (myModel->willDrawFaceNormal == 1)
	{
		//calculate Model-View matrix
		glm::mat4x4 model = myModel->getModelTransform();
		glm::mat4x4 mv = view * model;
		glm::mat4x4 normalMatrix = glm::transpose(glm::inverse(mv)); // (M^-1)^T

		normalMatrix = myProjection * normalMatrix; //and projet them as usual


		glm::vec4 *facesNormal = myModel->getNormalFace();
		glm::vec4 *facesAvg = myModel->getFaceAvgs();
		// #faces = #points / 3
		glm::vec4 *transFaces = new glm::vec4[size / 3];
		glm::vec4 *transAvg = new glm::vec4[size / 3];


		//will help track the movement of the normals
		glm::vec4 trackMovement = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); //(0,0,0,1)

		//copy them
		for (int f = 0; f < size / 3; f++)
		{
			transFaces[f] = facesNormal[f];
			transFaces[f].w = 1.0f; //the normals shouldn't move, but we don't want to lose "w trans"

			transAvg[f] = facesAvg[f];
			int kula = 0;
		}

		//transform them AVGs & normals

		glm::mat4x4 regularTrans = T * model;

		for (int f = 0; f < size / 3; f++)
		{
			transFaces[f] = normalMatrix * transFaces[f]; //trans them with w=0?
			transFaces[f] = transFaces[f] / transFaces[f].w; //divide by w
			transFaces[f] = windowresizing * transFaces[f];

			//regular points, regular transformation
			transAvg[f] = regularTrans * transAvg[f]; 
			transAvg[f] = transAvg[f] / transAvg[f].w; //divide by w
			transAvg[f] = windowresizing * transAvg[f];
		}

		//track movement
		trackMovement = normalMatrix * trackMovement;
		trackMovement = trackMovement / trackMovement.w;
		trackMovement = windowresizing * trackMovement;


		//now draw each normal vertex. from vertex to the normal point
		glm::vec3 a = glm::vec3(), b = glm::vec3();
		float sizeNormals;
		for (int f = 0; f < size / 3; f++)
		{
			//point - face's avg
			a.x = transAvg[f].x;
			a.y = transAvg[f].y;

			//normal
			b.x = transFaces[f].x;
			b.y = transFaces[f].y;

			//return to the origin (only direction)
			b.x = b.x - trackMovement.x;
			b.y = b.y - trackMovement.y;

			
			//normalize them? -- let them be the size of 40
			if(b.x != 0.0f || b.y != 0.0f)
				b = glm::normalize(b);
			sizeNormals = 40;
			b.x = sizeNormals * b.x;
			b.y = sizeNormals * b.y;
			

			//let him start from the point he's normal to
			b.x = b.x + a.x;
			b.y = b.y + a.y;

			drawLine_z(a, b, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)); //faces' normals with WHITE
			int checking = 0;
		}
		

		delete[] transAvg;
		delete[] transFaces;
	}
	int place;
	glm::vec3 colorofpoint;
	float fog_val;
	delete[] transVerticesPositions; //they take a lot of memory and will not be used again
	delete[] drawVertexPositions;
	delete[] vertexPositions;

}

void Renderer::createBuffers(int w, int h)
{
	createOpenGLBuffer(); //Do not remove this line.
	colorBuffer = new float[3*w*h];
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			putPixel(i, j, glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}
}

bool Renderer::shouldntFill(glm::vec2 a, glm::vec2 b, glm::vec2 c)
{
	float degA, degB, degC;
	glm::vec2 AB = b - a, BC = c - b, AC = c - a;
	float ab = glm::distance(a, b);
	float bc = glm::distance(b, c);
	float ac = glm::distance(a, c);
	//theta = acos((v1 DOT v2) / (| v1 | *| v2 | )) ---in radians

	if (ab == 0.0f || bc == 0.0f || ac == 0.0f)
		return true;

	degA = acos(glm::dot(AB, AC) / (ab * ac));
	degB = acos(glm::dot(-AB, BC) / (ab * bc));
	degC = acos(glm::dot(BC, AC) / (bc * ac));
	
	degA = glm::degrees(degA);
	degB = glm::degrees(degB);
	degC = glm::degrees(degC);

	if (degA < 13.0f || degB < 13.0f || degC < 13.0f)
		return true;
	return false;

}

//draw Triangle flat-shading  --- one color for a mesh
void Renderer::drawTringleFlat(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	const glm::vec3&  color, float w, float h)
{
	if (w < 3 || h < 3)
		return;
	int ymax =  fmax(fmax(point1.y, point2.y), point3.y);
	int ymin =  fmin(fmin(point1.y, point2.y), point3.y);
	int xmax = fmax(fmax(point1.x, point2.x), point3.x);
	int xmin =  fmin(fmin(point1.x, point2.x), point3.x);

	//cout << (color-bad_color).x<<" "<< (color - bad_color).y<<" "<< (color - bad_color).z<<'\n';
	
	while (color == bad_color)
	{
		float a, b, c;
		a = rand() % 128, b = rand() % 128, c = rand() % 128;
		a = a / 128.f, b = b / 128.f,c = c / 128.f;
		bad_color = glm::vec3(a, b, c);
	}
	colorBuffer2.clear();
	colorBuffer3.clear();

	
	drawLine_z(point1, point2, bad_color);
	drawLine_z(point1, point3, bad_color);
	drawLine_z(point2, point3, bad_color);


	


	//scan algo' -- scan all y's

	bool put = FALSE;
	//bool cont = TRUE;
	for (int y = ymin; y < ymax; y++)
	{
		put = FALSE;
		int count = 0;
		int countFAKE = 0;
		
		//count how much seperate intersections with edges there are
		for (int x = xmin; x <= xmax; x++) //used, not just for fun very importent
		{
			if ((y<int(h)) && (y > 0) && (x<int(w)) && (x > 0))
			{
				if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
					bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
					bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
				{
					while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
						bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
						bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
						x++;
					x--;
					countFAKE++;
				}
			}
		}

		//tactic 1 - do draw bad_color but works even when out of bounds.
		//i.e., same drawline func but returns TRUE if it hits a given y.
		//"drawLineBADCOLOR" - drawLine but calls special putpixel -- returns {xmin,xmax}
		//"putpixelBADCOLOR" - draws the color (if needed) but returns x-value when hits a given y
		
		//now need to check if there are 2 different areas of hits'
		//we can check it by looking for different edges hits who are distinct (don't share common x-values)
		glm::vec2 edge1HITS = drawLineBADCOLOR(point1, point2, bad_color, y);
		glm::vec2 edge2HITS = drawLineBADCOLOR(point2, point3, bad_color, y);
		glm::vec2 edge3HITS = drawLineBADCOLOR(point1, point3, bad_color, y);

		bool shouldFillGaps = false;

		//if they're seperated, go fill that gap!
		if (edge1HITS.x != -INFINITY && edge1HITS.y != -INFINITY && edge2HITS.x != -INFINITY && edge2HITS.y != -INFINITY &&
			edge1HITS.x != INFINITY && edge1HITS.y != INFINITY && edge2HITS.x != INFINITY && edge2HITS.y != INFINITY)
		{
			if (edge2HITS.x - edge1HITS.y > 1)
				if (!(edge3HITS.x - 1 <= edge1HITS.y && edge3HITS.y + 1 >= edge2HITS.x))
					shouldFillGaps = true;
				
			if (edge1HITS.x - edge2HITS.y > 1)
				if (!(edge3HITS.x - 1 <= edge2HITS.y && edge3HITS.y + 1 >= edge1HITS.x))
					shouldFillGaps = true;
		}
		
		//if they're seperated, go fill that gap!
		if (edge2HITS.x != -INFINITY && edge2HITS.y != -INFINITY && edge3HITS.y != -INFINITY && edge3HITS.x != -INFINITY &&
			edge2HITS.x != INFINITY && edge2HITS.y != INFINITY && edge3HITS.y != INFINITY && edge3HITS.x != INFINITY)
		{
			if (edge3HITS.x - edge2HITS.y > 1) //min-max>1?
				if (!(edge1HITS.x - 1 <= edge2HITS.y && edge1HITS.y + 1 >= edge3HITS.x))
					shouldFillGaps = true;
			if (edge2HITS.x - edge3HITS.y > 1)
				if (!(edge1HITS.x - 1 <= edge3HITS.y && edge1HITS.y + 1 >= edge2HITS.x))
					shouldFillGaps = true;
		}

		//if they're seperated, go fill that gap!
		if (edge1HITS.x != -INFINITY && edge1HITS.y != -INFINITY && edge3HITS.x != -INFINITY && edge3HITS.y != -INFINITY &&
			edge1HITS.x != INFINITY && edge1HITS.y != INFINITY && edge3HITS.x != INFINITY && edge3HITS.y != INFINITY)
		{
			if (edge1HITS.x - edge3HITS.y > 1)
				if (!(edge2HITS.x - 1 <= edge3HITS.y && edge2HITS.y + 1 >= edge1HITS.x))
					shouldFillGaps = true;
			if (edge3HITS.x - edge1HITS.y > 1)
				if (!(edge2HITS.x - 1 <= edge1HITS.y && edge2HITS.y + 1>= edge3HITS.x))
					shouldFillGaps = true;
		}


		put = FALSE;
		//cont = TRUE;
		//only if there's two intersections with the edges so you should draw.
		if (shouldFillGaps == true)
		{
			if(xmin > 0)
				//scan left to right
				for (int x = xmin; (x <= xmax); x++)
				{
					if ((y<int(h)) && (y > 0) && (x<int(w)) && (x > 0))
					{
						if (put) //now we need to draw
						{

							if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
								bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
								bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
							{

								//first draw the rest of the edge, and then stop drawing
								while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									putPixel(x, y, point1, point2, point3, color);
									x++;
								}
								x--;
								put = FALSE;
								//cont = FALSE; //this is the second edge. no need to continue
							}
							putPixel(x, y, point1, point2, point3, color);
						}
						else
						{
							//if we saw another edge. start drawing.
							//but don't forget the edge can be several pixels long.
							if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
								bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
								bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
							{
								put = TRUE;
								putPixel(x, y, point1, point2, point3, color);
								x++;
								//draw the whole edge, and only afterwards start looking for the next one
								while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									putPixel(x, y, point1, point2, point3, color);
									x++;
								}
								x--;
							}

						}
					}
				}
			else
			{
				//scan right to left
				for (int x = fmin(xmax,int(w)-1); x >= 0; x--)
				{
					if (y < int(h) && y >0)
						if ((x >= 0) && (x< int(w)))
						{
							if (put) //now we need to draw
							{

								if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{

									//first draw the rest of the edge, and then stop drawing
									while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
										bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
										bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
									{
										putPixel(x, y, point1, point2, point3, color);
										x--;
									}
									x++;
									put = FALSE;
									//cont = FALSE; //this is the second edge. no need to continue
								}
								else
									putPixel(x, y, point1, point2, point3, color);
							}
							else
							{
								//if we saw another edge. start drawing.
								//but don't forget the edge can be several pixels long.
								if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									put = TRUE;
									putPixel(x, y, point1, point2, point3, color);
									x--;
									//draw the whole edge, and only afterwards start looking for the next one
									while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
										bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
										bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
									{
										putPixel(x, y, point1, point2, point3, color);
										x--;
									}
									x++;
								}

							}
						}
				}
			}
		}
		
		
	}
	

	glm::vec2 cur_point;
	glm::vec3 cur_color;
	for(int i=0;i<colorBuffer2.size();i++)
	{
		cur_color = colorBuffer2[i];
		cur_point = colorBuffer3[i];
		if (!((cur_color == bad_color) || (cur_color == back_round_color)))
		{
			putPixel(cur_point.x, cur_point.y, color);
		}
	}

	drawLine(point1, point2,  color);
	drawLine(point1, point3, color);
	drawLine(point2, point3, color);

}
void Renderer::drawTringleGouraud(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	const glm::vec3&  color1, const glm::vec3&  color2, const glm::vec3&  color3, float w, float h)
{
	if (w < 3 || h < 3)
		return;
	int ymax = fmax(fmax(point1.y, point2.y), point3.y);
	int ymin = fmin(fmin(point1.y, point2.y), point3.y);
	int xmax = fmax(fmax(point1.x, point2.x), point3.x);
	int xmin = fmin(fmin(point1.x, point2.x), point3.x);

	//cout << (color-bad_color).x<<" "<< (color - bad_color).y<<" "<< (color - bad_color).z<<'\n';

	colorBuffer2.clear();
	colorBuffer3.clear();
	drawLine_z(point1, point2, bad_color);
	drawLine_z(point1, point3, bad_color);
	drawLine_z(point2, point3, bad_color);



	//scan algo' -- scan all y's

	bool put = FALSE;
	//bool cont = TRUE;
	for (int y = ymin; y < ymax; y++)
	{
		put = FALSE;
		int count = 0;
		int countFAKE = 0;

		//count how much seperate intersections with edges there are
		for (int x = xmin; x <= xmax; x++) //used, not just for fun very importent
		{
			if ((y<int(h)) && (y > 0) && (x<int(w)) && (x > 0))
			{
				if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
					bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
					bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
				{
					while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
						bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
						bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
						x++;
					x--;
					countFAKE++;
				}
			}
		}

		//tactic 1 - do draw bad_color but works even when out of bounds.
		//i.e., same drawline func but returns TRUE if it hits a given y.
		//"drawLineBADCOLOR" - drawLine but calls special putpixel -- returns {xmin,xmax}
		//"putpixelBADCOLOR" - draws the color (if needed) but returns x-value when hits a given y

		//now need to check if there are 2 different areas of hits'
		//we can check it by looking for different edges hits who are distinct (don't share common x-values)
		glm::vec2 edge1HITS = drawLineBADCOLOR(point1, point2, bad_color, y);
		glm::vec2 edge2HITS = drawLineBADCOLOR(point2, point3, bad_color, y);
		glm::vec2 edge3HITS = drawLineBADCOLOR(point1, point3, bad_color, y);

		bool shouldFillGaps = false;

		//if they're seperated, go fill that gap!
		if (edge1HITS.x != -INFINITY && edge1HITS.y != -INFINITY && edge2HITS.x != -INFINITY && edge2HITS.y != -INFINITY &&
			edge1HITS.x != INFINITY && edge1HITS.y != INFINITY && edge2HITS.x != INFINITY && edge2HITS.y != INFINITY)
		{
			if (edge2HITS.x - edge1HITS.y > 1)
				if (!(edge3HITS.x - 1 <= edge1HITS.y && edge3HITS.y + 1 >= edge2HITS.x))
					shouldFillGaps = true;

			if (edge1HITS.x - edge2HITS.y > 1)
				if (!(edge3HITS.x - 1 <= edge2HITS.y && edge3HITS.y + 1 >= edge1HITS.x))
					shouldFillGaps = true;
		}

		//if they're seperated, go fill that gap!
		if (edge2HITS.x != -INFINITY && edge2HITS.y != -INFINITY && edge3HITS.y != -INFINITY && edge3HITS.x != -INFINITY &&
			edge2HITS.x != INFINITY && edge2HITS.y != INFINITY && edge3HITS.y != INFINITY && edge3HITS.x != INFINITY)
		{
			if (edge3HITS.x - edge2HITS.y > 1) //min-max>1?
				if (!(edge1HITS.x - 1 <= edge2HITS.y && edge1HITS.y + 1 >= edge3HITS.x))
					shouldFillGaps = true;
			if (edge2HITS.x - edge3HITS.y > 1)
				if (!(edge1HITS.x - 1 <= edge3HITS.y && edge1HITS.y + 1 >= edge2HITS.x))
					shouldFillGaps = true;
		}

		//if they're seperated, go fill that gap!
		if (edge1HITS.x != -INFINITY && edge1HITS.y != -INFINITY && edge3HITS.x != -INFINITY && edge3HITS.y != -INFINITY &&
			edge1HITS.x != INFINITY && edge1HITS.y != INFINITY && edge3HITS.x != INFINITY && edge3HITS.y != INFINITY)
		{
			if (edge1HITS.x - edge3HITS.y > 1)
				if (!(edge2HITS.x - 1 <= edge3HITS.y && edge2HITS.y + 1 >= edge1HITS.x))
					shouldFillGaps = true;
			if (edge3HITS.x - edge1HITS.y > 1)
				if (!(edge2HITS.x - 1 <= edge1HITS.y && edge2HITS.y + 1 >= edge3HITS.x))
					shouldFillGaps = true;
		}

		

		put = FALSE;
		//cont = TRUE;
		//only if there's two intersections with the edges so you should draw.
		if (shouldFillGaps == true)
		{
			if (xmin > 0)
				//scan left to right
				for (int x = xmin; (x <= xmax); x++)
				{
					if ((y<int(h)) && (y > 0) && (x<int(w)) && (x > 0))
					{
						if (put) //now we need to draw
						{

							if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
								bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
								bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
							{

								//first draw the rest of the edge, and then stop drawing
								while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									putPixel2(x, y, point1, point2, point3, color1, color2, color3);
									x++;
								}
								x--;
								put = FALSE;
								//cont = FALSE; //this is the second edge. no need to continue
							}
							putPixel2(x, y, point1, point2, point3, color1, color2, color3);
						}
						else
						{
							//if we saw another edge. start drawing.
							//but don't forget the edge can be several pixels long.
							if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
								bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
								bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
							{
								put = TRUE;
								putPixel2(x, y, point1, point2, point3, color1, color2, color3);
								x++;
								//draw the whole edge, and only afterwards start looking for the next one
								while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									putPixel2(x, y, point1, point2, point3, color1, color2, color3);
									x++;
								}
								x--;
							}

						}
					}
				}
			else
			{
				//scan right to left
				for (int x = fmin(xmax, int(w) - 1); x >= 0; x--)
				{
					if (y < int(h) && y >0)
						if ((x >= 0) && (x< int(w)))
						{
							if (put) //now we need to draw
							{

								if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{

									//first draw the rest of the edge, and then stop drawing
									while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
										bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
										bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
									{
										putPixel2(x, y, point1, point2, point3, color1, color2, color3);
										x--;
									}
									x++;
									put = FALSE;
									//cont = FALSE; //this is the second edge. no need to continue
								}
								putPixel2(x, y, point1, point2, point3, color1, color2, color3);
							}
							else
							{
								//if we saw another edge. start drawing.
								//but don't forget the edge can be several pixels long.
								if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									put = TRUE;
									putPixel2(x, y, point1, point2, point3, color1, color2, color3);
									x--;
									//draw the whole edge, and only afterwards start looking for the next one
									while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
										bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
										bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
									{
										putPixel2(x, y, point1, point2, point3, color1, color2, color3);
										x--;
									}
									x++;
								}

							}
						}
				}
			}
		}

	}

	glm::vec2 cur_point;
	glm::vec3 cur_color;
	for (int i = 0; i<colorBuffer2.size(); i++)
	{
		cur_color = colorBuffer2[i];
		cur_point = colorBuffer3[i];
		if ((cur_color == bad_color) || (cur_color == back_round_color))
		{
			
		}
		else
			putPixel(cur_point.x, cur_point.y, cur_color);
	}

	drawLine_ground(point1, point2,
		point1, point2, point3,
		color1, color2, color3);

	drawLine_ground(point2, point3,
		point1, point2, point3,
		color1, color2, color3);

	drawLine_ground(point1, point3,
		point1, point2, point3,
		color1, color2, color3);
	colorBuffer3.clear();
	colorBuffer2.clear();
}

void Renderer::drawTringlePhong(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	const glm::vec3&  norm1, const glm::vec3&  norm2, const glm::vec3&  norm3,
	float Diffus_st, vector<glm::vec3> diffus, vector<glm::vec3> directions , vector<glm::vec3> positions
	, glm::vec3 am_vec, glm::vec3 amcolor, glm::vec3 difcolor, glm::vec3 spectcolor, const glm::vec3 & v_direction, const vector<int> & spect_exp,
	const vector<glm::vec3> & ligth_spect_c,
	const vector<bool> & types, glm::mat4x4 windowresizing_invers,float w, float h)
{
	if (w < 3 || h < 3)
		return;
	int ymax = fmax(fmax(point1.y, point2.y), point3.y);
	int ymin = fmin(fmin(point1.y, point2.y), point3.y);
	int xmax = fmax(fmax(point1.x, point2.x), point3.x);
	int xmin = fmin(fmin(point1.x, point2.x), point3.x);

	//cout << (color-bad_color).x<<" "<< (color - bad_color).y<<" "<< (color - bad_color).z<<'\n';

	

	colorBuffer2.clear();
	colorBuffer3.clear();
	drawLine_z(point1, point2, bad_color);
	drawLine_z(point1, point3, bad_color);
	drawLine_z(point2, point3, bad_color);


	//scan algo' -- scan all y's

	bool put = FALSE;
	//bool cont = TRUE;
	for (int y = ymin; y < ymax; y++)
	{
		put = FALSE;
		int count = 0;
		int countFAKE = 0;

		//count how much seperate intersections with edges there are
		for (int x = xmin; x <= xmax; x++) //used, not just for fun very importent
		{
			if ((y<int(h)) && (y > 0) && (x<int(w)) && (x > 0))
			{
				if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
					bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
					bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
				{
					while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
						bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
						bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
						x++;
					x--;
					countFAKE++;
				}
			}
		}

		//tactic 1 - do draw bad_color but works even when out of bounds.
		//i.e., same drawline func but returns TRUE if it hits a given y.
		//"drawLineBADCOLOR" - drawLine but calls special putpixel -- returns {xmin,xmax}
		//"putpixelBADCOLOR" - draws the color (if needed) but returns x-value when hits a given y

		//now need to check if there are 2 different areas of hits'
		//we can check it by looking for different edges hits who are distinct (don't share common x-values)
		glm::vec2 edge1HITS = drawLineBADCOLOR(point1, point2, bad_color, y);
		glm::vec2 edge2HITS = drawLineBADCOLOR(point2, point3, bad_color, y);
		glm::vec2 edge3HITS = drawLineBADCOLOR(point1, point3, bad_color, y);

		bool shouldFillGaps = false;

		//if they're seperated, go fill that gap!
		if (edge1HITS.x != -INFINITY && edge1HITS.y != -INFINITY && edge2HITS.x != -INFINITY && edge2HITS.y != -INFINITY &&
			edge1HITS.x != INFINITY && edge1HITS.y != INFINITY && edge2HITS.x != INFINITY && edge2HITS.y != INFINITY)
		{
			if (edge2HITS.x - edge1HITS.y > 1)
				if (!(edge3HITS.x - 1 <= edge1HITS.y && edge3HITS.y + 1 >= edge2HITS.x))
					shouldFillGaps = true;

			if (edge1HITS.x - edge2HITS.y > 1)
				if (!(edge3HITS.x - 1 <= edge2HITS.y && edge3HITS.y + 1 >= edge1HITS.x))
					shouldFillGaps = true;
		}

		//if they're seperated, go fill that gap!
		if (edge2HITS.x != -INFINITY && edge2HITS.y != -INFINITY && edge3HITS.y != -INFINITY && edge3HITS.x != -INFINITY &&
			edge2HITS.x != INFINITY && edge2HITS.y != INFINITY && edge3HITS.y != INFINITY && edge3HITS.x != INFINITY)
		{
			if (edge3HITS.x - edge2HITS.y > 1) //min-max>1?
				if (!(edge1HITS.x - 1 <= edge2HITS.y && edge1HITS.y + 1 >= edge3HITS.x))
					shouldFillGaps = true;
			if (edge2HITS.x - edge3HITS.y > 1)
				if (!(edge1HITS.x - 1 <= edge3HITS.y && edge1HITS.y + 1 >= edge2HITS.x))
					shouldFillGaps = true;
		}

		//if they're seperated, go fill that gap!
		if (edge1HITS.x != -INFINITY && edge1HITS.y != -INFINITY && edge3HITS.x != -INFINITY && edge3HITS.y != -INFINITY &&
			edge1HITS.x != INFINITY && edge1HITS.y != INFINITY && edge3HITS.x != INFINITY && edge3HITS.y != INFINITY)
		{
			if (edge1HITS.x - edge3HITS.y > 1)
				if (!(edge2HITS.x - 1 <= edge3HITS.y && edge2HITS.y + 1 >= edge1HITS.x))
					shouldFillGaps = true;
			if (edge3HITS.x - edge1HITS.y > 1)
				if (!(edge2HITS.x - 1 <= edge1HITS.y && edge2HITS.y + 1 >= edge3HITS.x))
					shouldFillGaps = true;
		}


		put = FALSE;
		//cont = TRUE;
		//only if there's two intersections with the edges so you should draw.
		if (shouldFillGaps == true)
		{
			if (xmin > 0)
				//scan left to right
				for (int x = xmin; (x <= xmax); x++)
				{
					if ((y<int(h)) && (y > 0) && (x<int(w)) && (x > 0))
					{
						if (put) //now we need to draw
						{

							if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
								bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
								bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
							{

								//first draw the rest of the edge, and then stop drawing
								while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
										Diffus_st, diffus, directions, positions,
										am_vec, amcolor, difcolor, spectcolor, v_direction,
										spect_exp, ligth_spect_c, types, windowresizing_invers);
									x++;
								}
								x--;
								put = FALSE;
								//cont = FALSE; //this is the second edge. no need to continue
							}
							putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
								Diffus_st, diffus, directions, positions,
								am_vec, amcolor, difcolor, spectcolor, v_direction,
								spect_exp, ligth_spect_c, types, windowresizing_invers);
						}
						else
						{
							//if we saw another edge. start drawing.
							//but don't forget the edge can be several pixels long.
							if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
								bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
								bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
							{
								put = TRUE;
								putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
									Diffus_st, diffus, directions, positions,
									am_vec, amcolor, difcolor, spectcolor, v_direction,
									spect_exp, ligth_spect_c, types, windowresizing_invers);
								x++;
								//draw the whole edge, and only afterwards start looking for the next one
								while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
										Diffus_st, diffus, directions, positions,
										am_vec, amcolor, difcolor, spectcolor, v_direction,
										spect_exp, ligth_spect_c, types, windowresizing_invers);
									x++;
								}
								x--;
							}

						}
					}
				}
			else
			{
				//scan right to left
				for (int x = fmin(xmax, int(w) - 1); x >= 0; x--)
				{
					if (y < int(h) && y >0)
						if ((x >= 0) && (x< int(w)))
						{
							if (put) //now we need to draw
							{

								if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{

									//first draw the rest of the edge, and then stop drawing
									while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
										bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
										bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
									{
										putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
											Diffus_st, diffus, directions, positions,
											am_vec, amcolor, difcolor, spectcolor, v_direction,
											spect_exp, ligth_spect_c, types, windowresizing_invers);
										x--;
									}
									x++;
									put = FALSE;
									//cont = FALSE; //this is the second edge. no need to continue
								}
								putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
									Diffus_st, diffus, directions, positions,
									am_vec, amcolor, difcolor, spectcolor, v_direction,
									spect_exp, ligth_spect_c, types,windowresizing_invers);
							}
							else
							{
								//if we saw another edge. start drawing.
								//but don't forget the edge can be several pixels long.
								if (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
									bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
									bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
								{
									put = TRUE;
									putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
										Diffus_st, diffus, directions, positions,
										am_vec, amcolor, difcolor, spectcolor, v_direction,
										spect_exp, ligth_spect_c, types, windowresizing_invers);
									x--;
									//draw the whole edge, and only afterwards start looking for the next one
									while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
										bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
										bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
									{
										putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
											Diffus_st, diffus, directions, positions,
											am_vec, amcolor, difcolor, spectcolor, v_direction, 
											spect_exp, ligth_spect_c, types, windowresizing_invers);
										x--;
									}
									x++;
								}

							}
						}
				}
			}
		}

	}
	glm::vec2 cur_point;
	glm::vec3 cur_color;
	for (int i = 0; i<colorBuffer2.size(); i++)
	{
		cur_color = colorBuffer2[i];
		cur_point = colorBuffer3[i];
		if ((cur_color == bad_color) || (cur_color == back_round_color))
		{

		}
		else
			putPixel(cur_point.x, cur_point.y, cur_color);
	}
	drawLine_phong(point1, point2,
		point1, point2, point3,
		norm1, norm2, norm3,
		Diffus_st, diffus,directions, positions, am_vec, amcolor, difcolor,
		spectcolor,v_direction, spect_exp, ligth_spect_c, types, windowresizing_invers);

	drawLine_phong(point2, point3,
		point1, point2, point3,
		norm1, norm2, norm3,
		Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor,
		spectcolor, v_direction, spect_exp, ligth_spect_c, types, windowresizing_invers);

	drawLine_phong(point1, point3,
		point1, point2, point3,
		norm1,norm2,norm3,
		Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor,
		spectcolor, v_direction, spect_exp, ligth_spect_c,types, windowresizing_invers);
	colorBuffer3.clear();
	colorBuffer2.clear();
}

//glm::vec3 color = AMcolor + (Difuscolor*difcolor) + (Spectcolor*spectcolor);
//do drawTriangle algo' but chooses pixel color with texture + light (light is given ready to use)
//this algo will work exacly the same for Gouraud & Flat shading because the normal doesn't change
//thus light stays the same, and can be given as paramaters
void Renderer::drawTriangleTexture(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 ambLight, glm::vec3 difLight, glm::vec3 spectLight)
{

}

//like draw line but when we put pixel we reset the z buff
//not finished
void Renderer::drawLine_reset(glm::vec3 point1, glm::vec3 point2, glm::vec3 color)
{int p1 = point1.x, q1 = point1.y; // point1 parameters
int p2 = point2.x, q2 = point2.y; // point2 parameters

int y, x;
float m;
int c;
if (p1 > p2)
{
	int temp = p1;
	p1 = p2;
	p2 = temp;

	temp = q1;
	q1 = q2;
	q2 = temp;


}
int replaced = 0;

//first deal with special cases like p2 - p1 = 0 or q2 - q1 = 0
if (p2 - p1 == 0)
{
	int min = q2 >= q1 ? q1 : q2;
	int max = q2 <= q1 ? q1 : q2;


	for (int h = min; h < max; h++)
	{
		putPixel2(p1, h);
		putPixel(p1, h, color);
	}

	return;
}
if (q2 - q1 == 0)
{
	int min = p2 >= p1 ? p1 : p2;
	int max = p2 <= p1 ? p1 : p2;

	for (int w = min; w < max; w++)
	{
		putPixel2(w, q1);
		putPixel(w, q1, color);

	}

	return;
}





//for measuring distance between the line's y and the approximation's y
int e;
int tmp;

m = float(q2 - q1) / float(p2 - p1);

if (m >= 0.0f) //m>=0
{
	if (m > 1.0f) // if m>1 replace x & y for both points
	{
		//switch(p1,q1)
		tmp = p1;
		p1 = q1;
		q1 = tmp;

		//switch(p2,q2)
		tmp = p2;
		p2 = q2;
		q2 = tmp;

		replaced = 1;
	}

	// y = mx + c
	m = (q2 - q1) / (p2 - p1);
	c = q1 - m * p1;
	x = p1, y = q1, e = -1 * (p2 - p1);


	while (x <= p2)
	{
		//e = m*x*(dp) + c*dp - y*dp - dp; //measuring distance
		if (e > 0)
		{
			y = y + 1;
			e = e - 2 * (p2 - p1);
		}
		if (replaced == 0)
		{
			putPixel2(x, y);
			putPixel(x, y, color);

		}
		else
		{
			putPixel2(y, x);
			putPixel(y, x, color);

		}

		x = x + 1; //for next point
		e = e + 2 * (q2 - q1); //line's y got bigger by m*dp
	}
}


//m < 0 - similar to m>0 but it's less readable if we would try to combine
else
{



	//if m<-1 should swap(x,y) for both points
	// and also swap the two points between themselves
	/*
	1*           2*
	\            -----
	\     =>          -------
	2*                         ---1*

	***now 2* is before 1* and -1 < m < 0
	*/
	if (m < -1.0f)
	{
		//switch(p1,q1)
		tmp = p1;
		p1 = q1;
		q1 = tmp;

		//switch(p2,q2)
		tmp = p2;
		p2 = q2;
		q2 = tmp;


		//***now switch(point1, point2):
		//switch(p1,p2)
		tmp = p1;
		p1 = p2;
		p2 = tmp;

		//switch(q1,q2)
		tmp = q1;
		q1 = q2;
		q2 = tmp;



		replaced = 1;
	}


	// y = mx + c
	m = (q2 - q1) / (p2 - p1);
	c = q1 - m * p1;
	x = p1; y = q1; e = p2 - p1;
	while (x <= p2)
	{
		//e = m * x + c - y;

		if (e < 0)
		{
			y = y - 1; e = e + 2 * (p2 - p1);
		}
		if (replaced == 0)
		{
			putPixel2(x, y);
			putPixel(x, y, color);
		}
		else
		{
			putPixel2(y, x);
			putPixel(y, x, color);
		}

		x = x + 1; e = e + 2 * (q2 - q1);
	}

}}
void Renderer::drawLine_z(glm::vec2 point1, glm::vec2 point2, const glm::vec3& color)
{
	int p1 = point1.x, q1 = point1.y; // point1 parameters
	int p2 = point2.x, q2 = point2.y; // point2 parameters

	int y, x;
	float m;
	int c;
	if (p1 > p2)
	{
		int temp = p1;
		p1 = p2;
		p2 = temp;

		temp = q1;
		q1 = q2;
		q2 = temp;


	}
	int replaced = 0;

	//first deal with special cases like p2 - p1 = 0 or q2 - q1 = 0
	if (p2 - p1 == 0)
	{
		int min = q2 >= q1 ? q1 : q2;
		int max = q2 <= q1 ? q1 : q2;


		for (int h = min; h < max; h++)
		{
			putPixel2(p1, h);
			putPixel(p1, h,  color);
		}

		return;
	}
	if (q2 - q1 == 0)
	{
		int min = p2 >= p1 ? p1 : p2;
		int max = p2 <= p1 ? p1 : p2;

		for (int w = min; w < max; w++)
		{
			putPixel2(w, q1);
			putPixel(w, q1,  color);
			
		}

		return;
	}





	//for measuring distance between the line's y and the approximation's y
	int e;
	int tmp;

	m = float(q2 - q1) / float(p2 - p1);

	if (m >= 0.0f) //m>=0
	{
		if (m > 1.0f) // if m>1 replace x & y for both points
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;

			replaced = 1;
		}

		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1, y = q1, e = -1 * (p2 - p1);


		while (x <= p2)
		{
			//e = m*x*(dp) + c*dp - y*dp - dp; //measuring distance
			if (e > 0)
			{
				y = y + 1;
				e = e - 2 * (p2 - p1);
			}
			if (replaced == 0)
			{
				putPixel2(x, y);
				putPixel(x, y, color);
				
			}
			else
			{
				putPixel2(y, x);
				putPixel(y, x, color);
				
			}
			
			x = x + 1; //for next point
			e = e + 2 * (q2 - q1); //line's y got bigger by m*dp
		}
	}


	//m < 0 - similar to m>0 but it's less readable if we would try to combine
	else
	{



		//if m<-1 should swap(x,y) for both points
		// and also swap the two points between themselves
		/*
		1*           2*
		\            -----
		\     =>          -------
		2*                         ---1*

		***now 2* is before 1* and -1 < m < 0
		*/
		if (m < -1.0f)
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;


			//***now switch(point1, point2):
			//switch(p1,p2)
			tmp = p1;
			p1 = p2;
			p2 = tmp;

			//switch(q1,q2)
			tmp = q1;
			q1 = q2;
			q2 = tmp;



			replaced = 1;
		}


		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1; y = q1; e = p2 - p1;
		while (x <= p2)
		{
			//e = m * x + c - y;

			if (e < 0)
			{
				y = y - 1; e = e + 2 * (p2 - p1);
			}
			if (replaced == 0)
			{
				putPixel2(x, y);
				putPixel(x, y, color);	
			}
			else
			{
				putPixel2(y, x);
				putPixel(y, x, color);
			}
		
			x = x + 1; e = e + 2 * (q2 - q1);
		}

	}


}
void Renderer::drawLine(glm::vec3 point1, glm::vec3 point2, const glm::vec3& color)
{
	int p1 = point1.x, q1 = point1.y, z1 = point1.z; // point1 parameters
	int p2 = point2.x, q2 = point2.y, z2 = point2.z; // point2 parameters
	float d_z;
	int y, x;
	float m;
	int c;
	if (p1 > p2)
	{
		int temp = p1;
		p1 = p2;
		p2 = temp;

		temp = q1;
		q1 = q2;
		q2 = temp;


		temp = z1;
		z1 = z2;
		z2 = temp;
	}
	int replaced = 0;

	//first deal with special cases like p2 - p1 = 0 or q2 - q1 = 0
	if (p2 - p1 == 0)
	{
		int min = q2 >= q1 ? q1 : q2;
		int max = q2 <= q1 ? q1 : q2;
		float z, d_z;
		if (q1 == min)
		{
			z = point1.z;
			d_z = (point2.z - z)/(min-max);
		}
		else
		{
			z = point2.z;
			d_z = (point1.z - z) / (min - max);
		}
		for (int h = min; h < max; h++)
		{
			putPixel(p1, h,z, color);
			z += d_z;
		}

		return;
	}
	if (q2 - q1 == 0)
	{
		int min = p2 >= p1 ? p1 : p2;
		int max = p2 <= p1 ? p1 : p2;
		float z, d_z;
		if (p1 == min)
		{
			z = point1.z;
			d_z = (point2.z - z) / (min - max);
		}
		else
		{
			z = point2.z;
			d_z = (point1.z - z) / (min - max);
		}
		for (int w = min; w < max; w++)
		{
			putPixel(w, q1,z, color);
			z = z + d_z;
		}

		return;
	}





	//for measuring distance between the line's y and the approximation's y
	int e; 
	int tmp;
	
	m = float(q2 - q1) / float(p2 - p1);
	
	if (m >= 0.0f) //m>=0
	{
		if (m > 1.0f) // if m>1 replace x & y for both points
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;
			
			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;

			replaced = 1;
		}

		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1, y = q1, e = -1 * (p2 - p1);
		
		
		float z = z1, z_d = (z2 - z1) / (p2 - p1);
		while (x <= p2)
		{
			//e = m*x*(dp) + c*dp - y*dp - dp; //measuring distance
			if (e > 0)
			{
				y = y + 1;
				e = e - 2 * (p2 - p1);
			}
			if (replaced == 0)
				putPixel(x, y,z, color);
			else
				putPixel(y, x,z, color);
			z = z + z_d;
			x = x + 1; //for next point
			e = e + 2 * (q2 - q1); //line's y got bigger by m*dp
		}
	}


	//m < 0 - similar to m>0 but it's less readable if we would try to combine
	else
	{
		
		
		
		//if m<-1 should swap(x,y) for both points
		// and also swap the two points between themselves
		/*
		   1*           2*
			 \            -----
			  \     =>          -------
			  2*                         ---1*
			
			***now 2* is before 1* and -1 < m < 0
		*/
		if (m < -1.0f)
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;
			

			//***now switch(point1, point2):
			  //switch(p1,p2)
			tmp = p1;
			p1 = p2;
			p2 = tmp;

			  //switch(q1,q2)
			tmp = q1;
			q1 = q2;
			q2 = tmp;

		
			tmp = z1;
			z1 = z2;
			z2 = tmp;

			replaced = 1;
		}


		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1; y = q1; e = p2 - p1;
		float z = z1, z_d = (z2 - z1) / (p2 - p1);
		while (x <= p2)
		{
			//e = m * x + c - y;
			
			if (e < 0)
			{
				y = y - 1; e = e + 2 * (p2-p1);
			}
			if (replaced == 0)
				putPixel(x, y,z, color);
			else
				putPixel(y, x,z, color);
			z = z + z_d;
			x = x + 1; e = e + 2*(q2 - q1);
		}

	}


}
void Renderer::drawLine_ground(glm::vec2 start, glm::vec2 end, 
	glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	const glm::vec3& color1, const glm::vec3& color2, const glm::vec3& color3)
{
	int p1 = start.x, q1 = start.y; // start parameters
	int p2 = end.x, q2 = end.y; // end parameters
	int y, x;
	float m;
	int c;
	if (p1 > p2)
	{
		int temp = p1;
		p1 = p2;
		p2 = temp;

		temp = q1;
		q1 = q2;
		q2 = temp;
	}
	int replaced = 0;
	glm::vec3 color = color1;
	//first deal with special cases like p2 - p1 = 0 or q2 - q1 = 0
	if (p2 - p1 == 0)
	{
		
		int min = q2 >= q1 ? q1 : q2;
		int max = q2 <= q1 ? q1 : q2;
		for (int h = min; h < max; h++)
		{
			putPixel2(p1, h,point1, point2,point3 ,color1, color2, color3);
		}

		return;
	}
	if (q2 - q1 == 0)
	{
		int min = p2 >= p1 ? p1 : p2;
		int max = p2 <= p1 ? p1 : p2;
		for (int w = min; w < max; w++)
		{
			putPixel2(w, q1, point1, point2, point3, color1, color2, color3);
		}

		return;
	}





	//for measuring distance between the line's y and the approximation's y
	int e; 
	int tmp;
	
	m = float(q2 - q1) / float(p2 - p1);
	
	if (m >= 0.0f) //m>=0
	{
		if (m > 1.0f) // if m>1 replace x & y for both points
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;

			replaced = 1;
		}

		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1, y = q1, e = -1 * (p2 - p1);


		while (x <= p2)
		{
			//e = m*x*(dp) + c*dp - y*dp - dp; //measuring distance
			if (e > 0)
			{
				y = y + 1;
				e = e - 2 * (p2 - p1);
			}
			if (replaced == 0)
				putPixel2(x, y, point1, point2, point3, color1, color2, color3);
			else
				putPixel2(y, x, point1, point2, point3, color1, color2, color3);
		
			x = x + 1; //for next point
			e = e + 2 * (q2 - q1); //line's y got bigger by m*dp
		}
	}


	//m < 0 - similar to m>0 but it's less readable if we would try to combine
	else
	{
		
		
		
		//if m<-1 should swap(x,y) for both points
		// and also swap the two points between themselves
		/*
		   1*           2*
			 \            -----
			  \     =>          -------
			  2*                         ---1*
			
			***now 2* is before 1* and -1 < m < 0
		*/
		if (m < -1.0f)
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;
			

			//***now switch(point1, point2):
			  //switch(p1,p2)
			tmp = p1;
			p1 = p2;
			p2 = tmp;

			  //switch(q1,q2)
			tmp = q1;
			q1 = q2;
			q2 = tmp;

			replaced = 1;
		}


		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1; y = q1; e = p2 - p1;

		while (x <= p2)
		{
			//e = m * x + c - y;
			
			if (e < 0)
			{
				y = y - 1; e = e + 2 * (p2-p1);
			}
			if (replaced == 0)
				putPixel2(x, y, point1, point2, point3, color1, color2, color3);
			else	
				putPixel2(y, x, point1, point2, point3, color1, color2, color3);
			
			x = x + 1; e = e + 2*(q2 - q1);
		}

	}


}
void Renderer::drawLine_phong(glm::vec2 start, glm::vec2 end,
	glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	const glm::vec3& norm1, const glm::vec3& norm2, const glm::vec3& norm3,
	float Diffus_st, vector<glm::vec3> diffus, vector<glm::vec3> directions,vector<glm::vec3> positions,
	glm::vec3 am_vec, glm::vec3 amcolor, glm::vec3 difcolor,glm::vec3 spectcolor,  const glm::vec3 & v_direction, const vector<int> & spect_exp,
	const vector<glm::vec3> & ligth_spect_c, vector<bool> type, glm::mat4x4 windowresizing_invers)
{
	int p1 = start.x, q1 = start.y; // start parameters
	int p2 = end.x, q2 = end.y; // end parameters
	int y, x;
	float m;
	int c;
	if (p1 > p2)
	{
		int temp = p1;
		p1 = p2;
		p2 = temp;

		temp = q1;
		q1 = q2;
		q2 = temp;
	}
	int replaced = 0;
	//first deal with special cases like p2 - p1 = 0 or q2 - q1 = 0
	if (p2 - p1 == 0)
	{

		int min = q2 >= q1 ? q1 : q2;
		int max = q2 <= q1 ? q1 : q2;
		for (int h = min; h < max; h++)
		{
			putPixel3(p1, h, point1, point2, point3, norm1, norm2, norm3,
				Diffus_st, diffus,directions, positions, am_vec, amcolor, difcolor, spectcolor, v_direction,
				spect_exp, ligth_spect_c, type,windowresizing_invers);
		}

		return;
	}
	if (q2 - q1 == 0)
	{
		int min = p2 >= p1 ? p1 : p2;
		int max = p2 <= p1 ? p1 : p2;
		for (int w = min; w < max; w++)
		{
			putPixel3(w, q1, point1, point2, point3, norm1, norm2, norm3,
				Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor, spectcolor,
				v_direction, spect_exp, ligth_spect_c, type, windowresizing_invers);
		}

		return;
	}





	//for measuring distance between the line's y and the approximation's y
	int e;
	int tmp;

	m = float(q2 - q1) / float(p2 - p1);

	if (m >= 0.0f) //m>=0
	{
		if (m > 1.0f) // if m>1 replace x & y for both points
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;

			replaced = 1;
		}

		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1, y = q1, e = -1 * (p2 - p1);


		while (x <= p2)
		{
			//e = m*x*(dp) + c*dp - y*dp - dp; //measuring distance
			if (e > 0)
			{
				y = y + 1;
				e = e - 2 * (p2 - p1);
			}
			if (replaced == 0)
				putPixel3(x,y, point1, point2, point3, norm1, norm2, norm3,
					Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor,
					spectcolor, v_direction, spect_exp, ligth_spect_c, type, windowresizing_invers);
			else
				putPixel3(y,x, point1, point2, point3, norm1, norm2, norm3,
					Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor,
					spectcolor, v_direction, spect_exp, ligth_spect_c, type, windowresizing_invers);

			x = x + 1; //for next point
			e = e + 2 * (q2 - q1); //line's y got bigger by m*dp
		}
	}


	//m < 0 - similar to m>0 but it's less readable if we would try to combine
	else
	{



		//if m<-1 should swap(x,y) for both points
		// and also swap the two points between themselves
		/*
		1*           2*
		\            -----
		\     =>          -------
		2*                         ---1*

		***now 2* is before 1* and -1 < m < 0
		*/
		if (m < -1.0f)
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;


			//***now switch(point1, point2):
			//switch(p1,p2)
			tmp = p1;
			p1 = p2;
			p2 = tmp;

			//switch(q1,q2)
			tmp = q1;
			q1 = q2;
			q2 = tmp;

			replaced = 1;
		}


		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1; y = q1; e = p2 - p1;

		while (x <= p2)
		{
			//e = m * x + c - y;

			if (e < 0)
			{
				y = y - 1; e = e + 2 * (p2 - p1);
			}
			if (replaced == 0)
			putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
				Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor,
				spectcolor, v_direction, spect_exp, ligth_spect_c, type, windowresizing_invers);
			else
				putPixel3(y, x, point1, point2, point3, norm1, norm2, norm3,
					Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor,
					spectcolor, v_direction, spect_exp, ligth_spect_c, type, windowresizing_invers);

			x = x + 1; e = e + 2 * (q2 - q1);
		}

	}

}


//drawLineBADCOLOR - used for bad_color algo' - checks if the line hit a given y-value
//and if it does, return xmin & xmax
//else, return {-1,-1}
//same as regular, just uses putPixelBADCOLOR
glm::vec2 Renderer::drawLineBADCOLOR(glm::vec3 point1, glm::vec3 point2, const glm::vec3& color, int GIVENy)
{

	float xmin = INFINITY;
	float xmax = -INFINITY;
	float xvalue;




	int p1 = point1.x, q1 = point1.y; // point1 parameters
	int p2 = point2.x, q2 = point2.y; // point2 parameters

	int y, x;
	float m;
	int c;
	if (p1 > p2)
	{
		int temp = p1;
		p1 = p2;
		p2 = temp;

		temp = q1;
		q1 = q2;
		q2 = temp;


	}
	int replaced = 0;

	//first deal with special cases like p2 - p1 = 0 or q2 - q1 = 0
	if (p2 - p1 == 0)
	{
		int min = q2 >= q1 ? q1 : q2;
		int max = q2 <= q1 ? q1 : q2;


		for (int h = min; h < max; h++)
		{
			xvalue = putPixelBADCOLOR(p1, h, color, GIVENy);
			if (xvalue != -INFINITY && xvalue >= xmax)
				xmax = xvalue;
			if (xvalue != -INFINITY && xvalue <= xmin)
				xmin = xvalue;
		}

		glm::vec2 ret;
		ret.x = xmin;
		ret.y = xmax;
		return ret;
	}
	if (q2 - q1 == 0)
	{
		int min = p2 >= p1 ? p1 : p2;
		int max = p2 <= p1 ? p1 : p2;

		for (int w = min; w < max; w++)
		{
			xvalue = putPixelBADCOLOR(w, q1, color, GIVENy);
			if (xvalue != -INFINITY && xvalue >= xmax)
				xmax = xvalue;
			if (xvalue != -INFINITY && xvalue <= xmin)
				xmin = xvalue;
		}

		glm::vec2 ret;
		ret.x = xmin;
		ret.y = xmax;
		return ret;
	}





	//for measuring distance between the line's y and the approximation's y
	int e;
	int tmp;

	m = float(q2 - q1) / float(p2 - p1);

	if (m >= 0.0f) //m>=0
	{
		if (m > 1.0f) // if m>1 replace x & y for both points
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;

			replaced = 1;
		}

		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1, y = q1, e = -1 * (p2 - p1);


		while (x <= p2)
		{
			//e = m*x*(dp) + c*dp - y*dp - dp; //measuring distance
			if (e > 0)
			{
				y = y + 1;
				e = e - 2 * (p2 - p1);
			}
			if (replaced == 0)
			{
				xvalue = putPixelBADCOLOR(x, y, color, GIVENy);
				if (xvalue != -INFINITY && xvalue >= xmax)
					xmax = xvalue;
				if (xvalue != -INFINITY && xvalue <= xmin)
					xmin = xvalue;

			}
			else
			{
				xvalue = putPixelBADCOLOR(y, x, color, GIVENy);
				if (xvalue != -INFINITY && xvalue >= xmax)
					xmax = xvalue;
				if (xvalue != -INFINITY && xvalue <= xmin)
					xmin = xvalue;

			}

			x = x + 1; //for next point
			e = e + 2 * (q2 - q1); //line's y got bigger by m*dp
		}
	}


	//m < 0 - similar to m>0 but it's less readable if we would try to combine
	else
	{



		//if m<-1 should swap(x,y) for both points
		// and also swap the two points between themselves
		/*
		1*           2*
		\            -----
		\     =>          -------
		2*                         ---1*

		***now 2* is before 1* and -1 < m < 0
		*/
		if (m < -1.0f)
		{
			//switch(p1,q1)
			tmp = p1;
			p1 = q1;
			q1 = tmp;

			//switch(p2,q2)
			tmp = p2;
			p2 = q2;
			q2 = tmp;


			//***now switch(point1, point2):
			//switch(p1,p2)
			tmp = p1;
			p1 = p2;
			p2 = tmp;

			//switch(q1,q2)
			tmp = q1;
			q1 = q2;
			q2 = tmp;



			replaced = 1;
		}


		// y = mx + c
		m = (q2 - q1) / (p2 - p1);
		c = q1 - m * p1;
		x = p1; y = q1; e = p2 - p1;
		while (x <= p2)
		{
			//e = m * x + c - y;

			if (e < 0)
			{
				y = y - 1; e = e + 2 * (p2 - p1);
			}
			if (replaced == 0)
			{
				xvalue = putPixelBADCOLOR(x, y, color, GIVENy);
				if (xvalue != -INFINITY && xvalue >= xmax)
					xmax = xvalue;
				if (xvalue != -INFINITY && xvalue <= xmin)
					xmin = xvalue;
			}
			else
			{
				xvalue = putPixelBADCOLOR(y, x, color, GIVENy);
				if (xvalue != -INFINITY && xvalue >= xmax)
					xmax = xvalue;
				if (xvalue != -INFINITY && xvalue <= xmin)
					xmin = xvalue;
			}

			x = x + 1; e = e + 2 * (q2 - q1);
		}

	}


	glm::vec2 ret;
	ret.x = xmin;
	ret.y = xmax;
	return ret;
}





//Procedural Texture code

//Grayscale - maps x-values to color 
glm::vec3 Renderer::marble_color(float x)
{
	//first get a value in range [0,1]
	x = x - (int)x;

	//then return the color it represents (x, x, x) --gray-ish color
	glm::vec3 retColor = glm::vec3(x, x, x);

	return retColor;
}

//marble-texture - some random-ness
glm::vec3 Renderer::marble(float x)
{
	return marble_color(sin(x));
}



//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::initOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &glScreenTex);
	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &glScreenVtc);
	GLuint buffer;
	// Makes this VAO the current one.
	glBindVertexArray(glScreenVtc);
	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);
	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};
	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};
	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);
	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	// Make this program the current one.
	glUseProgram( program );
	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );
	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::createOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);
	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);
	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, width, height);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);
	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, glScreenTex);
	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_FLOAT, colorBuffer);
	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);
	// Make glScreenVtc current VAO
	glBindVertexArray(glScreenVtc);
	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color2)
{	
	glm::vec3 color = color2;
	if (fog)
		color = glm::vec3(0, 0, 0.0000001f);
	back_round_color = color;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			putPixel(i, j,color);
		}
	}
}

void Renderer::Viewport(int w, int h)
{
	/*
	if (SuperSampling)
	{
		h = h * sampel_size;
		w = w * sampel_size;
	}
	*/
	if (w == width && h == height)
	{
		return;
	}
	width = w;
	height = h;
	delete[] colorBuffer;
	colorBuffer = new float[3 * h*w];
	createOpenGLBuffer();
}

