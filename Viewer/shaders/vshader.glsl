#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 n;


uniform mat4 transformPos;
uniform mat4 transformNorm;


//uniform bool auto_textur;
out vec4 norm;


void main()
{
    gl_Position = aPos;

	gl_Position = transformPos * gl_Position;
	gl_Position = gl_Position / gl_Position.w; // now he's NDC!

	norm = n;

}
