#include<stdio.h>
#include<iostream>
#define GLEW_STATIC
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include "TextColor.h"

/*
OpenGL Study
Learn OpenGL Website
Vladimir Li
2021/04/19
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

float vertices[] = {
	0.5f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main(){\n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "Failed to Initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(900, 900, "OpenGL", NULL, NULL);
	if (window == nullptr) {
		fprintf(stderr, TEXT_COLOR_RED "Could Not Create Window\n" TEXT_COLOR_RESET);
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, TEXT_COLOR_RED "Failed to Intialize GLEW\n" TEXT_COLOR_RESET);
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	printf(TEXT_COLOR_GREEN "WINDOW CREATE SUCCESS\n" TEXT_COLOR_RESET);
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	printf("Max Vertex Attributes: %d\n", nrAttributes);

	// VBO

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Vertex Shader

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int vertex_success;
	char vertex_infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertex_success);

	if (!vertex_success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, vertex_infolog);
		fprintf(stderr, TEXT_COLOR_RED "Vertex Shader Compilation Failed \nInfo:\n%s" TEXT_COLOR_RESET, vertex_infolog);
		return -1;
	}

	// Fragment Shader

	unsigned int fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentshader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentshader);

	int frag_success;
	char frag_infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &frag_success);

	if (!frag_success) {
		glGetShaderInfoLog(fragmentshader, 512, NULL, frag_infolog);
		fprintf(stderr, TEXT_COLOR_RED "Fragments Shader Compilation Failed \nInfo:\n%s" TEXT_COLOR_RESET, frag_infolog);
		return -1;
	}

	// Shader Program

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentshader);
	glLinkProgram(shaderProgram);

	int prog_success;
	char prog_infolog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &prog_success);

	if (!prog_success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, prog_infolog);
		fprintf(stderr, TEXT_COLOR_RED "Shader Link Failed\nInfo:\n%s" TEXT_COLOR_RESET, prog_infolog);
		return -1;
	}

	glUseProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentshader);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);


	/* Initializing Triangle */
	//1. bind vao
	glBindVertexArray(VAO);
	//2. copy vertex into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//3. copy index array into index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//4. set vertex attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window)){

		// Draw
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0x0;
}