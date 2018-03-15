#version 330 core

layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 2) in vec2 i_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main()
{
	gl_Position = projection * view * model * vec4(i_position, 1.0f);
	uv = i_uv;
}