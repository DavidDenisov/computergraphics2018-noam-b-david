#version 330 core

out vec4 FragColor;
in vec3 vertexColor;
in vec3 vertexPos;


void main() 
{ 
	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); //simple testing
	FragColor = vec4(vertexColor, 1.0f); // in/out testing (from the vshader)

	FragColor = vec4(vertexPos, 1.0f); //exrecise
}
