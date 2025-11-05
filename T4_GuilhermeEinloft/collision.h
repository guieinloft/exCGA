/*
 * collision.h
 * funções de verificação de colisão entre objetos
 */

#ifndef COLLISION_H
#define COLLISION_H

/* colisão círculo-linha, para verificar colisões com a b-spline */
bool collision_circle_line(glm::vec3 c_cnt, float c_rad,
		glm::vec3 l_p1, glm::vec3 l_p2);
/* colisão círculo-círculo, para verificar colisões com os outros círculos */
bool collision_circle_circle(glm::vec3 c1_cnt, float c1_rad,
		glm::vec3 c2_cnt, float c2_rad);

#endif
