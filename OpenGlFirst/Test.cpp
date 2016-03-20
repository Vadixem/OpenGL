#define GLEW_STATIC
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <conio.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void key_callback(GLFWwindow* window, int key, 
				  int scancode, int action, int mode);

int main()
{
	//// initialize glew before we call any openGL func-ns
	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK)
	//{
	//	cout << "Failed to ini GLEW" << endl;
	//	return -1;
	//}



	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGlFirst", nullptr, nullptr);
	// if window == nullptr, terminate everything
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	
	glfwMakeContextCurrent(window);

	glViewport(0, 0, 800, 600);

		//*float r, g, b;
		//*r = g = b = 0;
	while (!glfwWindowShouldClose(window))
	{

		// listen on users input
		glfwPollEvents();			
			//*r += 0.002f;
			//*g += 0.01f;
			//*b += 0.01f;
		// Rendering commands here
			//*glClearColor(r, g, b, 1.0f);
		glClearColor(1.0f, 0.3f, 0.1f, 1.0f);	
		glClear(GL_COLOR_BUFFER_BIT);
		// draw image
		glfwSwapBuffers(window);	
			//*this_thread::sleep_for(chrono::milliseconds(34));	
	}

	// register the func with the proper callback 
	glfwSetKeyCallback(window, key_callback);

	glfwTerminate();
	
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When a user presses the escape key, we set the WindowShouldClose property to true, 
	// closing the application 
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	glfwSetWindowShouldClose(window, GL_TRUE);	
}