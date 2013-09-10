// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <stdio.h>

#include "TextureFontTest.h"

#include <BIL/Interface.hpp>
#include <BIL/Label.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/Rect.hpp>
#include <BIL/TextureFont.hpp>

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(TextureFontTest);

TextureFontTest::TextureFontTest ()
{

}

TextureFontTest::~TextureFontTest ()
{

}

void TextureFontTest::setUp ()
{
}

void TextureFontTest::tearDown ()
{
}

static GLfloat black[4] = { 0, 0, 0, 1 };

void TextureFontTest::show1 ()
{
	/* Initialize the library */
	if (!glfwInit())
		return;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	glfwSetWindowSizeCallback(window, &cbWindowSize);
	glfwSetKeyCallback(window, &cbKey);
	glfwSetMouseButtonCallback(window, &cbMouseButton);
	glfwSetCursorPosCallback(window, &cbCursorPos);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* initialize BIL after OpenGL content is created */
	if (!Interface::initialize()) {
		glfwTerminate();
		CPPUNIT_ASSERT(false);
		return;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	Freetype fe;
	fe.open(Font("Sans"), 96);
	fe.setCharSize(100);
	TextureFont tex_font;
	tex_font.generate(&fe, L'A');

	GLint attribute_coord;
	GLint uniform_tex;
	GLint uniform_color;

	GLuint vbo;

	GLSLProgram program;

	program.attachShaderPair(TextureFont::getVertexShader(), TextureFont::getFragmentShader());
	program.link();

	attribute_coord = program.getAttributeLocation("coord");
	uniform_tex = program.getUniformLocation("tex");
	uniform_color = program.getUniformLocation("color");

	if(attribute_coord == -1 || uniform_tex == -1 || uniform_color == -1)
		CPPUNIT_ASSERT(false);

	// Create the vertex buffer object
	glGenBuffers(1, &vbo);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {

		/* Render here */
		app->render();

		/* Enable blending, necessary for our alpha texture */
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(program.id());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_font.texture());
		glUniform1i(uniform_tex, 0);

		glUniform4fv(uniform_color, 1, black);

		/* Set up the VBO for our vertex data */
		glEnableVertexAttribArray(attribute_coord);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

		// render character from atlas here
		/* Calculate the vertex and texture coordinates */
		float x2 = 100 + tex_font.glyph_metrics().bitmap_left;
		float y2 = 100 + tex_font.glyph_metrics().bitmap_top;
		float w = tex_font.glyph_metrics().bitmap_width;
		float h = tex_font.glyph_metrics().bitmap_height;

		point box[4] = {
			{x2, y2, 0, 0},
			{x2 + w, y2, 1, 0},
			{x2, y2 - h, 0, 1},
			{x2 + w, y2 - h, 1, 1},
		};

		/* Draw the character on the screen */
		glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glDisableVertexAttribArray(attribute_coord);
		glUseProgram(0);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	if(glIsBuffer(vbo)) {
		glDeleteBuffers(1, &vbo);
	}

	/* release BIL */
	Interface::release();
	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void TextureFontTest::render_text (const char* text, float x, float y)
{

}

void TextureFontTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void TextureFontTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BIL::Interface::instance()->resizeEvent(w, h);
}

void TextureFontTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BIL::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void TextureFontTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BIL::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void TextureFontTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BIL::Interface::instance()->cursorPosEvent(xpos, ypos);
}
