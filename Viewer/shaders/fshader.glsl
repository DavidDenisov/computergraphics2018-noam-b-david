#version 330 core
in vec3 norm;
in vec3 pos;
in vec3 LightingColor;
in vec2 TexCoords;
uniform int phong;
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
uniform vec3 view_dir;
//uniform int constant[15];
//uniform int linear[15];
//uniform int quadratic[15];
uniform bool auto_texture;
uniform bool norm_as_color;

uniform sampler2D ourTexture;

vec3 CalcDirLight(int place,vec3 ligt_direction)
{
	vec3 lightDir = ligt_direction;
	// diffuse shading
	float diff = min(max(dot(norm, lightDir), 0.0),1.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = min(pow(max(dot(view_dir,reflectDir), 0.0), exp),1.0);
	// combine results
	vec3 diffuse,specular;
	diffuse=vec3(0,0,0);

	
	if(auto_texture)
		{
			
		}
	else
		{
			diffuse = dif_ligth[place] * dif_color*diff;
			specular = spec_ligth[place] * spec_color * spec;
		}
	return (diffuse + specular);
}


vec3 CalcPointLight(int place)
{
    vec3 lightDir = normalize(pos_dir[place] -vec3(pos));
    return  CalcDirLight(place,lightDir);
}


void main() 
{ 
		gl_FragColor = texture(ourTexture, TexCoords);
}


