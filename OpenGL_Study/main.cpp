#include<stdio.h>
#include<iostream>

#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION

#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include "TextColor.h"
#include "Shader.h"
#include "Texture.h"

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
	 0.5f,  0.5f,  0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0f,
	 0.5f, -0.5f,  0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	-0.5f, -0.5f,  0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
	-0.5f,  0.5f,  0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0f
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

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

	// Print Vertex Attribute Numbers
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	printf("Max Vertex Attributes: %d\n", nrAttributes);

	// Vertex Shader
	Shader shader("VertexShader.glsl", "FragmentShader.glsl");

	// Texture 
	Texture texture1("container.jpg", Texture::JPG, Texture::REPEAT, Texture::LINEAR);
	Texture texture2("awesomeface.png", Texture::PNG, Texture::REPEAT, Texture::LINEAR);

	/*-------------------Old Way to Load Texture-------------------*/

	//unsigned int texture1, texture2;
	//glGenTextures(1, &texture1);
	//glBindTexture(GL_TEXTURE_2D, texture1);
	//// Set Mode
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);

	//unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	//if (data) {
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else {
	//	fprintf(stderr, TEXT_COLOR_RED "Failed to Load Texture" TEXT_COLOR_RESET);
	//}
	//stbi_image_free(data);


	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture2);
	//// Set Mode
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINE);

	//data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	//if (data) {
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else {
	//	fprintf(stderr, TEXT_COLOR_RED "Failed to Load Texture2" TEXT_COLOR_RESET);
	//}
	//stbi_image_free(data);
	
	/*-------------------Old Way to Load Texture-------------------*/

	// VAO&VBO
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	shader.use();

	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);


	float mixVal = 0.0f;
	glUniform1f(glGetUniformLocation(shader.ID, "mixVal"), mixVal);

	while (!glfwWindowShouldClose(window)){

		// User Input
		//doSomething(window);

		
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && mixVal <= 1.0f)
			mixVal += 0.01f;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && mixVal >= 0.0f)
			mixVal -= 0.01f;
		shader.setFloat("mixVal", mixVal);


		// Draw
		glClearColor(0.2f, 0.3f, 0.3f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1.ID);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2.ID);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0x0;
}

void doSomething(GLFWwindow* window) {
	
}