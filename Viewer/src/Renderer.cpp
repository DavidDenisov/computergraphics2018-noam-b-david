#include "Renderer.h"
#include "InitShader.h"
#include <imgui/imgui.h>
#include <iostream>
#include <cmath>
#include "Camera.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)

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
float norm(const glm::vec3 v)
{
		//return pow((abs(v.x) + abs(v.y) + abs(v.z)),0.5);
		float ans = max(v.x, 0.f) + max(v.y, 0.f) + max(v.z, 0.f);
		ans=pow(ans,0.5);
		if (ans > 1.f)
			return 1.f;
		return ans;
}
void Renderer::SetCameraTransform(const glm::mat4x4& cTransform)
{
	myCameraTransform = cTransform;
}
void Renderer::SetProjection(const glm::mat4x4& projection)
{
	myProjection = projection;
}
Renderer::Renderer() : width(1280), height(720),
myCameraTransform(1.0f), myProjection(1.0f), worldTransform(1.0f), nTransform(1.0f)
{
	initOpenGLRendering();
	createBuffers(1280,720);
}

Renderer::Renderer(int w, int h) : width(w), height(h),
myCameraTransform(1.0f), myProjection(1.0f), worldTransform(1.0f), nTransform(1.0f)
{
	initOpenGLRendering();
	createBuffers(w,h);
}

