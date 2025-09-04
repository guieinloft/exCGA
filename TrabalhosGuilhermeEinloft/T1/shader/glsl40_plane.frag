/*
	Simplesmente aplica a cor recebida.
	Note que ela foi interpolada entre o VS e FS.
*/

#version 400

in vec3 fcoord;
in vec3 fnormal;
//uniform vec3 color;
uniform vec3 light_pos;
uniform vec3 camera_pos;
//uniform vec4 consts; //[0] = k_ambient, [1] = k_diffuse, [2] = k_specular, [3] = shininess

void main() 
{
	vec3 normal = normalize(fnormal);
	vec3 light_dir = normalize(light_pos - fcoord);
	vec3 camera_dir = normalize(camera_pos - fcoord);
	vec3 halfway_dir = normalize(light_dir + camera_dir);

	vec4 consts = vec4(0, 0.5, 0.5, 100);

	/* ambient */
	float i_ambient = consts.x;
    
	/* diffuse */
	float i_diffuse = consts.y * max(dot(normal, light_dir), 0.0);

	/* specular */
	float i_specular = consts.z * pow(max(dot(normal, halfway_dir), 0.0), consts.w);
	
	gl_FragColor = vec4(i_ambient + i_diffuse + i_specular); //GL 4.0
}
