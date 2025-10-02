/*
* Autor: Rodrigo Appelt, Cesar Pozzer
* Data: 10/03/2025
*
* Classe que gerencia os recursos do OpenGL 4.0
* e desenho de primitivas 2D.
*/

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "glslprogram.h"
#include "Fonts.h"




struct DrawStructure {
public:
	unsigned int vao;
	unsigned int vbo_positions;
	unsigned int vbo_colors;
	unsigned int vbo_uv;
	unsigned int texture;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> uv;

	void reset();
};

class Controller {
public:
	Controller(int& screenWidth, int& screenHeight);

	/// <summary>
	/// Compila shaders, cria texturas e buffers.
	/// </summary>
	void init();
	void newFrame();

	void processColor(float r, float g, float b);
	void processTranslate(float x, float y);
	void processClear(float r, float g, float b);
	void processRect(float x1, float y1, float x2, float y2, bool fill);
	void processPoint(float x1, float y1);
	void processCircle(float x, float y, float radius, int div, bool fill);
	void processLine(float x1, float y1, float x2, float y2);
	void processText(float x, float y, const char* str, float fontSize);
private:
	int &screenWidth;
	int &screenHeight;
	GLSLProgram uiShader;
	GLSLProgram textShader;

	Atlas font_atlas;
	std::map<int, Glyph> font_glyphs;

	// essas structures tem os vbos e vaos para cada tipo de desenho
	DrawStructure fillStructure;
	DrawStructure lineStructure;
	DrawStructure textStructure;

	void drawFilled();
	void drawLines();
	void drawText();
	void compileShaders();
	void createUiBuffers();
	void createTextBuffers();
	void createTextures();

	// variaveis de controle internas
	glm::vec2 translation{ 0.0f, 0.0f };
	glm::vec3 color{ 0.0f, 0.0f, 0.0f };
};