Renderer::~Renderer()
{
	delete[] colorBuffer;
}
void Renderer::putPixel_no_check(int i, int j, const glm::vec3& color)
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
	if (z > zBuffer[i][j])
	{
		zBuffer[i][j] = z;
		colorBuffer[INDEX(width, i, j, 0)] = color.x;
		colorBuffer[INDEX(width, i, j, 1)] = color.y;
		colorBuffer[INDEX(width, i, j, 2)] = color.z;
	}
}

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
	if (point_z > zBuffer[i][j])
	{
		zBuffer[i][j] = point_z;
		colorBuffer[INDEX(width, i, j, 0)] = color.x;
		colorBuffer[INDEX(width, i, j, 1)] = color.y;
		colorBuffer[INDEX(width, i, j, 2)] = color.z;
		
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
	if (point_z > zBuffer[x1][y1])
	{
		zBuffer[x1][y1] = point_z;
		colorBuffer[INDEX(width, x1, y1, 0)] = color.x;
		colorBuffer[INDEX(width, x1, y1, 1)] = color.y;
		colorBuffer[INDEX(width, x1, y1, 2)] = color.z;

	}
	
}
void Renderer::putPixel3(int x1, int y1, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	glm::vec3 norm1, glm::vec3 norm2, glm::vec3 norm3,
	float Diffus_st, vector<glm::vec3> diffus,
	vector<glm::vec3> direction, vector<glm::vec3> positions, glm::vec3 am_vec,
	glm::vec3 amcolor, glm::vec3 difcolor, glm::vec3 spectcolor, const vector<bool> & types)
{
	float dist1 = abs(x1 - point1.x) + abs(y1 - point1.y);
	float dist2 = abs(x1 - point2.x) + abs(y1 - point2.y);
	float dist3 = abs(x1 - point3.x) + abs(y1 - point3.y);
	float sum1 = dist1 + dist2 + dist3;
	glm::vec3 cur_norm = norm1 * (dist1 / sum1) + norm2 * (dist2 / sum1) + norm3 * (dist3 / sum1);
	float point_z = point1.z*(dist1 / sum1) + point2.z*(dist2 / sum1) + point3.z*(dist3 / sum1);
	if (point_z > zBuffer[x1][y1])
	{
	float x2 = 0.f;

	glm::vec3  am_color = am_vec * am_vec;
	glm::vec3  dif_color = glm::vec3(0, 0, 0);
	glm::vec3  spect_color = glm::vec3(0, 0, 0);
	for (int i = 0; i < diffus.size(); i++)
	{
		glm::vec3 diffus_dir = glm::normalize(positions[i] - glm::vec3(x1, y1, point_z));
		if (types[i])
			diffus_dir = direction[i];

		if ((norm(diffus_dir) == 0.f) && (norm(-diffus_dir) != 0.f))
		{
			diffus_dir = -diffus_dir;
			cur_norm = -cur_norm;
		}
		if ((!isnan(cur_norm.x)) && (!isnan(cur_norm.z)) && (!isnan(cur_norm.y)))
			if ((norm(diffus_dir) != 0.f) && (norm(cur_norm) != 0.f) && (sum(cur_norm) != 0.f))
				x2 = norm(diffus_dir * cur_norm) / (norm(diffus_dir)*norm(cur_norm));

		x2 = fmin(1.f, x2);
		dif_color = dif_color + glm::vec3((diffus[i] * x2* Diffus_st));
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
	const glm::vec3 & amcolor, const glm::vec3 & difcolor, const glm::vec3 & spectcolor,
	float w, float h, glm::mat4x4 windowresizing,
	MeshModel* myModel, Camera* activeCam, const glm::vec3 & am_vec, const vector<glm::vec3> & diffus
	, const vector<glm::vec3> & positions, const vector<glm::vec3> & directions,
	const vector<bool> & ligth_type, int type)
{
	//we recieve the object to draw with a vector of verticesPositions
	//we will draw these triangles but first will do the transformations
	zBuffer = new float*[w];
	for (int i = 0; i < w; i++)
	{
		zBuffer[i] = new float[h];
		for (int j = 0; j < h; j++)
			zBuffer[i][j] = -INFINITY;
	}


	//first do the transformations:
	myCameraTransform = activeCam->get_camWorldTransform() * activeCam->get_camModelTransform();
	//the view matrix
	glm::mat4x4 view = worldTransform * glm::inverse(myCameraTransform); // T = M * C^-1


	//now the project transformation:
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
		/*
		transVerticesPositions[i].x = w/2 * transVerticesPositions[i].x + w/2;
		*/
	}

	//now draw the triangles (and always before put them in vec2) !!!
	glm::vec3 a(0.0f, 0.0f, 0.0f), b(0.0f, 0.0f, 0.0f), c(0.0f, 0.0f, 0.0f);
	for (int face = 0; face < size - 2; face = face + 3)
	{

		a = transVerticesPositions[face];
		b = transVerticesPositions[face + 1];
		c = transVerticesPositions[face + 2];


		//draw triangle [a,b,c]
		if (type == 0)//flat
		{

			glm::vec3 avg = (a + b + c) / 3.f;
			glm::vec3 cface_norm = myModel->getNormalFace()[face];
			float x3 = 0.f;

			glm::vec3  AMcolor = am_vec * amcolor;
			glm::vec3  Difuscolor = glm::vec3(0, 0, 0);
			glm::vec3  Spectcolor = glm::vec3(0, 0, 0);
			for (int i = 0; i < diffus.size(); i++)
			{
				glm::vec3 diffus_dir = avg - positions[i];
				if (ligth_type[i])
					diffus_dir = directions[i];
				
				glm::vec3 face_norm = cface_norm;
				
				if ((norm(diffus_dir) == 0.f) && (norm(-diffus_dir) != 0.f))
				{
					face_norm = -face_norm;
					diffus_dir = -diffus_dir;
				}
				if ((!isnan(face_norm.x)) && (!isnan(face_norm.z)) && (!isnan(face_norm.y)))
					if ((norm(diffus_dir) != 0.f) && (norm(face_norm) != 0.f) &&
							(sum(face_norm) != 0.f))
							x3 = norm(diffus_dir * face_norm) /(norm(diffus_dir)*norm(face_norm));
				
				
				x3 = fmin(1.f, x3);


				Difuscolor = Difuscolor + diffus[i]*x3* myModel->Diffus;
			}
			glm::vec3 color = AMcolor + (Difuscolor*difcolor) + (Spectcolor*spectcolor);
			drawTringle(a, b, c, color, w, h);
		}
		if (type == 1)// Gouraud 
		{
			glm::vec3 cv1 = myModel->getNormalVertex()[face];
			glm::vec3 cv2 = myModel->getNormalVertex()[face+1];
			glm::vec3 cv3 = myModel->getNormalVertex()[face+2];

			float x1 = 0.f;
			float x2 = 0.f;
			float x3 = 0.f;
			glm::vec3  AMcolor1 = am_vec * amcolor;
			glm::vec3  Difuscolor1 = glm::vec3(0, 0, 0);
			glm::vec3  Spectcolor1 = glm::vec3(0, 0, 0);
			glm::vec3  AMcolor2 = am_vec * amcolor;
			glm::vec3  Difuscolor2 = glm::vec3(0, 0, 0);
			glm::vec3  Spectcolor2 = glm::vec3(0, 0, 0);
			glm::vec3  AMcolor3 = am_vec * amcolor;
			glm::vec3  Difuscolor3 = glm::vec3(0, 0, 0);
			glm::vec3  Spectcolor3 = glm::vec3(0, 0, 0);

			for (int i = 0; i < diffus.size(); i++)
			{
				glm::vec3 v1 = cv1, v2 = cv2, v3 = cv3;
				glm::vec3 cur_d1 = glm::normalize(positions[i]-a);
				glm::vec3 cur_d2 = glm::normalize(positions[i] - b);
				glm::vec3 cur_d3 = glm::normalize(positions[i] - c);
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
				if ((!isnan(v1.x)) && (!isnan(v1.z)) && (!isnan(v1.y)) )
					if ((norm(cur_d1) != 0.f) && (norm(v1) != 0.f) && (sum(v1) != 0.f))
						x1 = norm(cur_d1 * v1) / (norm(cur_d1)*norm(v1));

				if ((!isnan(v2.x)) && (!isnan(v2.z)) && (!isnan(v2.y)) )
					if ((norm(cur_d2) != 0.f) && (norm(v2) != 0.f) && (sum(v2) != 0.f))
						x2 = norm(cur_d2* v2) / (norm(cur_d2)*norm(v2));

				if ((!isnan(v3.x)) && (!isnan(v3.z)) && (!isnan(v3.y)) )
					if ((norm(cur_d3) != 0.f) && (norm(v3) != 0.f) && (sum(v3) != 0.f))
						x3 = norm(cur_d3 * v3) / (norm(cur_d3)*norm(v3));

				x1 = fmin(1.f, x1);
				x2 = fmin(1.f, x2);
				x3 = fmin(1.f, x3);

				Difuscolor1 = Difuscolor1 + glm::vec3( (diffus[i]*x1* myModel->Diffus));
				Difuscolor2 = Difuscolor2 + glm::vec3( (diffus[i]*x2* myModel->Diffus));
				Difuscolor3 = Difuscolor3 + glm::vec3( (diffus[i]*x3* myModel->Diffus));
			}
			glm::vec3 color1 = AMcolor1 + (Difuscolor1 * difcolor) + (Spectcolor1 * spectcolor);
			glm::vec3 color2 = AMcolor2 + (Difuscolor2 * difcolor) + (Spectcolor2 * spectcolor);
			glm::vec3 color3 = AMcolor3 + (Difuscolor3 * difcolor) + (Spectcolor3 * spectcolor);
			drawTringle(a, b, c, color1, color2, color3, w, h);
		}
		//if (type == 2 && myModel->willDrawVertexNormal == 1)
		if (type == 2)//Phong
		{
			glm::vec4 v1 = myModel->getNormalVertex()[face];
			glm::vec4 v2 = myModel->getNormalVertex()[face + 1];
			glm::vec4 v3 = myModel->getNormalVertex()[face + 2];
			drawTringle(a, b, c, v1,v2,v3, myModel->Diffus,diffus,directions,positions, am_vec,
				amcolor,difcolor, spectcolor,ligth_type, w, h);
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

	
	delete[] transVerticesPositions; //they take a lot of memory and will not be used again
	delete[] drawVertexPositions;
	delete[] vertexPositions;
	for (int i = 0; i < w; i++)
		delete[] zBuffer[i];
	delete[] zBuffer;
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

void Renderer::drawTringle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
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

	bool put = FALSE;
	//bool cont = TRUE;
	for (int y = ymin; y < ymax; y++)
	{
		put = FALSE;
		int count=0;
		for (int x = xmin; x <= xmax; x++) //not used, just for fun
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
					count++;
				}
			}
		}

		put = FALSE;
		//cont = TRUE;
		if (count == 2)
		{
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
		}
		
	}
	/*for (int x = xmin; x < xmax; x++)
	{
		put = FALSE;
		int count = 0;
		for (int y = ymin; y < ymax; y++)
		{
			if (bad_color == glm::vec3(colorBuffer[INDEX(width, x, y, 0)]
				, colorBuffer[INDEX(width, x, y, 1)],
				colorBuffer[INDEX(width, x, y, 2)]))
				count++;
		}
		if (count % 2 == 0)
		{
			for (int y = ymin; y < ymax; y++)
			{
				if (put)
				{
					if (bad_color == glm::vec3(colorBuffer[INDEX(width, x, y, 0)]
						, colorBuffer[INDEX(width, x, y, 1)],
						colorBuffer[INDEX(width, x, y, 2)]))
						put = FALSE;
					putPixel(x, y, color);
				}
				else
				{
					if (bad_color == glm::vec3(colorBuffer[INDEX(width, x, y, 0)]
						, colorBuffer[INDEX(width, x, y, 1)],
						colorBuffer[INDEX(width, x, y, 2)]))
						put = TRUE;
				}
			}
		}
	}*/
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

	drawLine(point1, point2, color);
	drawLine(point1, point3, color);
	drawLine(point2, point3, color);

}
void Renderer::drawTringle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, 
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

	bool put = FALSE;
	//bool cont = TRUE;
	for (int y = ymin; y < ymax; y++)
	{
		put = FALSE;
		int count = 0;
		for (int x = xmin; x <= xmax; x++) //used, not just for fun very importent
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
				count++;
			}

		}
		put = FALSE;
		//cont = TRUE;
		if (count==2)
		{
			for (int x = xmin; (x <= xmax); x++)
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
}

