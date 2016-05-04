#version 330 core
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 distanceVec = lightPos - FragPos; 
	float distance = sqrt(pow(distanceVec.x, 2) + pow(distanceVec.y, 2) + pow(distanceVec.z, 2)); 
	float maxim = 7.f;
	float intencity = max((maxim - distance)/maxim, ambientStrength);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * intencity * lightColor;
	vec3 result = (ambient + diffuse) * objectColor;	
	color = vec4(result, 1.0f);
}