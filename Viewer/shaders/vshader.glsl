#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 transform;



void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);

	gl_Position = transform * gl_Position;
	gl_Position = gl_Position / gl_Position.w; // now he's NDC!
}
