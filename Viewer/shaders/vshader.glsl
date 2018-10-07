#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec4 n;


uniform mat4 transformPos;
uniform mat4 transformNorm;

uniform vec3 am_color;
uniform vec3 dif_color;
uniform vec3 spec_color;

uniform vec3 am_ligth[15];
uniform vec3 dif_ligth[15];
uniform vec3 spec_ligth[15];
uniform int active_ligths_arry_size;
uniform vec3 exp[15];
uniform vec3 pos_dir[15];
uniform vec3 view_dir;
//uniform bool auto_textur;
out vec4 norm;


void main()
{
    gl_Position = aPos;

	gl_Position = transformPos * gl_Position;
	gl_Position = gl_Position / gl_Position.w; // now he's NDC!

	norm = n;

}
