//g++ test.cpp -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -std=c++11 -o run 

#include <iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Using Core Profile
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Cannot resize window
	
	// Create a Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	// Init GLEW
	glewExperimental = GL_TRUE;
	if (glewInit()) {
		//[info] https://stackoverflow.com/questions/13784233/glewinit-failed-opengl-app
		//For some reason the return wasn't comparing well to GLEW_OK 
		//but if the value is anything but 0, there's an error.
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	
	// Tell OpenGL render size
	glViewport(0, 0, 800, 600);
	
	// Game Loop
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // Check if there is a Stanley are pushing buttons
		
		// it will cause problem if we don't draw anything on linux, so we set a clear color here.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // State Setting func
		glClear(GL_COLOR_BUFFER_BIT); // State Apply func
		
		glfwSwapBuffers(window);
	}
	
	// Free GLFW Memory
	glfwTerminate();

	return 0;
}
