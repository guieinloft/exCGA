/*
 * circle.h
 * contém as funções específicas de geração e posicionamento dos círculos
 * para hashing dos círculos, conferir "sp_hash.h"
 */

#ifndef CIRCLE_H
#define CIRCLE_H

/* raio do círculo */
#define CIRC_R 2.0f
/* número máximo de círculos */
#define CIRC_NUM 8192

/* struct que define o círculo */
struct circle {
	glm::vec3 center;
	glm::mat4 trans; /* para facilitar na hora de mandar pro shader */
};

/* array de círculos */
extern struct circle circles[CIRC_NUM];
/* número atual de círculos */
extern int circ_num;

/* inicializa os círculos sem geração da tabela hash */
void circles_init();
/* inicializa os círculos com geração da tabela hash */
void circles_init_hash();
/* inicializa o VAO do círculo */
void circles_init_VAO();
/* renderiza os círculos na tela */
void circles_render(unsigned int transLoc);

#endif
