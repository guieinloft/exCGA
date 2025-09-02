/**
*   Programa para ilustrar:
*   - Acesso a tex2 em GLSL
*   - comando glActiveTexture()
*
*
*   Autor/Adaptacao: Cesar Tadeu Pozzer
*   UFSM - 09/2007
*
*   pozzer@inf.ufsm.br
*   pozzer3@gmail.com
*
**/

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "Bmp.h"
#include "glsl.h"

#define RECT_SIZE 10

//variaveis uniform
GLint loc_u_texture_0;  //local da variavel texture do arquivo tex.frag
GLint loc_u_texture_1;  //local da variavel texture do arquivo tex.frag
GLint loc_u_alpha;     //local da variavel alpha do arquivo tex.frag
GLint loc_u_mouse;  //local da variavel texture do arquivo tex.frag
GLint loc_u_size;  //local da variavel texture do arquivo tex.frag

int size = 0;
float zoom = 0.001;
float mouse_pos[2];

float alpha = 0;

Bmp *img1, *img2;
unsigned char *data1, *data2;
GLuint tex1, tex2;

float ang = 0;
int screenH;

Glsl *shader1;

void init_gl(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);

   glShadeModel (GL_SMOOTH);
   //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
   glEnable(GL_BLEND);

   img1 = new Bmp("glsl_5_Texture/resource/tex1.bmp");
   img1->convertBGRtoRGB();
   data1 = img1->getImage();
   if( data1 == NULL )
   {
       printf("erro");
	   exit(0);
   }
   img2 = new Bmp("glsl_5_Texture/resource/tex2.bmp");
   img2->convertBGRtoRGB();
   data2 = img2->getImage();
   if( data2 == NULL )
   {
       printf("erro");
	   exit(0);
   }
   //glEnable(GL_TEXTURE);
   //glEnable(GL_TEXTURE_2D);

   glGenTextures( 1, &tex1 );
   glBindTexture( GL_TEXTURE_2D, tex1 );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img1->getWidth(),img1->getHeight(), 0,GL_RGB, GL_UNSIGNED_BYTE, data1);

   glGenTextures( 1, &tex2 );
   glBindTexture( GL_TEXTURE_2D, tex2 );
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img2->getWidth(),img2->getHeight(), 0,GL_RGB, GL_UNSIGNED_BYTE, data2);


   //glActiveTexture selects which texture unit subsequent texture state calls will
   //affect.  The number of texture units an implementation supports is
   //implementation dependent, but must be at least 2.
   glActiveTexture(GL_TEXTURE0);
   glEnable(GL_TEXTURE_2D);
   glBindTexture( GL_TEXTURE_2D, tex1 );

   glActiveTexture(GL_TEXTURE1);
   glEnable(GL_TEXTURE_2D);
   glBindTexture( GL_TEXTURE_2D, tex2 );


}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLoadIdentity();

   //muda a intensidade da cor de cada pixel por uma senoide no intervalo [0.2, 4.2]

   glUniform1f(loc_u_alpha, alpha += 0.05);
   glUniform1i(loc_u_texture_0, 0);
   glUniform1i(loc_u_texture_1, 1);
   glUniform2f(loc_u_mouse, mouse_pos[0], mouse_pos[1]);
   glUniform1f(loc_u_size, (float)size);

   glNormal3f(0, 1, 0);
   glBegin(GL_QUADS);

      glTexCoord2f(0, 0);
      glVertex3f(-2, -2,  -10);

      glTexCoord2f(1, 0);
	  glVertex3f(2, -2,   -10);

      glTexCoord2f(1, 1);
      glVertex3f(2, 2,  -10);

      glTexCoord2f(0, 1);
      glVertex3f(-2, 2, -10);
   glEnd();

   glutSwapBuffers();
  // printf("x");
}

void reshape (int w, int h)
{
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(30, 1, 1, 100);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   screenH = h;
}

void keyboard(unsigned char key, int, int) {
	if (key >= '0' && key <= '9')
		size = (key - '0') * 4;
}

void mouse(int x, int y) {
	mouse_pos[0] = (float)x;
	mouse_pos[1] = (float)(screenH - y);
}

void mouseclick(int button, int state, int, int) {
	if (state == GLUT_DOWN) {
		if (button == 4)
			size += 2;
		else if (button == 3)
			size -= 2 * (size > 0);
	}
	printf("%d\n", size);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (600, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow("Shader 1");
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutPassiveMotionFunc(mouse);
   glutMouseFunc(mouseclick);

   //neste caso, o glsl deve ser inicializado antes do gl para poder chamar o
   //comando  glActiveTexture();
   shader1 = new Glsl("glsl_5_Texture/src/tex.vert", "glsl_5_Texture/src/tex.frag");
   shader1->setActive(true);
   init_gl();

   loc_u_texture_0 = shader1->getUniformLoc("texture_0");
   loc_u_texture_1 = shader1->getUniformLoc("texture_1");
   loc_u_alpha    = shader1->getUniformLoc("alpha");
   loc_u_mouse = shader1->getUniformLoc("mouse");
   loc_u_size = shader1->getUniformLoc("size");

   //printf(" IDs: %d %d ", loc_u_texture, loc_u_alpha);

   glutMainLoop();
   return 0;
}
