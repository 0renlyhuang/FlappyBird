#version 430 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
	// color = vec4(1.0f);
	color = texture(ourTexture, TexCoord);
}
