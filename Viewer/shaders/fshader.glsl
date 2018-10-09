#version 330 core
 in vec3 norm;

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


vec3 CalcPointLight(int place)
{
    vec3 lightDir = normalize(pos_dir[place] -vec3(gl_PointCoord.x,gl_PointCoord.y,gl_FragCoord.z));
  
    float diff = max(dot(norm, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(view_dir, reflectDir), 0.0),exp);
    // attenuation
    //float distance    = length( pos_dir[place] - vec3(gl_PointCoord.x,gl_PointCoord.y,gl_FragCoord.z));
    //float attenuation = 1.0 / (constant[place] + linear[place] * distance + 
  	//		     quadratic[place] * (distance * distance));
				
    // combine results
	vec3 ambient = vec3(0, 0, 0);
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	ambient = am_ligth[place] * am_color;
	diffuse = dif_ligth[place] * dif_color*diff;
	specular = spec_ligth[place] * spec_color * spec;

	
    //ambient  *= attenuation;
    //diffuse  *= attenuation;
    //specular *= attenuation;
    

	return (ambient + diffuse + specular);
}

vec3 CalcDirLight(int place)
{
	vec3 lightDir = pos_dir[place];
	// diffuse shading
	float diff = min(max(dot(norm, lightDir), 0.0),1.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = min(pow(max(dot(view_dir,reflectDir), 0.0), exp),1.0);
	// combine results
	vec3 ambient = vec3(0, 0, 0);
	vec3 diffuse = vec3(0, 0, 0);
	vec3 specular = vec3(0, 0, 0);

	ambient = am_ligth[place] * am_color;
	diffuse = dif_ligth[place] * dif_color*diff;
	specular = spec_ligth[place] * spec_color * spec;
	
	return (ambient + diffuse + specular);
}

void main() 
{ 
	//FragColor = norm; normal as color looks super cool 
	//simple testing

	vec4 transNorm = vec4(n, 0.0);
	transNorm = NormMatrix * transNorm; //transform the normal! >:(
	norm = vec3(transNorm.x, transNorm.y, transNorm.z);

	int i=0;
	if(! norm_as_color)
	{
		FragColor =vec4(0.0,0.0,0.0,1.0);
		for(i=0;i<active_ligths_arry_size;i++)
		{
		if(ligth_type[i])
		   FragColor=FragColor+vec4(CalcDirLight(i),0.0);
	    else
		  FragColor=FragColor+vec4(CalcPointLight(i),0.0);
		}
	}
	else
	{
		FragColor=vec4(norm,1);
	}

}


