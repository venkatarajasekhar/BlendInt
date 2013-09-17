/**
 * BIL demo
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <Cpp/Events.hpp>
#include <BIL/Interface.hpp>

#include <BIL/ToggleButton.hpp>
#include <BIL/VerticalLayout.hpp>
#include <BIL/HorizontalLayout.hpp>

#include <BIL/Types.hpp>
#include <BIL/Button.hpp>
#include <BIL/Label.hpp>
#include <BIL/ScrollWidget.hpp>

using namespace BIL;

static void cbError (int error, const char* description)
{
	std::cerr << "Error: " << description
			<< " (error code: " << error << ")"
			<< std::endl;
}

static void cbWindowSize(GLFWwindow* window, int w, int h) {
	BIL::Interface::instance()->resizeEvent(w, h);
}

static void cbKey(GLFWwindow* window, int key, int scancode, int action,
		int mods) {
	BIL::Interface::instance()->keyEvent(key, scancode, action, mods);
}

static void cbMouseButton(GLFWwindow* window, int button, int action,
		int mods) {
	BIL::Interface::instance()->mouseButtonEvent(button, action, mods);
}

static void cbCursorPos(GLFWwindow* window, double xpos, double ypos) {
	BIL::Interface::instance()->cursorPosEvent(xpos, ypos);
}

int main(int argc, char* argv[]) {
	using namespace BIL;

	Cpp::Events::ProcessInit processInit;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwSetErrorCallback(&cbError);

	GLFWwindow* window = glfwCreateWindow(1200, 800, "Demo Window for BIL", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
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
		return -1;
	}

	Interface* app = Interface::instance();
	app->resize(1200, 800);

	VerticalLayout* layout1 = new VerticalLayout;
	layout1->set_pos(100, 100);
	layout1->set_alignment(BIL::AlignVerticalCenter);

	Label* label1 = new Label(L"la1");
	label1->set_pos(100, 100);

	layout1->addWidget(label1);

	HorizontalLayout layout2;

	Label* label2 = new Label(L"Hello Blender");
	label2->set_pos(500, 500);

	layout2.addWidget(label2);
	layout2.addLayout(layout1);

	layout2.set_pos (400, 500);

	ScrollBar bar(Horizontal);
	bar.set_pos (500, 200);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		app->render();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glScissor (100, 100, 200, 200);
		glEnable (GL_SCISSOR_TEST);

		glColor3f(0.6f, 1.0f, 0.2f);
		glBegin(GL_POLYGON);
			glVertex2d(50,50);
			glVertex2d(600,50);
			glVertex2d(600,500);
			glVertex2d(50,500);
		glEnd();

		glDisable(GL_SCISSOR_TEST);

		glPopMatrix();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	/* release BIL */
	Interface::release();

	glfwTerminate();
	return 0;
}

