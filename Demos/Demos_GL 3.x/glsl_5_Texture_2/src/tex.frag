//Autor: Cesar Tadeu Pozzer

#version 130

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float     alpha;
varying float  x_coord;
varying float  y_coord;

void main()
{
   vec2 pos1 = gl_TexCoord[0].st * 0.9 + alpha;
   float disp = (texture2D(texture_1, pos1).r * 2 - 1) * 0.1;
   vec2 pos0 = gl_TexCoord[0].st + disp;
   vec3 cor = texture2D(texture_0, pos0).rgb;
   
   //gl_FragColor = texture2D(texture, position).rgba;
   gl_FragColor = vec4(cor, 1);
}
