//Autor: Cesar Tadeu Pozzer

#version 130

uniform sampler2D texture_0; //se nao for inicializada, vale 0, referente a textura 0
uniform sampler2D texture_1; //se nao for inicializada, vale 0, referente a textura 0
uniform float     alpha;
uniform vec2 mouse;
uniform float size;
uniform float zoom;

void main()
{
   vec2 pos = gl_TexCoord[0].st;
   vec3 cor = vec3(0, 0, 0);
   vec2 mdist = mouse - gl_FragCoord.xy;
   if (mdist.x * mdist.x + mdist.y * mdist.y < (size * size))
	   cor = texture2D(texture_0, pos + mdist * log(size) * 0.0002).rgb;
   else
	   cor = texture2D(texture_0, pos).rgb;
   
   //gl_FragColor = texture2D(texture, position).rgba;
   gl_FragColor = vec4(cor, 1);
}
