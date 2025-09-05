/*
 * GlutSolids.h
 * arquivo com os solidos substitutos
 */

#ifndef GLUT_SOLIDS
#define GLUT_SOLIDS

/*
 * AVISO:
 * para o comando de renderizacao escalar o modelo, eh necessario que o shader
 * ativo possua uma variavel uniforme "scale"
 */

/* inicializa os solidos da glut (teapot e icosaedro) */
void glutSolidsInit();
/* renderiza o teapot da glut*/
void glutSolidTeapot(float scale);
/* renderiza o icosaedro da glut */
void glutSolidIcosahedron(float scale);
/* renderiza o icosaedro com smooth shading (nao presente na glut) */
void glutSolidIcoSmooth(float scale);

#endif