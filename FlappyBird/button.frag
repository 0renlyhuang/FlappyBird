#version 430 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D buttonTex;

void main()
{
	vec4 texColor = texture(buttonTex, TexCoord);
	if(texColor.a < 0.1)
        discard;
	color = texColor;
}
