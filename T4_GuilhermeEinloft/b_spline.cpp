#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <math.h>
#include "b_spline.h"

glm::vec3 points[B_POINTS];
glm::vec3 b_spline[B_SEG];
glm::vec3 b_colors[B_SEG];
GLuint b_spline_VAO;

glm::vec3 b_spline_point(float t, int i)
{
	glm::vec3 res = glm::vec3(0.0f);
	res = res + points[(i + 0) % B_POINTS] * (float)(pow(1 - t, 3) / 6);
	res = res + points[(i + 1) % B_POINTS] * (float)((3 * pow(t, 3) - 6 * pow(t, 2) + 4) / 6);
	res = res + points[(i + 2) % B_POINTS] * (float)((-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) / 6);
	res = res + points[(i + 3) % B_POINTS] * (float)(t * t * t / 6);
	return res;
}

void b_spline_generate()
{
	for (int i = 0; i < B_SEG; i++) {
		float jt = (float)i * (B_POINTS - 3) / B_SEG;
		int j = (int)jt;
		float t = fmod(jt, 1.f);
		b_spline[i] = b_spline_point(t, j);
	}
}

void b_spline_generate_points()
{
	for (int i = 0; i < B_POINTS; i++) {
		float x = rand() % SCREEN_W;
		float y = rand() % SCREEN_H;
		points[i] = glm::vec3(x, y, 0.0f);
	}
}

void b_spline_generate_VAO()
{
	glGenVertexArrays(1, &b_spline_VAO);
	glBindVertexArray(b_spline_VAO);

	unsigned int handle;
	glGenBuffers(1, &handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, B_SEG * 3 * sizeof(float),
			(GLvoid*)&b_spline[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0,
			(GLubyte*)NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void b_spline_draw()
{
	glBindVertexArray(b_spline_VAO);
	glDrawArrays(GL_LINE_STRIP, 0, B_SEG);
	//glDrawArrays(GL_LINES, 0, B_SEG);
	glBindVertexArray(0);
}
