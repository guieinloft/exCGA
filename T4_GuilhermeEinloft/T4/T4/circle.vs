#version 330 core
layout (location = 0) in vec3 aPos;

out vec2 pos;

uniform mat4 proj;
uniform mat4 trans;

void main()
{
	pos = aPos.xy;
	gl_Position = proj * trans * vec4(aPos, 1.0);
}
