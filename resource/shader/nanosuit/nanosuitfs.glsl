#version 330 core

in vec2 uv;

uniform sampler2D diffuse1;
uniform sampler2D diffuse2;
uniform sampler2D specular1;

void main()
{
	gl_FragColor = texture(diffuse1, uv) + texture(specular1, uv) + texture(diffuse2, uv);
}
