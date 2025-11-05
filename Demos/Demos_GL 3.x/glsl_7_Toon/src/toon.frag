// simple toon fragment shader
// www.lighthouse3d.com

#version 130

varying vec3 normal, lightDir;

void main()
{
	float intensity;
	vec4 color;

	intensity = (clamp(max(dot(lightDir,normal),0.0), 0.4, 0.6) - 0.4) * 4.0 + 0.2; 

	color = vec4(vec3(0.8, 0.4, 0.4) * intensity, 1.0);
		
	gl_FragColor = color;
}
