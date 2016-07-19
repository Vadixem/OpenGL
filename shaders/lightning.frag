#version 330 core
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
uniform Material material;

struct Light
{
	vec3 position;
	/*vec3 direction;*/	

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float quadratic;
	float linear;
};
uniform Light light;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 color;

uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance +light.quadratic * (distance * distance));
	// Ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	ambient *= attenuation;
		
	// Diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	diffuse *= attenuation;
	
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));	
	specular *= attenuation;

	vec3 result = (ambient + specular + diffuse);	
	color = vec4(result, 1.0f);

}