void Renderer::drawTringle(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
	const glm::vec3&  norm1, const glm::vec3&  norm2, const glm::vec3&  norm3,
	float Diffus_st, vector<glm::vec3> diffus, vector<glm::vec3> directions , vector<glm::vec3> positions
	, glm::vec3 am_vec, glm::vec3 amcolor, glm::vec3 difcolor, glm::vec3 spectcolor,
	const vector<bool> & types,float w, float h)
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

	bool put = FALSE;
	//bool cont = TRUE;
	for (int y = ymin; y < ymax; y++)
	{
		put = FALSE;
		int count = 0;
		for (int x = xmin; x <= xmax; x++) //used, not just for fun very importent
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
				count++;
			}

		}
		put = FALSE;
		//cont = TRUE;
		if (count == 2)
		{
			for (int x = xmin; (x <= xmax); x++)
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
								Diffus_st, diffus,directions, positions, am_vec, amcolor, difcolor, 
								spectcolor, types);
							x++;
						}
						x--;
						put = FALSE;
						//cont = FALSE; //this is the second edge. no need to continue
					}
					putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
						Diffus_st, diffus, directions, positions,
						am_vec, amcolor, difcolor, spectcolor,types);
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
							Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor,
							spectcolor, types);
						x++;
						//draw the whole edge, and only afterwards start looking for the next one
						while (bad_color.x == colorBuffer[INDEX(width, x, y, 0)] &&
							bad_color.y == colorBuffer[INDEX(width, x, y, 1)] &&
							bad_color.z == colorBuffer[INDEX(width, x, y, 2)])
						{
							putPixel3(x, y, point1, point2, point3, norm1, norm2, norm3,
								Diffus_st, diffus,directions, positions,
								am_vec, amcolor, difcolor, spectcolor,types);
							x++;
						}
						x--;
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
		Diffus_st, diffus,directions, positions, am_vec, amcolor, difcolor, spectcolor, types);

	drawLine_phong(point2, point3,
		point1, point2, point3,
		norm1, norm2, norm3,
		Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor, spectcolor, types);

	drawLine_phong(point1, point3,
		point1, point2, point3,
		norm1,norm2,norm3,
		Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor, spectcolor,types);
}
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
	glm::vec3 am_vec, glm::vec3 amcolor, glm::vec3 difcolor,glm::vec3 spectcolor, vector<bool> type)
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
				Diffus_st, diffus,directions, positions, am_vec, amcolor, difcolor, spectcolor, type);
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
				Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor, spectcolor, type);
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
					Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor, spectcolor, type);
			else
				putPixel3(y,x, point1, point2, point3, norm1, norm2, norm3,
					Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor, spectcolor, type);

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
				Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor, spectcolor, type);
			else
				putPixel3(y, x, point1, point2, point3, norm1, norm2, norm3,
					Diffus_st, diffus, directions, positions, am_vec, amcolor, difcolor, spectcolor, type);

			x = x + 1; e = e + 2 * (q2 - q1);
		}

	}

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

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
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
