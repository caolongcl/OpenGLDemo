#version 330 core

in vec2 texCoord;

uniform sampler2D sampler;

void main()
{
	gl_FragColor = textureLod(sampler, texCoord, 2);
}