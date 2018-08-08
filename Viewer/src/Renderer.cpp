#include "Renderer.h"
#include "InitShader.h"
#include <imgui/imgui.h>
#include <iostream>

#include "Camera.h"

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
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

void Renderer::putPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= width) return;
	if (j < 0) return; if (j >= height) return;
	colorBuffer[INDEX(width, i, j, 0)] = color.x;
	colorBuffer[INDEX(width, i, j, 1)] = color.y;
	colorBuffer[INDEX(width, i, j, 2)] = color.z;
}

void Renderer::SetObjectMatrices(const glm::mat4x4& worldTransform, const glm::mat4x4& nTransform)
{
	this->worldTransform = worldTransform;
	this->nTransform = nTransform;
}

void Renderer::DrawTriangles(glm::vec4* vertexPositions, int size,
	glm::vec4 color,int w,int h, glm::mat4x4 windowresizing, MeshModel* myModel, Camera* activeCam)
{
	//we recieve the object to draw with a vector of verticesPositions
	//we will draw these triangles but first will do the transformations


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
			/transVerticesPositions[i].w; //normallize them
		
		//now the points are NDC. normalized Device Coordinates
		//now do window coordinates transformation
		transVerticesPositions[i] = windowresizing * transVerticesPositions[i];
		/*
		transVerticesPositions[i].x = w/2 * transVerticesPositions[i].x + w/2;
		*/
	}

	
	

	//now draw the triangles (and always before put them in vec2) !!!
	glm::vec2 a(0.0f, 0.0f), b(0.0f, 0.0f), c(0.0f, 0.0f);
	for (int face = 0; face < size - 2; face = face + 3)
	{
		
		a.x = transVerticesPositions[face].x;
		a.y = transVerticesPositions[face].y;

		b.x = transVerticesPositions[face + 1].x;
		b.y = transVerticesPositions[face + 1].y;

		c.x = transVerticesPositions[face + 2].x;
		c.y = transVerticesPositions[face + 2].y;

		//draw triangle [a,b,c]
		this->drawLine(a, b, color);
		this->drawLine(b, c, color);
		this->drawLine(c, a, color);
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
				drawLine(bounds[(i) % 4], bounds[(i + 1) % 4], color);
			}
			//draw a'-b'-c'-d'
			for (int i = 4; i < 8; i++)
			{
				drawLine(bounds[i % 4 + 4], bounds[(i + 1) % 4 + 4], color);
			}
			//draw a-a' b-b' c-c' d-d'
			for (int i = 0; i < 4; i++)
			{
				drawLine(bounds[i], bounds[i + 4], color);
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
			b = glm::normalize(b);
			sizeNormals = 40;
			b.x = sizeNormals * b.x;
			b.y = sizeNormals * b.y;

			//let him start from the point he's normal to
			b.x = b.x + a.x;
			b.y = b.y + a.y;

			drawLine(a, b, glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)); //faces' normals with GREEN
		}



		delete[] transNormalPositions;
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
		glm::vec2 a = glm::vec2(), b = glm::vec2();
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

			drawLine(a, b, glm::vec4(1.0f, 1.0f, 1.0f, 0.0f)); //faces' normals with WHITE
			int checking = 0;
		}
		int hello = 0;

		delete[] transAvg;
		delete[] transFaces;
	}

	
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

void Renderer::drawLine(glm::vec2 point1, glm::vec2 point2, glm::vec4 color)
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
				putPixel(x, y, color);
			else
				putPixel(y, x, color);
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
				putPixel(x, y, color);
			else
				putPixel(y, x, color);
			
			x = x + 1; e = e + 2*(q2 - q1);
		}

	}


}
void Renderer::printLineNaive()
{
	float m = 0.7f, b = 10.0f; //slope
	int x,y;
	int r = 5;
	glm::vec4 green = glm::vec4(0, 1, 0, 1);
	
	for (int x = 0; x < width; x++) // x goes from left corner to right corner
	{
		y = m * x + b;
		
		

		for (int r0 = 0; r0 < r; r0++) //so it wouldn't be thin
		{
			if (!((int)y < height && (int)y >= 0)) //if y is out of bounds, stop drawing
				break;
			if(x + r0 < width && x + r0 >= 0)
				putPixel(x + r0, (int)y, green);
			if (x + r0 < width && x - r0 >= 0)
				putPixel(x - r0, (int)y, green);
    }
  }
}

void Renderer::SetDemoBuffer()
{
	int r = 5;
	// Wide red vertical line
	glm::vec4 red = glm::vec4(1, 0, 0, 1);
	for (int i = 0; i<height; i++)
	{
		for (int r0 = 0; r0 < r; r0++)
		{
			putPixel((width / 2) + r0, i, red);
			putPixel((width / 2) - r0, i, red);
		}
	}
	// Wide magenta horizontal line
	glm::vec4 magenta = glm::vec4(1, 0, 1, 1);
	for (int i = 0; i<width; i++)
	{
		for (int r0 = 0; r0 < r; r0++)
		{
			putPixel(i, (height / 2) + r0, magenta);
			putPixel(i, (height / 2) - r0, magenta);
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
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			putPixel(i, j, color);
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
