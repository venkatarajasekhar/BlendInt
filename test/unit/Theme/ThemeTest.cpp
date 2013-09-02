// cpp

#include <GL/glew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include <BIL/Interface.hpp>
#include <BIL/Label.hpp>
#include <BIL/Theme.hpp>

#include "ThemeTest.h"

using namespace BIL;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ThemeTest);

ThemeTest::ThemeTest ()
{

}

ThemeTest::~ThemeTest ()
{

}

void ThemeTest::setUp ()
{
}

void ThemeTest::tearDown ()
{
}

void ThemeTest::initialize1 ()
{
	if(!Theme::initialize()) {
		CPPUNIT_ASSERT(false);
		return;
	}

	unsigned char r = 0x00;

	Theme* theme = Theme::instance();

	if (theme != NULL)
		theme->initialize();

	//_themeUI.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
	r = theme->themeUI()->wcol_tool.outline.r();

	if (theme != NULL) {
		Theme::release();
	}

	cout << "Red: " << r << endl;
	Theme::release();
	CPPUNIT_ASSERT(r == 0x19);
}

void ThemeTest::initialize2 ()
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

    Theme* theme = Theme::instance();
	//_themeUI.wcol_tool.outline = RGBAf(0.098, 0.098, 0.098);
	Color bg_color = theme->themeUI()->wcol_menu_item.item;
	Color textcolor = theme->themeUI()->wcol_menu_item.text;

	Label label(L"Text in Label");
	label.set_pos(Point(50, 50));
	label.set_background(bg_color);
	label.setFont(Font("Droid Sans", 12));
	label.setTextColor(textcolor);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BIL */
	Interface::release();

	glfwTerminate();
	CPPUNIT_ASSERT(true);
}

void ThemeTest::cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

void ThemeTest::cbWindowSize (GLFWwindow* window, int w, int h)
{
	BIL::Interface::instance()->resizeEvent(w, h);
}

void ThemeTest::cbKey (GLFWwindow* window, int key, int scancode, int action,
        int mods)
{
	BIL::Interface::instance()->keyEvent(key, scancode, action, mods);
}

void ThemeTest::cbMouseButton (GLFWwindow* window, int button, int action,
        int mods)
{
	BIL::Interface::instance()->mouseButtonEvent(button, action, mods);
}

void ThemeTest::cbCursorPos (GLFWwindow* window, double xpos, double ypos)
{
	BIL::Interface::instance()->cursorPosEvent(xpos, ypos);
}
