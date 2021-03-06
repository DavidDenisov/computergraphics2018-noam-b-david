#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

#include "MeshModel.h"
#include "Camera.h"
using namespace std;

/*
 * Renderer class. This class takes care of all the rendering operations needed for rendering a full scene to the screen.
 * It contains all the data structures we learned in class plus your own data structures.
 */
class Renderer
{
private:
	bool auto_color = false;
	vector<vector<glm::vec3>> textur_map;// 10000*10000
	glm::vec3 back_round_color;
	// 3*width*height
	float *colorBuffer;
	vector<glm::vec3>  colorBuffer2;
	vector<glm::vec2>  colorBuffer3;
	// width*height
	
	// Screen dimensions
	int width, height;

	// Added more attributes. all you need to keep to drawTriangle
	glm::vec3 bad_color = glm::vec3(0.33349, 0.6349, 0.46539);
	glm::mat4x4 myCameraTransform; 
	glm::mat4x4 myProjection;
	glm::mat4x4 worldTransform;
	glm::mat4x4 nTransform;

	//find z-value of (x,y) in triangle (p1,p2,p3)
	float findZ(int x1, int y1, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
	void putPixel_no_check(int i, int j, const glm::vec3& color);
	void putPixel2(int i, int j);
	void putPixel3(int x1, int y1, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
		glm::vec3 norm1, glm::vec3 norm2, glm::vec3 norm3,
		float Diffus_st, vector<glm::vec3> diffus,
		vector<glm::vec3> direction, vector<glm::vec3> positions, glm::vec3 am_vec,
		glm::vec3 amcolor, glm::vec3 difcolor, glm::vec3 spectcolor, const glm::vec3 & v_direction, const vector<int> & spect_exp,
		const vector<glm::vec3> & ligth_spect_c, const vector<bool> & types, glm::mat4x4 windowresizing_invers, MeshModel* myModel, int face);
	//myModel & face is for texture. to know the vertices p1,p2,p3 before the tranformations...

	void putPixel2(int x1, int y1, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3
		, const glm::vec3& color1, const glm::vec3& color2, const glm::vec3& color3);

	// Draws a pixel in location p with color color
	void putPixel(int i, int j, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
		const glm::vec3& color);
	void putPixel(int i, int j, int z, const glm::vec3& color, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
	void putPixel(int i, int j, const glm::vec3& color);
	// creates float array of dimension [3,w,h]
	void createBuffers(int w, int h);

	//"putpixelBADCOLOR" - draws the color(if needed) but returns TRUE when hit s a given y
	float putPixelBADCOLOR(int i, int j, const glm::vec3& color, int GIVENy);

	//interpolate color by two points by x or y (value = the x/y value that we want his color)
	static glm::vec3 interpolate(int value, bool xORy, glm::vec3 point1, glm::vec3 point2, glm::vec3 color1, glm::vec3 color2);

	bool fog = false;
	bool superSampling = false;
	float zFar = 1;




	//##############################
	//##openGL stuff. Don't touch.##
	//##############################
	GLuint glScreenTex;
	GLuint glScreenVtc;
	void createOpenGLBuffer();
	void initOpenGLRendering();
	//##############################

public:
	
	float sampel_size = 1.f;
	float get_zFar();
	void set_zFar(float f);
	bool getSuperSampling();
	void set_superSampling(bool x);
	bool get_fog();
	void set_fog(bool x);
	void drawLine_reset(glm::vec3 point1, glm::vec3 point2, glm::vec3 color);


	bool get_auto_color();
	void set_auto_color(bool x); 
	float **zBuffer;
	void horizontal(int i, int i2, int j, int r);
	void vertical(int j, int j2, int i, int r);
	void Bresenham(int i, int j, int i2, int j2,int r);
	Renderer();
	Renderer(int w, int h);
	~Renderer();
	// Local initializations of your implementation
	void Init();

	bool shouldntFill(glm::vec2 a, glm::vec2 b, glm::vec2 c);

	void Renderer::drawTringleFlat(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
		const glm::vec3&  color, float w, float h);
	
	void Renderer::drawTringleGouraud(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
		const glm::vec3&  color1, const glm::vec3&  color2, const glm::vec3&  color3, float w, float h);

	void Renderer::drawTringlePhong(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
		const glm::vec3&  norm1, const glm::vec3&  norm2, const glm::vec3&  norm3,
		float Diffus_st, vector<glm::vec3> diffus, vector<glm::vec3> directions, vector<glm::vec3> positions
		, glm::vec3 am_vec, glm::vec3 amcolor, glm::vec3 difcolor, glm::vec3 spectcolor, const glm::vec3 & v_direction, const vector<int> & spect_exp,
		const vector<glm::vec3> & ligth_spect_c,
		const vector<bool> & types, glm::mat4x4 windowresizing_invers, float w, float h, MeshModel* myModel, int face);
	// Draws wireframe triangles to the color buffer



	//do drawTriangle algo' but chooses pixel color with texture + light (light is given ready to use)
	//this algo will work exacly the same for Gouraud & Flat shading because the normal doesn't change
	//thus light stays the same, and can be given as paramaters
	void drawTriangleTexture(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3, glm::vec3 ambLight, glm::vec3 difLight, glm::vec3 spectLight);


	//void DrawTriangles(glm::vec4* vertexPositionVECTOR, int size, glm::vec4 color
		//, float w, float h, glm::mat4x4 windowresizing, MeshModel* myModel, Camera* activeCam,const glm::vec3 & am_vec);
	void DrawTriangles(glm::vec4* vertexPositions, int size,
		const glm::vec3 & AMcolor, const glm::vec3 & Difcolor, const glm::vec3 & SPECTcolor,float w, float h,
		glm::mat4x4 windowresizing,MeshModel* myModel, Camera* activeCam, const glm::vec3 & am_vec,
		const vector<glm::vec3> & diffus, const vector<glm::vec3> & positions,
		const vector<glm::vec3> & directions,const vector<bool> & ligth_type,
		const glm::vec3 & v_direction,const vector<int> & spect_exp,
		const vector<glm::vec3> & ligth_spect_c, int type, int count);
	
	// Sets the camera transformations with relation to world coordinates
	void SetCameraTransform(const glm::mat4x4& cTransform);

	// Sets the camera projection (perspective, orthographic etc...)
	void SetProjection(const glm::mat4x4& projection);

	// Sets the transformations for model and normals. The object transformations 
	// decide the spacial relations of the object with respect to the world.
	void SetObjectMatrices(const glm::mat4x4& worldTransform, const glm::mat4x4& nTransform);

	// Swaps between the back buffer and front buffer, as explained in class.
	// https://en.wikipedia.org/wiki/Multiple_buffering#Double_buffering_in_computer_graphics
	void SwapBuffers();

	// Sets the color buffer to a new color (all pixels are set to this color).
	void ClearColorBuffer(const glm::vec3& color);

	// Resize the buffer.
	void Viewport(int w, int h);

	// Clears the z buffer to zero.
	void ClearDepthBuffer();

	// Draw wide vertical and horizontal lines on the screen
	void SetDemoBuffer();
  
	//new function for home work
	void drawLine_z(glm::vec2 point1, glm::vec2 point2, const glm::vec3& color);

	void drawLine(glm::vec3 point1, glm::vec3 point2, const glm::vec3& color);
	

	void drawLine_ground(glm::vec2 start, glm::vec2 end,
		glm::vec3 point1,glm::vec3 point2, glm::vec3 point3,
		const glm::vec3& color1, const glm::vec3& color2, const glm::vec3& color3);

	void drawLine_phong(glm::vec2 start, glm::vec2 end,
		glm::vec3 point1, glm::vec3 point2, glm::vec3 point3,
		const glm::vec3& norm1, const glm::vec3& norm2, const glm::vec3& norm3,
		float Diffus_st, vector<glm::vec3> diffus, vector<glm::vec3> directions, vector<glm::vec3> positions,
		glm::vec3 am_vec, glm::vec3 amcolor, glm::vec3 difcolor, glm::vec3 spectcolor,  const glm::vec3 & v_direction, const vector<int> & spect_exp,
		const vector<glm::vec3> & ligth_spect_c, vector<bool> type, glm::mat4x4 windowresizing_invers, MeshModel* myModel, int face);



	//drawLineBADCOLOR - used for bad_color algo' - checks if the line hit a given y-value
	//and returns xmin & xmax where y was drawn
	//same as regular, just uses putPixelBADCOLOR
	glm::vec2 drawLineBADCOLOR(glm::vec3 point1, glm::vec3 point2, const glm::vec3& color, int GIVENy);

	//Task1, naive solution to draw a line
	void printLineNaive();




	//Procedural Texture code
	glm::vec3 marble_color(float x); //Grayscale - maps x-values to color 
	glm::vec3 marble(float x); //marble-texture - some random-ness 
};
