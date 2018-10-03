#version 150

in  vec2 vTexCoord;

layout (location = 0) in vec3 aPos;



out vec2 texCoord;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    texCoord = vTexCoord;
}
