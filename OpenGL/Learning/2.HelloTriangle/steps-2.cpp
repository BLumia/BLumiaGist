//g++ test.cpp -lGLEW -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -std=c++11 -o run 

#include <iostream>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Shaders
const GLchar* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
	"}\0";
const GLchar* fragmentShaderSource = "#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

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
	
	// Tell OpenGL render size
	glViewport(0, 0, 800, 600);
	
	// Prepare Stuff
	// Triangle Points
	GLfloat vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};
	GLuint indices[] = { // 起始于0!
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	// VBO, VAO n EBO
	GLuint VBO, VAO, EBO;
	glGenBuffers(1, &VBO); // gen one buffer
	glGenBuffers(1, &EBO); // gen one buffer
	glGenVertexArrays(1, &VAO); // gen one vertex array(what's that)
	// Bind VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // data form VBO to memory
	// Vertex Attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // What's that argument
	// Unbind VAO
	glBindVertexArray(0);
	// Shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FLAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linking Shader Programs
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	// Free our shaders after linked to program.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	// Game Loop
	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents(); // Check if there is a Stanley are pushing buttons
		
		// it will cause problem if we don't draw anything on linux, so we set a clear color here.
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // State Setting func
		glClear(GL_COLOR_BUFFER_BIT); // State Apply func
		
		//Use Shader Program
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // Bind VAO
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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
