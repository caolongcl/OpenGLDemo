#version 330 core

in vec3 vertexColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D texture2d;
uniform sampler2D texture2d1;

uniform float mixWeight;

void main()
{
	//color = texture(texture2d, texCoord) * vec4(vertexColor, 0.0f);
	color = mix(texture(texture2d, texCoord), texture(texture2d1, vec2(texCoord.s, 1.0f-texCoord.t)), mixWeight);
}