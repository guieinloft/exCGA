#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "b_spline.h"
#include "sp_hash.h"
#include "circle.h"

GLFWwindow *window;

/* ids dos shaders da b-spline e dos círculos, respectivamente */
GLuint shaderIDB;
GLuint shaderIDC;

/* localização da uniform proj no shader da b-spline */
GLuint projLocB;
/* localização das uniforms no shader dos círculos */
GLuint projLocC, transLocC, circRLocC;
/* matriz de projeção */
glm::mat4 proj;

std::chrono::steady_clock sclock;
std::chrono::steady_clock::time_point time1, time2;
float deltaTime = 0.0f;

float scale = 1.0;
float center[2] = {SCREEN_W/2, SCREEN_H/2};
bool button_held = 0;
float mouse_pos[2], mouse_pos_prev[2];

/* callback do scroll, aumenta e diminui a escala */
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		scale *= 1.2;
	else if (yoffset < 0)
		scale /= 1.2;
	
	proj = glm::ortho(center[0] - SCREEN_W * 0.5 / scale,
			center[0] + SCREEN_W * 0.5 / scale,
			center[1] - SCREEN_H * 0.5 / scale,
			center[1] + SCREEN_H * 0.5 / scale);
}

/* callback do botão do mouse, pra sinalizar se deve-se alterar o centro */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
		button_held = 1;
	else if (action == GLFW_RELEASE)
		button_held = 0;
}

/* callback da posição do mouse, pra alterar o centro */
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse_pos[0] = xpos;
	mouse_pos[1] = ypos;
	if (button_held) {
		center[0] -= (mouse_pos[0] - mouse_pos_prev[0]) / scale;
		center[1] += (mouse_pos[1] - mouse_pos_prev[1]) / scale;
		proj = glm::ortho(center[0] - SCREEN_W * 0.5 / scale,
				center[0] + SCREEN_W * 0.5 / scale,
				center[1] - SCREEN_H * 0.5 / scale,
				center[1] + SCREEN_H * 0.5 / scale);
	}
	mouse_pos_prev[0] = xpos;
	mouse_pos_prev[1] = ypos;
}

/* inicializa o GLFW e as callbacks */
void initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(SCREEN_W, SCREEN_H, "Spatial Hashing", NULL, NULL);
	if (!window) {
		printf("Erro ao inicializar janela do GLFW\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	
	/* callbacks */
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
}

/* inicializa o GLEW */
void initGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("Erro ao inicializar o GLEW\n");
		exit(-1);
	}
}

/* inicializa o shader (apenas Vertex e Fragment suportados) */
GLuint initShader(const char *vs, const char *fs)
{
	GLuint shaderID;
	/* vertex shader */
	std::ifstream vsfile(vs);
	std::stringstream vsstreambuffer;
	vsstreambuffer << vsfile.rdbuf();
	std::string vsbuffer = vsstreambuffer.str();
	const char *vscbuffer = vsbuffer.c_str();

	unsigned int vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &vscbuffer, NULL);
	glCompileShader(vsID);

	/* fragment shader */
	std::ifstream fsfile(fs);
	std::stringstream fsstreambuffer;
	fsstreambuffer << fsfile.rdbuf();
	std::string fsbuffer = fsstreambuffer.str();
	const char *fscbuffer = fsbuffer.c_str();

	unsigned int fsID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fsID, 1, &fscbuffer, NULL);
	glCompileShader(fsID);

	int success;
	char infoLog[512];
	glGetShaderiv(fsID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fsID, 512, NULL, infoLog);
		printf("%s\n", infoLog);
	}

	/* shader program */
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vsID);
	glAttachShader(shaderID, fsID);
	glLinkProgram(shaderID);

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	return shaderID;
}

/* inicializa (ou reinicializa) o delta time */
void initDeltaTime()
{
	time1 = std::chrono::steady_clock::now();
	time2 = std::chrono::steady_clock::now();
}

/* verifica o tempo percorrido desde a inicialização do delta time até o momento */
float getDeltaTime()
{
	time2 = std::chrono::steady_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() / 1000000.0f;
	return deltaTime;
}

int main()
{
	/* glfw */
	initGLFW();
	/* glew */
	initGLEW();
	/* gl */
	glViewport(0, 0, SCREEN_W, SCREEN_H);
	/* glm */
	proj = glm::ortho(0.0f, (float)SCREEN_W, 0.0f, (float)SCREEN_H, -1.0f, 1.0f);
	/* shaders */
	shaderIDB = initShader("b_spline.vs", "b_spline.fs");
	glUseProgram(shaderIDB);
	projLocB = glGetUniformLocation(shaderIDB, "proj");
	shaderIDC = initShader("circle.vs", "circle.fs");
	glUseProgram(shaderIDC);
	projLocC = glGetUniformLocation(shaderIDC, "proj");
	transLocC = glGetUniformLocation(shaderIDC, "trans");
	circRLocC = glGetUniformLocation(shaderIDC, "circ_r");

	/* b-spline */
	b_spline_generate_points();
	b_spline_generate();
	b_spline_generate_VAO();

	/* circle (sem hash) */
	circ_num = 0;
	initDeltaTime();
	circles_init();
	deltaTime = getDeltaTime();
	printf("SEM HASH: %f\n", deltaTime);
	
	/* circle (com hash) */
	circ_num = 0;
	initDeltaTime();
	circles_init_hash();
	deltaTime = getDeltaTime();
	printf("COM HASH: %f\n", deltaTime);

	circles_init_VAO();

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		glUseProgram(shaderIDB);
		glUniformMatrix4fv(projLocB, 1, GL_FALSE, glm::value_ptr(proj));
		b_spline_draw();
		glUseProgram(shaderIDC);
		glUniformMatrix4fv(projLocC, 1, GL_FALSE, glm::value_ptr(proj));
		glUniform1f(circRLocC, CIRC_R);
		circles_render(transLocC);
	}
	glfwTerminate();
	return 0;
}
