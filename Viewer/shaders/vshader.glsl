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

<<<<<<< HEAD
	//vec4 norm2 = vec4(n,1)*transformNorm;
	//norm= vec3( norm2.x, norm2.y, norm2.z);
	vec4 norm2 = transformNorm*vec4(n,1);
	 norm2 =  norm2 / norm2.w;
	norm =  normalize(vec3(norm2));
=======
	

>>>>>>> d6a010f7af5419d83eac6708e5866d6c714f13bb
}
