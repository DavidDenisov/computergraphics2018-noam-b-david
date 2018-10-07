#version 330 core
 in vec4 norm;

uniform vec3 am_color;
uniform vec3 dif_color;
uniform vec3 spec_color;

uniform vec3 am_ligth[15];
uniform vec3 dif_ligth[15];
uniform vec3 spec_ligth[15];
uniform int exp;
uniform int active_ligths_arry_size;
uniform vec3 pos_dir[15];
uniform bool ligth_type[15];
uniform vec3 view_dir;
 uniform int constant[15];
 uniform int linear[15];
 uniform int quadratic[15];
//uniform bool auto_textur;

out vec4 FragColor;




void main() 
{ 
	//FragColor = norm; normal as color looks super cool 
	//simple testing
	int i=0;
	FragColor =vec4(0,0,0,1);
	for(i=0;i<active_ligths_arry_size;i++)
	{
		if(ligth_type[place])
		   FragColor=FragColor+CalcDirLight(place);
	    else
		  FragColor=FragColor+CalcPointLight(place);
	}

	

}
vec3 CalcDirLight(int place)
{
	vec3 lightDir = normalize(pos_dir[place]);
	// diffuse shading
	float diff = max(dot(norm, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(view_dir, reflectDir), 0.0), exp);
	// combine results
	vec3 ambient = glm::vec3(0, 0, 0);
	vec3 diffuse = glm::vec3(0, 0, 0);
	vec3 specular = glm::vec3(0, 0, 0);

	ambient = am_ligth[place] * am_color;
	diffuse = dif_ligth[place] * dif_color*diff;
	specular = spec_ligth[place] * spec_color * spec;
	
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(int place)
{
    vec3 lightDir = normalize(pos_dir[place] - fragPos);
  
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),exp);
    // attenuation
    float distance    = length( pos_dir[place] - fragPos);
    float attenuation = 1.0 / (constant[place] + linear[place] * distance + 
  			     quadratic[place] * (distance * distance));
				
    // combine results
	vec3 ambient = glm::vec3(0, 0, 0);
	vec3 diffuse = glm::vec3(0, 0, 0);
	vec3 specular = glm::vec3(0, 0, 0);

	ambient = am_ligth[place] * am_color;
	diffuse = dif_ligth[place] * dif_color*diff;
	specular = spec_ligth[place] * spec_color * spec;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}
