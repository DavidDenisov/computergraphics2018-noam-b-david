#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 n;


uniform mat4 transformPos;
uniform mat4 transformNorm;

out vec3 norm;


void main()
{
    gl_Position = aPos;

	gl_Position = transformPos * gl_Position;
	gl_Position = gl_Position / gl_Position.w; // now he's NDC!

	//vec4 norm2 = vec4(n,1)*transformNorm;
	//norm= vec3( norm2.x, norm2.y, norm2.z);
	vec4 norm2 = transformNorm*vec4(n,0);
	 //snorm2 =  norm2 / norm2.w;
	norm =  normalize(vec3(norm2));

}
