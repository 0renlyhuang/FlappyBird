#version 430 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	vec4 texColor = texture(tex, TexCoord);
	if(texColor.a < 0.1)
        discard;
	color = texColor;
}