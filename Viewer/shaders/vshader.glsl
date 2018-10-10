#version 330 core
layout (location = 0) in vec4 aPos;
layout (location = 1) in vec3 n;
layout (location = 2) in vec2 aTexCoords;;

uniform mat4 transformPos;
uniform mat4 transformNorm;
uniform int phong;//  0= groung (+-), 1= phong

uniform vec3 am_color;
uniform vec3 dif_color;
uniform vec3 spec_color;

uniform vec3 am_ligth;
uniform vec3 dif_ligth[15];
uniform vec3 spec_ligth[15];
uniform int exp;
uniform int active_ligths_arry_size;
uniform vec3 pos_dir[15];
uniform bool ligth_type[15];
uniform bool auto_textur;
uniform vec3 view_dir;
uniform bool norm_as_color;

out vec3 norm;
out vec3 pos;
out vec3 LightingColor;
out vec2 TexCoords;
vec3 CalcDirLight(int place,vec3 ligt_direction,vec3 norm_v)
{
	vec3 lightDir = ligt_direction;
	// diffuse shading
	float diff = min(max(dot(norm_v, lightDir), 0.0),1.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, norm_v);
	float spec = min(pow(max(dot(view_dir,reflectDir), 0.0), exp),1.0);
	// combine results

	vec3 diffuse = dif_ligth[place] * dif_color*diff;
	vec3 specular = spec_ligth[place] * spec_color * spec;
	return (diffuse + specular);
}


vec3 CalcPointLight(int place,vec3 norm_v)
{
    vec3 lightDir = normalize(pos_dir[place] -vec3(pos));
    return  CalcDirLight(place,lightDir,norm_v);
}

void main()
{
    gl_Position = aPos;

	gl_Position = transformPos * gl_Position;
	gl_Position = gl_Position / gl_Position.w; // now he's NDC!


	vec4 norm2 = transformNorm*vec4(n,0.0);
	norm =  normalize(vec3(norm2));
	pos = vec3(gl_Position);
	LightingColor=am_ligth * am_color;
	if(phong==0)
	{
		int i;
		if(!norm_as_color)
			for(i=0;i<active_ligths_arry_size;i++)
				{
					if(ligth_type[i])
						LightingColor=LightingColor+CalcDirLight(i,normalize(pos_dir[i]),norm);
					else
						LightingColor=LightingColor+CalcPointLight(i,norm);
				}
		else
			LightingColor=norm;
	}
	 TexCoords=aTexCoords;
}
