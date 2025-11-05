#version 400
in vec2 pos;
out vec4 FragColor;
uniform float circ_r;

void main()
{
	if (length(pos) >= circ_r)
		discard;
	FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}

