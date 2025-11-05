/*
 * b_spline.h
 * contém as funções específicas para geração da curva B-Spline
 */

#ifndef B_SPLINE_H
#define B_SPLINE_H

/* numero de segmentos para aproximar a curva */
#define B_SEG 4096
/* numero de pontos de controle */
#define B_POINTS 200
/* dimensoes da tela */
#define SCREEN_W 1280
#define SCREEN_H 720

/*
 * OBS:
 * os arrays são de vec3 pois é mais fácil de lidar com eles nos VAOs.
 * vale para os círculos também
 */

/* array de pontos de controle */
extern glm::vec3 points[B_POINTS];
/* array de segmentos aproximados da curva */
extern glm::vec3 b_spline[B_SEG];

/* gera os pontos de controle aleatoriamente */
void b_spline_generate_points();
/* avalia a curva e gera uma aproximação */
void b_spline_generate();
/* gera um VAO a partir dos segmentos aproximados */
void b_spline_generate_VAO();
/* renderiza o VAO na tela */
void b_spline_draw();

#endif
