#include <glm/glm.hpp>

bool collision_circle_line(glm::vec3 c_cnt, float c_rad,
		glm::vec3 l_p1, glm::vec3 l_p2)
{
	glm::vec3 d = l_p2 - l_p1;
	glm::vec3 f = l_p1 - c_cnt;
	float a = glm::dot(d, d);
	float b = 2 * glm::dot(f, d);
	float c = glm::dot(f, f) - c_rad * c_rad;
	
	float delta = b * b - 4 * a * c;
	if (delta < 0)
		return 0;

	delta = sqrt(delta);

	float t1 = (-b - delta) / (2 * a);
	float t2 = (-b + delta) / (2 * a);

	if (t1 >= 0 && t1 <= 1)
		return 1;
	if (t2 >= 0 && t2 <= 1)
		return 1;
	return 0;
}

bool collision_circle_circle(glm::vec3 c1_cnt, float c1_rad,
		glm::vec3 c2_cnt, float c2_rad)
{
	return (glm::distance(c1_cnt, c2_cnt) > c1_rad + c2_rad) ? 0 : 1;
}
