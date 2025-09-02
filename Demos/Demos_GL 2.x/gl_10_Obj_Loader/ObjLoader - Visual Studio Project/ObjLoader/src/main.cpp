/*******************************************************************
*	ObjLoader Example
*	
*	Descrição: Exemplo de utilização do ObjLoader. Neste exemplo é
*	utilizada a biblioteca GLFW para o gerenciamento da janela e das
*	entradas.
*
*	Autor: Alex Thomas Almeida Frasson
*
*	Universidade Federal de Santa Maria - 2012/1
*******************************************************************/

#include <glew.h>
#include <glfw.h>
#include <iostream>
#include "MyMesh.h"


#define WIDTH 1280
#define HEIGHT 720


MyMesh mesh;
float angle;			//Angulo de rotação do modelo.
float camZPos;			//Posição da camera no eixo z. (zoom)
int mouseX;				//Posição do mouse no eixo x.
bool mouseLeftPressed;	//Botão esquerdo do mouse, pressionado ou não.
bool running;



void render(void);
void mousePosCallback(int x, int y);
void mouseButtonCallback(int button, int state);
void mouseWheelCallBack(int pos);
void init(void);
void close(void);



int main(){

	init();

	while(running){

		render();

		running = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam(GLFW_OPENED);
	}

	close();

	return 0;
}

void render(void){
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	glPushMatrix();
		glTranslatef(0.0f, -1.0f, camZPos);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
		mesh.draw();//Desenha o modelo.
	glPopMatrix();


	glFlush();
	glfwSwapBuffers();
}

void init(void){

	//GLFW
	glfwInit();
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindow(WIDTH, HEIGHT, 8, 8, 8, 8, 24, 8, GLFW_WINDOW);

	glfwSetMousePosCallback(mousePosCallback);
	glfwSetMouseButtonCallback(mouseButtonCallback);
	glfwSetMouseWheelCallback(mouseWheelCallBack);
	glfwSetMouseWheel(-20);

	//OpenGL
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, WIDTH, HEIGHT);
	gluPerspective(54.0f, (GLfloat)WIDTH/(GLfloat)HEIGHT, 1.0f, 1000.0f);

	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat lightPosition[] = {0.0, 3.0, 5.0, 0.0};
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Variables
	running = true;
	angle = 0.0f;
	camZPos = -20.0f;
	//Carrega o modelo
	//mesh.load("resources\\mp40.obj");
	//mesh.load("resources\\magequads.obj");
	//mesh.load("resources\\AstonMartin.obj");
	mesh.load("resources\\knight.obj");
	//mesh.load("resources\\assassin.obj");
}

void mousePosCallback(int x, int y){
	if(mouseLeftPressed){
		angle -= (mouseX - x)*0.1f;
		if(angle < 0.0f){
			angle = 360.0f;
		}else if(angle > 360.0f){
			angle = 0.0f;
		}
	}
	mouseX = x;
}

void mouseButtonCallback(int button, int state){
	mouseLeftPressed = button == GLFW_MOUSE_BUTTON_LEFT && state == GLFW_PRESS;
}

void mouseWheelCallBack(int pos)
{
	if(pos < -1)
		camZPos = (float)-pos*(pos/40.0f);
	else
		glfwSetMouseWheel(-1);
}

void close(void){
	glfwTerminate();
}