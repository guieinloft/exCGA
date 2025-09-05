/*
	calcula a intensidade da iluminação Blinn-Phong para o objeto
	depois, aplica ao objeto
*/

#version 400

in vec3 fcoord; // coordenada do fragmento
in vec3 fnormal; // normal do fragmento
uniform vec3 color_amb;
uniform vec3 color_diff;
uniform vec3 color_spec;
uniform vec4 consts; //[0] = k_ambient, [1] = k_diffuse, [2] = k_specular, [3] = shininess
uniform vec3 light_pos;
uniform vec3 light_color;
uniform vec3 camera_pos;

void main() 
{
	vec3 normal = normalize(fnormal); // normal do fragmento normalizada
	vec3 light_dir = normalize(light_pos - fcoord);
	vec3 camera_dir = normalize(camera_pos - fcoord);
	vec3 halfway_dir = normalize(light_dir + camera_dir);

	/* ambiente */
	float i_ambient = consts.x;
	vec3 c_ambient = i_ambient * light_color;
    
	/* difusa */
	float i_diffuse = consts.y * max(dot(normal, light_dir), 0.0);
	vec3 c_diffuse = i_diffuse * light_color;

	/* specular */
	float i_specular = consts.z * pow(max(dot(normal, halfway_dir), 0.0), consts.w);
	vec3 c_specular = i_specular * light_color;
	
	gl_FragColor = vec4(c_ambient * color_amb + c_diffuse * color_diff + c_specular * color_spec, 1.0); //GL 4.0
}
