#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLfloat vertices[] = {
	-0.5, -0.5, 0.f,
	0.5, -0.5, 0.f,
	0, 0.5, 0.f
};


const char* vertex_shader = 
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;"
"void main()"
"{"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
"}";

const char* fragment_shader = 
"#version 460 core\n"
"out vec4 FragColor;"
"void main()"
"{"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
"}";



void frameBufferSizeCallback(GLFWwindow* window, int width,  int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}


int main() {

	if (!glfwInit()) {
		std::cerr << "failed to init glfw" << std::endl;
		return -1;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	GLFWwindow* pWindow = glfwCreateWindow(640, 480, "Battle City", nullptr, nullptr);

	if (!pWindow) {
		std::cerr << "failed to pWindow" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(pWindow);

	if (!gladLoadGL()) {
		std::cerr << "can't load glad" << std::endl;
		return -1;
	}

	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL VERSION: " << glGetString(GL_VERSION) << std::endl;


	glfwSetFramebufferSizeCallback(pWindow, frameBufferSizeCallback);

	//shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader, nullptr);
	glCompileShader(fragmentShader);


	//shader program
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glUseProgram(shaderProgram);

	//clear 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), nullptr);
	glEnableVertexAttribArray(0);

	//VBO
	GLuint VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);


	//VAO
	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	


	while (!glfwWindowShouldClose(pWindow)) {

		glfwPollEvents();
		processInput(pWindow);

		glClearColor(0, 0.7, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	glfwTerminate();

	return 0;
}
