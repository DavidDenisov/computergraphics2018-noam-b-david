#version 330 core
in vec3 norm123;
in vec3 pos;
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
//uniform bool auto_textur;
uniform bool norm_as_color;
out vec4 FragColor;

vec3 CalcDirLight(int place,vec3 ligt_direction)
{
	vec3 lightDir = ligt_direction;
	// diffuse shading
	float diff = min(max(dot(norm123, lightDir), 0.0),1.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, norm123);
	float spec = min(pow(max(dot(view_dir,reflectDir), 0.0), exp),1.0);
	// combine results

	vec3 ambient = am_ligth * am_color;
	vec3 diffuse = dif_ligth[place] * dif_color*diff;
	vec3 specular = spec_ligth[place] * spec_color * spec;
	return (ambient + diffuse + specular);
}


vec3 CalcPointLight(int place)
{
    vec3 lightDir = normalize(pos_dir[place] -vec3(pos));
    return  CalcDirLight(place,lightDir);
}


void main() 
{ 
	//FragColor = norm123; normal as color looks super cool 
	//simple testing
	int i=0;
	if(! norm_as_color)
	{
		FragColor =vec4(0.0,0.0,0.0,1.0);
		for(i=0;i<active_ligths_arry_size;i++)
		{
		if(ligth_type[i])
		   FragColor=FragColor+vec4( CalcDirLight(i,normalize(pos_dir[i])),0.0);
	    else
		  FragColor=FragColor+vec4(CalcPointLight(i),0.0);
		}
	}
	else
	{
		FragColor=vec4(norm123,1.0);
	}

}


