/*
	Recebe a posicao do vertice e sua respectiva cor.
	Transforma o vertice e passa a cor para o FS
*/
#version 400 // versao minima da GLSL utilizada. Pode ser usada para invocar funcionalidades de versoes especificas da linguagem.

layout (location=0) in vec3 vcoord;
layout (location=1) in vec3 vnormal;

out vec3 fcoord;
out vec3 fnormal;

uniform mat4 MVP;
uniform mat4 MM;

void main()
{
	fcoord = (MM * vec4(vcoord, 1.0)).xyz;
	fnormal = vnormal;
	float scale = 1.0;
	gl_Position = MVP * vec4(vcoord * scale, 1.0);
}
