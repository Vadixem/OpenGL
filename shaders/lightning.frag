#version 330 core
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	// Ambient
	vec3 ambient = lightColor * material.ambient;
	
	// Distance
	vec3 distanceVec = lightPos - FragPos; 
	float distance = sqrt(pow(distanceVec.x, 2) + pow(distanceVec.y, 2) + pow(distanceVec.z, 2)); 
	float maxim = 25.f;
	float intencity = max((maxim - distance)/maxim, 0.0001f);
		

	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = intencity * lightColor * (diff * material.diffuse);
	
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = lightColor * (spec * material.specular);	

	vec3 result = (ambient + diffuse + specular) * objectColor;	
	color = vec4(result, 1.0f);

}