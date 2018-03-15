#version 330 core

layout(location = 0) in vec3 i_position;
layout(location = 1) in vec4 i_color;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(i_position, 1.0f);
	color = i_color;
}