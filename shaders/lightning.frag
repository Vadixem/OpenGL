#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};  

struct Light {
    vec3 position;
    vec3 direction;
    float iCutOff;
    float oCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
}; 

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

float getFlashLightIntensity(float angle, float innerCut, float outerCut)
{
	if (angle < innerCut) return 1.0f;
	else if ((angle < outerCut) && (angle > innerCut))
	return (outerCut - angle)/outerCut;
	else return 0.0f;
}

void main()
{
	
        // Ambient
        vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
        
        // Diffuse 
        vec3 norm = normalize(Normal);        
        // The normalized vector from light position to rendered fragment
    	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
	
        // Specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
        
        // Spotlight soft edges
    	float theta = dot(lightDir, normalize(-light.direction)); 
	float epsilon = (light.iCutOff - light.oCutOff);
	float intensity = clamp ((theta - light.oCutOff) / epsilon, 0.0, 1.0);    
	diffuse *= intensity;
	specular *= intensity;
                      
	// Attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation;  // Also remove attenuation from ambient, because if we move too far, the light in spotlight would then be darker than outside (since outside spotlight we have ambient lighting).
        diffuse  *= attenuation;
        specular *= attenuation;   
        color = vec4(ambient + diffuse + specular, 1.0f);  
} 