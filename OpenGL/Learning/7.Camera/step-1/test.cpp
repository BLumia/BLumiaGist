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

const int screenWidth = 800;
const int screenHeight = 600;
GLfloat gFovVal = 45.0f;

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
		//  ---- 位置 ----  ---- 纹理坐标 ----
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	glm::vec3 cubePositions[] = {
	  glm::vec3( 0.0f,  0.0f,  0.0f), 
	  glm::vec3( 2.0f,  5.0f, -15.0f), 
	  glm::vec3(-1.5f, -2.2f, -2.5f),  
	  glm::vec3(-3.8f, -2.0f, -12.3f),  
	  glm::vec3( 2.4f, -0.4f, -3.5f),  
	  glm::vec3(-1.7f,  3.0f, -7.5f),  
	  glm::vec3( 1.3f, -2.0f, -2.5f),  
	  glm::vec3( 1.5f,  2.0f, -2.5f), 
	  glm::vec3( 1.5f,  0.2f, -1.5f), 
	  glm::vec3(-1.3f,  1.0f, -1.5f)  
	};
	
	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);
	
	GLuint indices[] = { // 起始于0!
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	// VBO n VAO
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO); // gen one buffer
	glGenVertexArrays(1, &VAO); // gen one vertex array(what's that)
	// Bind VAO
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // what's GL_ARRAY_BUFFER..
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // data form VBO to memory

	// Vertex Attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // so that argument is the index of that vertex attrib 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);  
	// Unbind VAO
	glBindVertexArray(0);
	
	// Game Loop
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // Check if there is a Stanley are pushing buttons
		
		// it will cause problem if we don't draw anything on linux, so we set a clear color here.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // State Setting func
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//ACTIVE Shader Program
		ourShader.Use();
		
		//Transform
		glm::mat4 model, view, projection;
		//model = glm::rotate(model, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));//glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), 
				glm::vec3(0.0f, 0.0f, 0.0f), 
				glm::vec3(0.0f, 1.0f, 0.0f));
		projection = glm::perspective(glm::radians(gFovVal), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);
		//+1s
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		GLint projectionLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		//glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 1);
		glBindVertexArray(VAO); // Bind VAO
		
		for(GLuint i = 0; i < 10; i++)
		{
		  glm::mat4 model;
		  model = glm::translate(model, cubePositions[i]);
		  GLfloat angle = 20.0f * i; 
		  model = glm::rotate(model, glm::radians(((GLfloat)glfwGetTime() * 50.0f) + angle), glm::vec3(1.0f, 0.3f, 0.5f));
		  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		  glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		glBindVertexArray(0); // Unbind VAO
		
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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
		
	if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		gFovVal = gFovVal > 0.1f ? gFovVal - 1.0f : 0.0f;
		printf("Fov: %f\n",gFovVal);
	}
		
	if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
		gFovVal = gFovVal < 179.9f ? gFovVal + 1.0f : 180.0f;
		printf("Fov: %f\n",gFovVal);
	}
} 
