#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
in vec3 LightPos;  //defined in vsh
//uniform vec3 viewPos;  should be 0 0 0 now

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	float specularStrength = 0.5f;
	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;
	
	vec3 result = (ambient + diffuse + specular) * objectColor;
	color = vec4(result, 1.0f);
	//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}

