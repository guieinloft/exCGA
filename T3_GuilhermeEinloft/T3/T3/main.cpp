#include <stdio.h>
#include <stdlib.h>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

GLFWwindow *window;

float vertices[] = {
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f
};
float vert_textures[] = {
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};
int indices[] = {
	0, 1, 3,
	1, 2, 3
};

GLuint vaoID, shaderID;
GLuint scaleLoc, centerLoc, aspectLoc;

float scale = 2.0f;
float center[2] = {0.0f, 0.0f};
float aspect = 1.3333;
bool inputs[] = {0, 0, 0, 0, 0, 0};

std::chrono::steady_clock sclock;
std::chrono::steady_clock::time_point time1, time2;
float deltaTime = 0.0f, bigDeltaTime = 0.0f, avgDeltaTime = 0.0;
int num_frames = -1;

void keyCallback(GLFWwindow *window,
		int key, int scancode, int action, int mods)
{
	bool pressed = action & (GLFW_PRESS | GLFW_REPEAT);
	switch (key){
	case 'W':
		inputs[0] = pressed;
		break;
	case 'S':
		inputs[1] = pressed;
		break;
	case 'A':
		inputs[2] = pressed;
		break;
	case 'D':
		inputs[3] = pressed;
		break;
	case 'Q':
		inputs[4] = pressed;
		break;
	case 'E':
		inputs[5] = pressed;
		break;
	}
}

void framebufferSizeCallback(GLFWwindow *window, int w, int h)
{
	glViewport(0, 0, w, h);
	aspect = (float)w / h;  
}

void initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, 0);

	window = glfwCreateWindow(800, 600, "Mandelbrot", NULL, NULL);
	if (!window) {
		printf("Erro ao inicializar janela do GLFW\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	
	/* callbacks */
	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

void initGLEW()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		printf("Erro ao inicializar o GLEW\n");
		exit(-1);
	}
}

void initVAO()
{
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	unsigned int handle[3];
	glGenBuffers(3, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float),
			(GLvoid*)&vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0,
			(GLubyte*)NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float),
			(GLvoid*)&vert_textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0,
			(GLubyte*)NULL);
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int),
			(GLvoid*)&indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void initShader()
{
	/* vertex shader */
	std::ifstream vsfile("t3.vs");
	std::stringstream vsstreambuffer;
	vsstreambuffer << vsfile.rdbuf();
	std::string vsbuffer = vsstreambuffer.str();
	const char *vscbuffer = vsbuffer.c_str();

	unsigned int vsID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vsID, 1, &vscbuffer, NULL);
	glCompileShader(vsID);

	/* fragment shader */
	std::ifstream fsfile("t3.fs");
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
	glUseProgram(shaderID);

	glDeleteShader(vsID);
	glDeleteShader(fsID);

	/* uniform locations */
	scaleLoc = glGetUniformLocation(shaderID, "scale");
	centerLoc = glGetUniformLocation(shaderID, "center");
	aspectLoc = glGetUniformLocation(shaderID, "aspect");
}

void initDeltaTime()
{
	time1 = std::chrono::steady_clock::now();
	time2 = std::chrono::steady_clock::now();
}

int main()
{
	/* glfw */
	initGLFW();
	/* glew */
	initGLEW();
	/* gl */
	glViewport(0, 0, 800, 600);
	/* vao */
	initVAO();
	/* shader */
	initShader();
	/* deltatime */
	initDeltaTime();

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
		glBindVertexArray(vaoID);
		scale *= 1 + 2 * deltaTime * (inputs[4] - inputs[5]);
		center[0] += scale * deltaTime * (inputs[2] - inputs[3]);
		center[1] += scale * deltaTime * (inputs[1] - inputs[0]);
		glUniform1f(scaleLoc, scale);
		glUniform1f(aspectLoc, aspect);
		glUniform2f(centerLoc, center[0], center[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
				(GLubyte*)NULL);
		time1 = time2;
		time2 = std::chrono::steady_clock::now();
		num_frames++;
		deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time2 - time1).count() / 1000000.0f;
		if (deltaTime > bigDeltaTime && num_frames > 0)
			bigDeltaTime = deltaTime;
		if (num_frames > 0)
			avgDeltaTime = (avgDeltaTime * (num_frames - 1)
					+ deltaTime) / num_frames;
		printf("FPS Atual: %f\tMenor FPS: %f\tFPS Medio: %f\r",
				1.0f / deltaTime, 1.0f / bigDeltaTime,
				1.0 / avgDeltaTime);
	}
	glfwTerminate();
	return 0;
}
