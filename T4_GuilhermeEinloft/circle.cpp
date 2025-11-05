#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <stdio.h>

#include "b_spline.h"
#include "sp_hash.h"
#include "collision.h"
#include "circle.h"

static GLuint circles_VAO_ID;
struct circle circles[CIRC_NUM];
int circ_num = 0;

float circ_verts[] = {
	 CIRC_R,  CIRC_R, 0.0f,
	 CIRC_R, -CIRC_R, 0.0f,
	-CIRC_R, -CIRC_R, 0.0f,
	-CIRC_R,  CIRC_R, 0.0f
};

int circ_indices[] = {0, 1, 3, 1, 2, 3};

/*
 * função auxiliar para ver se o círculo colide com a b-spline
 * verifica todos os segmentos, pois a curva não está na hash
 */
bool circles_check_b_spline_collision(int i)
{
	for (int j = 1; j < B_SEG; j++) {
		if (collision_circle_line(circles[i].center, CIRC_R,
				b_spline[j - 1], b_spline[j]))
			return 1;
	}
	return 0;
}

/*
 * função auxiliar para ver se o círculo colide com algum outro círculo
 * verifica todos os círculos, sem uso da tabela hash
 */
bool circles_check_all_collisions(int i)
{
	for (int j = 0; j < i; j++) {
		if (collision_circle_circle(circles[i].center, CIRC_R,
				circles[j].center, CIRC_R))
			return 1;
	}
	return 0;
}

/*
 * função auxiliar para ver se o círculo colide com os círculos
 * presentes em uma célula do grid
 */
bool circles_check_cell_collisions(int i, int index)
{
	for (int j = initial_index[index]; j < final_index[index]; j++) {
		if (collision_circle_circle(circles[i].center, CIRC_R,
				circles[hash_table[j]].center, CIRC_R))
			return 1;
	}
	return 0;
}

/*
 * função auxiliar para ver se o círculo colide com algum outro círculo
 * faz uso da função hash espacial
 * verifica apenas os círculos da mesma célula e 3 células vizinhas
 */
bool circles_check_hash_collisions(int i)
{
	int scale_x = SCREEN_W / GRID_W; 
	int scale_y = SCREEN_H / GRID_H;
	int init_x = 0, init_y = 0;
	
	int dir = (int)hash_direction(circles[i].center);
	if (!(dir & 2))
		init_x = -scale_x;
	if (!(dir & 1))
		init_y = -scale_y;

	for (int x = init_x; x <= init_x + scale_x; x += scale_x) {
		for (int y = init_y; y <= init_y + scale_y; y += scale_y) {
			int index = hash_function(circles[i].center +
					glm::vec3((float)x, (float)y, 0.0f));
			if (circles_check_cell_collisions(i, index))
				return 1;
		}
	}
	/* versão anterior, que verificava as 8 células vizinhas */
	/*
	for (int x = -scale_x; x <= scale_x; x += scale_x) {
		for (int y = -scale_y; y <= scale_y; y += scale_y) {
			int index = hash_function(circles[i].center +
					glm::vec3((float)x, (float)y, 0.0f));
			if (circles_check_cell_collisions(i, index))
				return 1;
		}
	}
	*/
	return 0;
}

void circles_init()
{
	srand(0);
	for (int i = 0; i < CIRC_NUM; i++) {
		do {
			float x = rand() % SCREEN_W;
			float y = rand() % SCREEN_H;
			circles[i].center = glm::vec3(x, y, 0.0f);
			circles[i].trans = glm::mat4(1.0f);
			circles[i].trans = glm::translate(circles[i].trans,
					circles[i].center);
		} while (circles_check_b_spline_collision(i) ||
				circles_check_all_collisions(i));
		circ_num++;
	}
}

void circles_init_hash()
{
	srand(0);
	for (int i = 0; i < CIRC_NUM; i++) {
		do {
			float x = rand() % SCREEN_W;
			float y = rand() % SCREEN_H;
			circles[i].center = glm::vec3(x, y, 0.0f);
			circles[i].trans = glm::mat4(1.0f);
			circles[i].trans = glm::translate(circles[i].trans,
					circles[i].center);
		} while (circles_check_b_spline_collision(i) ||
				circles_check_hash_collisions(i));
		circ_num++;
		spatial_hashing_update();
	}
}

void circles_init_VAO()
{
	glGenVertexArrays(1, &circles_VAO_ID);
	glBindVertexArray(circles_VAO_ID);

	unsigned int handle[2];
	glGenBuffers(2, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float),
			(GLvoid*)&circ_verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0,
			(GLubyte*)NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int),
			(GLvoid*)&circ_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void circles_render(GLuint transLoc)
{
	glBindVertexArray(circles_VAO_ID);
	for (int i = 0; i < CIRC_NUM; i++) {
		glUniformMatrix4fv(transLoc, 1, GL_FALSE,
				glm::value_ptr(circles[i].trans));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
				(GLubyte*)NULL);
	}
	glBindVertexArray(0);
}
