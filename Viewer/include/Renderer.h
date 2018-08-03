#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>

using namespace std;

/*
 * Renderer class. This class takes care of all the rendering operations needed for rendering a full scene to the screen.
 * It contains all the data structures we learned in class plus your own data structures.
 */
class Renderer
{
private:
	// 3*width*height
	float *colorBuffer;
	// width*height
	float *zBuffer;
	// Screen dimensions
	int width, height;

	// Added more attributes. all you need to keep to drawTriangle
	glm::mat4x4 myCameraTransform; 
	glm::mat4x4 myProjection;
	glm::mat4x4 oTransform;
	glm::mat4x4 nTransform;

	// Draws a pixel in location p with color color
	void putPixel(int i, int j, const glm::vec3& color);
	// creates float array of dimension [3,w,h]
	void createBuffers(int w, int h);
	//##############################
	//##openGL stuff. Don't touch.##
	//##############################
	GLuint glScreenTex;
	GLuint glScreenVtc;
	void createOpenGLBuffer();
	void initOpenGLRendering();
	//##############################
	void horizontal(int i, int i2, int j, int r);
	void vertical(int j, int j2, int i, int r);
	void Bresenham(int i, int j, int i2, int j2,int r);
public:
	Renderer();
	Renderer(int w, int h);
	~Renderer();
	// Local initializations of your implementation
	void Init();

	// Draws wireframe triangles to the color buffer

	void DrawTriangles(const glm::vec4* vertexPositionVECTOR, int size, glm::vec4 color,int a,int b);

	// Sets the camera transformations with relation to world coordinates
	void SetCameraTransform(const glm::mat4x4& cTransform);

	// Sets the camera projection (perspective, orthographic etc...)
	void SetProjection(const glm::mat4x4& projection);

	// Sets the transformations for model and normals. The object transformations 
	// decide the spacial relations of the object with respect to the world.
	void SetObjectMatrices(const glm::mat4x4& oTransform, const glm::mat4x4& nTransform);

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
	void drawLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color);

	//Task1, naive solution to draw a line
	void printLineNaive();


};
