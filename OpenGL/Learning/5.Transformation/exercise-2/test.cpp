//g++ test.cpp -lSOIL -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -std=c++11 -o run 

/*
 * 
*/

#include <iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//SOIL
#include <SOIL/SOIL.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Shader
#include "shader.hpp"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Using Core Profile
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); //Cannot resize window
	
	// Create a Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	
	// Callback Binding
	glfwSetKeyCallback(window, key_callback);
	
	// Init GLEW
	glewExperimental = GL_TRUE;
	if (glewInit()) {
		//[info] https://stackoverflow.com/questions/13784233/glewinit-failed-opengl-app
		//For some reason the return wasn't comparing well to GLEW_OK 
		//but if the value is anything but 0, there's an error.
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	// Shader program
    Shader ourShader("./vertexshader.glsl", "./fragmentshader.glsl");
	
	// Tell OpenGL render size
	glViewport(0, 0, 800, 600);
	
	// Prepare Stuff
	// Texture Coordinate
	GLfloat texCoords[] = {
		0.0f, 0.0f, // 左下角
		1.0f, 0.0f, // 右下角
		0.5f, 1.0f // 顶部位置
	};
	int width, height;
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	GLuint texture[2];
	glGenTextures(2, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	// Points
	GLfloat vertices[] = {
		//  ---- 位置 ----     ---- 颜色 ----  ---- 纹理坐标 ----
		 0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
		 0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
		-0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下
		-0.5f,   0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
	};
	GLuint indices[] = { // 起始于0!
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	// VBO n VAO
	GLuint VBO, VAO, EBO;
	glGenBuffers(1, &VBO); // gen one buffer
	glGenVertexArrays(1, &VAO); // gen one vertex array(what's that)
	glGenBuffers(1, &EBO); // gen one buffer
	// Bind VAO
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // what's GL_ARRAY_BUFFER..
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // data form VBO to memory
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // data form VBO to memory
	// Vertex Attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // so that argument is the index of that vertex attrib 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); // so this one is 1
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);  
	// Unbind VAO
	glBindVertexArray(0);
	
	// Game Loop
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // Check if there is a Stanley are pushing buttons
		
		// it will cause problem if we don't draw anything on linux, so we set a clear color here.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // State Setting func
		glClear(GL_COLOR_BUFFER_BIT); // State Apply func
		
		//ACTIVE Shader Program
		ourShader.Use();
		
		//glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
		
		//Transform
		glm::mat4 trans;
		
		//1st Transform
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.0, 0.0, 1.0));
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));  
		GLuint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		glBindVertexArray(VAO); // Bind VAO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); // Unbind VAO
		
		//1st Transform
		trans = glm::mat4(); // Reset it to an identity matrix
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		//trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.0, 0.0, 1.0));
		GLfloat floatVal = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(floatVal, floatVal, floatVal));  
		transformLoc = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
		//glBindVertexArray(VAO); // Bind VAO
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); // Unbind VAO
		
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// Free GLFW Memory
	glfwTerminate();

	return 0;
}

//---------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	// When press ESC, set WindowShouldClose as true
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
} 
