#version 330 core

in vec3 result;
out vec4 color;

void main()
{
	color = vec4(result, 1.0f);
	//color = mix(texture(ourTexture1, TexCoord), texture(ourTexture2, TexCoord), 0.2);
}

