#version 400
in vec2 TexCoord;
out vec4 FragColor;

uniform float scale;
uniform float aspect;
uniform vec2 center;
void main()
{
	int iter = 512;
	vec2 z, c, z2;
	c.x = aspect * scale * (TexCoord.x - 0.5) - center.x;
	c.y = scale * (TexCoord.y - 0.5) - center.y;

	int i;
	z = vec2(0);
	z2 = vec2(0);
	for (i = 0; i < iter; i++) {
		z2.x = (z.x * z.x - z.y * z.y) + c.x;
		z2.y = (z.x + z.x) * z.y + c.y;
		if (z2.x * z2.x + z2.y * z2.y > 4.0)
			break;
		z = z2;
	}
	
	float c_temp = (i == iter) ? 0.0 : float(i) / float(iter);
	FragColor = vec4(c_temp * 2.0 - 1.0, c_temp, c_temp * 2.0, 1.0);
}

