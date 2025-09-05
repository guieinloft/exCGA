/*
 * Solids.h
 * cena onde vao ser renderizados os objetos
 * copiado de Plane.cpp e Plane.h da demo glsl40_plane
 */

#pragma once

#include <GL/glew.h>

#include <GLFW\glfw3.h>
#include "Scene.h"
#include <vector>
#include "glslprogram.h"

class Solids : public Scene
{
public:
	Solids(GLFWwindow* window, int size = 1);

	// mesh virtual functions
	void init();
	void update(double t);
	void render();
	void resize(int, int);

private:
	void define_scene();
	int size;
	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec3> colors;
	std::vector<unsigned int> indices;

	GLSLProgram shader;
	GLFWwindow* window;

	glm::mat4 modelMatrix;
	glm::mat4 teapotModelMatrix;
	glm::mat4 icoModelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 modelViewProjectionMatrix;
	glm::mat4 modelViewMatrix;

	glm::vec3 color_amb;
	glm::vec3 color_diff;
	glm::vec3 color_spec;

	glm::vec3 teapot_translate;
	glm::vec3 icosahedron_translate;
	glm::vec3 light_pos;
	glm::vec3 rot_vec;
	
	int scene;
};