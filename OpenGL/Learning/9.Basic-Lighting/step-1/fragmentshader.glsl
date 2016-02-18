#version 330 core

//in vec3 ourColor;
//in vec2 TexCoord;
out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;
//uniform sampler2D ourTexture1;
//uniform sampler2D ourTexture2;

void main()
{
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;
	vec3 result = ambient * objectColor;
	color = vec4(result, 1.0f);
	//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}

