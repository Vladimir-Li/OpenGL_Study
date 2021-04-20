#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "TextColor.h"

#define VERTEX "VERTEX_SHADER"
#define FRAGMENT "FRAGMENT_SHADER"
#define PROGRAM "SHADER_PROGRAM"

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragPath) {

		std::string vertexCode;
		std::string fragCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			// open file
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handler
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e) {
			fprintf(stderr, TEXT_COLOR_RED "Failed to Read File:\n%s1,\n%s2" TEXT_COLOR_RESET, vertexPath, fragPath);
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragCode.c_str();
		// compile shaders
		unsigned int vertex, fragment;
		//vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, VERTEX);
		//fragment shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, FRAGMENT);
		//shader program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, PROGRAM);

		// Delete The Shaders
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use() {
		glUseProgram(ID);
	}
	void setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	void checkCompileErrors(unsigned int shader, std::string type) {
		int success;
		char infolog[1024];
		if (type != "PROGRAM") {
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 1024, NULL, infolog);
				fprintf(stderr, TEXT_COLOR_RED "(Shader Compilation Failed\nType: %s" TEXT_COLOR_RESET, type);
				printf("InfoLog: %s", infolog);
			}
		}
		else{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shader, 1024, NULL, infolog);
				fprintf(stderr, TEXT_COLOR_RED "(Shader Linking Failed\nType: %s\n" TEXT_COLOR_RESET, type);
				printf("InfoLog: %s", infolog);
			}
		}
	}
};
#endif // !SHADER_H
