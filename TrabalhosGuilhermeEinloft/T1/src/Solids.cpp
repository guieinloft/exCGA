#include "Solids.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "GlutSolids.h"

using namespace std;

Solids::Solids(GLFWwindow* window, int size){
	this->size = size;
	this->window = window;
	this->scene = 0;
	this->rot_vec = vec3(0, 0, 0);
	this->color_amb = vec3(1.0f, 1.0f, 1.0f);
	this->color_diff = vec3(1.0f, 1.0f, 1.0f);
	this->color_spec = vec3(1.0f, 1.0f, 1.0f);
}

void Solids::init(){

	// init matrices
	modelMatrix = glm::mat4(1.0f);
	viewMatrix = glm::lookAt(
		vec3(0.0f, 0.0f, 0.0f), //eye
		vec3(0.0f, 0.0f, -1.0f), //center
		vec3(0.0f, 1.0f, 0.0f)); //up
	projectionMatrix = glm::perspective(glm::radians(60.0f), 1.0f, 0.1f, 100.0f);

	// load shaders
	try {
		shader.compileShader("shader/t1.vert");
		shader.compileShader("shader/t1.frag");

		shader.link();
		shader.use();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		system("pause");
		exit(EXIT_FAILURE);
	}
	shader.printActiveAttribs();
}

void Solids::define_scene()
{
	// define the scene
	if (glfwGetKey(window, GLFW_KEY_2)) {
		scene = 1;
		color_amb = vec3(1.0f, 0.0f, 0.0f);
		color_diff = vec3(1.0f, 0.0f, 0.0f);
		color_spec = vec3(1.0f, 1.0f, 1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_3)) {
		scene = 2;
		color_amb = vec3(0.0f, 1.0f, 0.0f);
		color_diff = vec3(0.0f, 1.0f, 0.0f);
		color_spec = vec3(1.0f, 1.0f, 1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_4)) {
		scene = 3;
		color_amb = vec3(0.0f, 0.0f, 1.0f);
		color_diff = vec3(0.0f, 0.0f, 1.0f);
		color_spec = vec3(1.0f, 1.0f, 1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_1)) {
		scene = 0;
		color_amb = vec3(1.0f, 1.0f, 1.0f);
		color_diff = vec3(1.0f, 1.0f, 1.0f);
		color_spec = vec3(1.0f, 1.0f, 1.0f);
	}
}

void Solids::update(double deltaTime){
	define_scene();

	// get mouse and light position
	double mousex, mousey;
	glfwGetCursorPos(window, &mousex, &mousey);
	light_pos.x = (float)((mousex - 400.0) / 100.0);
	light_pos.y = (float)((400.0 - mousey) / 100.0);
	light_pos.z = 0.0f;

	// matrices setup
	modelMatrix = mat4(1.0f); // identity
	modelMatrix = glm::translate(modelMatrix, vec3(0.0, 0.0, -5.0)); // translate back
	modelMatrix = glm::rotate(modelMatrix, glm::radians(light_pos.y), vec3(-1, 0, 0)); // rotaciona baseado na posicao da luz
	modelMatrix = glm::rotate(modelMatrix, glm::radians(light_pos.x), vec3(0, 1, 0));
	modelViewMatrix = viewMatrix * modelMatrix;
	modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

	// set variables on the shader
	shader.setUniform("MVP", modelViewProjectionMatrix); //ModelViewProjection
	shader.setUniform("MM", modelMatrix); //ModelMatrix (para calculo da iluminacao)
	shader.setUniform("camera_pos", vec3(0.0f, 0.0f, -1.0f)); //posicao da camera
	shader.setUniform("light_pos", light_pos); //posicao da luz
	shader.setUniform("color_amb", color_amb); //cor do objeto (ambiente)
	shader.setUniform("color_diff", color_diff); //cor do objeto (difusa)
	shader.setUniform("color_spec", color_spec); //cor do objeto (especular)
	shader.setUniform("consts", vec4(0.2, 0.8, 1.0, 100.0)); //constantes (descritas no shader)
	shader.setUniform("light_color", vec3(1.0f, 1.0f, 1.0f)); //cor da luz (sempre branca)
}

GLuint vaoID;

void Solids::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (scene) {
	case 0:
		/*
		 * exige um pouco mais de trabalho para mover os solidos 
		 * pois deve-se recalcular a MVP para cada objeto e reenviar ao shader
		 */
		modelMatrix = glm::translate(modelMatrix, vec3(-1, -1, 0));
		modelViewMatrix = viewMatrix * modelMatrix;
		modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
		shader.setUniform("MVP", modelViewProjectionMatrix);
		glutSolidTeapot(1.0);
		modelMatrix = glm::translate(modelMatrix, vec3(2, 2, 0));
		modelViewMatrix = viewMatrix * modelMatrix;
		modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;
		shader.setUniform("MVP", modelViewProjectionMatrix);
		glutSolidIcosahedron(1.0);
		break;
	case 1:
		glutSolidIcosahedron(2.0);
		break;
	case 2:
		glutSolidIcoSmooth(2.0);
		break;
	case 3:
		glutSolidTeapot(1.5);
		break;
	}
}

void Solids::resize(int x, int y) {

}
