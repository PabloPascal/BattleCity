#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Renderer/Renderer.h"

GLfloat vertices[] = {
	-0.5, -0.5, 0.f,
	0.5, -0.5, 0.f,
	0.0, 0.5, 0.0
};


GLfloat colors[] = {
	1, 0, 0,
	0, 1, 0,
	0, 0, 1
};


const char* vertex_shader =
"#version 460\n"
"layout(location = 0) in vec3 vertex_position;"
"layout(location = 1) in vec3 vertex_color;"
"out vec3 color;"
"void main()"
"{"
"	color = vertex_color;"
"   gl_Position = vec4(vertex_position, 1.0);"
"}";


const char* fragment_shader = 
"#version 460\n"
"in vec3 color;"
"void main()"
"{"
"   gl_FragColor = vec4(color.x, color.y, color.z, 1.0f);"
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
	Renderer::ShaderProgram shaderProgram(vertex_shader, fragment_shader);
	
	if (!shaderProgram.isCompiled()) {
		
		std::cerr << "Can't create Shader program" << std::endl;
		return -1;
	}


	//attrib
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), nullptr);
	glEnableVertexAttribArray(0);

	//VBO
	GLuint VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	GLuint VBOcolors = 0;
	glGenBuffers(1, &VBOcolors);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcolors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), &colors, GL_STATIC_DRAW);


	//VAO
	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBOcolors);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);


	while (!glfwWindowShouldClose(pWindow)) {

		glfwPollEvents();
		processInput(pWindow);

		glClearColor(0, 0.7, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}
