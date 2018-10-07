#version 330 core
 in vec4 norm;

uniform vec3 am_color;
uniform vec3 dif_color;
uniform vec3 spec_color;

uniform vec3 am_ligth[15];
uniform vec3 dif_ligth[15];
uniform vec3 spec_ligth[15];
uniform vec3 exp[15];
uniform int active_ligths_arry_size;
uniform vec3 pos_dir[15];
uniform vec3 view_dir;
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
		
	}
	

}
vec3 CalcDirLight(int place)
{
	vec3 lightDir = normalize(pos_dir[place]);
	// diffuse shading
	float diff = max(dot(norm, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), light.Specularity_exponent);
	// combine results
	vec3 ambient = glm::vec3(0, 0, 0);
	vec3 diffuse = glm::vec3(0, 0, 0);
	vec3 specular = glm::vec3(0, 0, 0);
	ambient = am * am_c;
	diffuse = light.difus*light.strengte_difus*diff *dif_c;
	specular = light.specalar*light.strengte_ambient * spec*spec_c;
	
	return (ambient + diffuse + specular);
}