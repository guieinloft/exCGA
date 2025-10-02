//Autor: Cesar Tadeu Pozzer

#version 130

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float     alpha;
varying float  x_coord;
varying float  y_coord;

void main()
{
	vec2 coord = gl_TexCoord[0].st * 2.0 - 1.0;
	float coord_dist = coord.x * coord.x + coord.y * coord.y;
	float cont_size = 0.05;
	if (coord_dist <= 1.0 && coord_dist >= 1.0 - cont_size)
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	else
		gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
