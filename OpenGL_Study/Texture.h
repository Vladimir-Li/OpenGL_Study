#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include "TextColor.h"

#define Wsetting(W)\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, W);\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, W);

#define Fsetting(F)\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, F);\
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, F);

#define Tsetting(T)\
glTexImage2D(GL_TEXTURE_2D, 0, T, width, height, 0, T, GL_UNSIGNED_BYTE, data);\
glGenerateMipmap(GL_TEXTURE_2D);

class Texture {
public:
	unsigned int ID;
	int width, height, nrChannels;
	enum type{
		PNG,
		JPG
	};
	enum wrapping {
		REPEAT,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};
	enum filter {
		NEAREST,
		LINEAR
	};
	Texture(const char* file, type fileType, wrapping wrapType, filter filterType) {
		// Init
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);
		// Settings
		switch (wrapType)
		{
		case Texture::REPEAT:
			Wsetting(GL_REPEAT);
			break;
		case Texture::MIRRORED_REPEAT:
			Wsetting(GL_MIRRORED_REPEAT);
			break;
		case Texture::CLAMP_TO_EDGE:
			Wsetting(GL_CLAMP_TO_EDGE);
			break;
		case Texture::CLAMP_TO_BORDER:
			Wsetting(GL_CLAMP_TO_BORDER);
			break;
		default:
			Wsetting(GL_REPEAT);
			break;
		}
		switch (filterType)
		{
		case Texture::NEAREST:
			Fsetting(GL_NEAREST);
			break;
		case Texture::LINEAR:
			Fsetting(GL_LINEAR);
			break;
		default:
			Fsetting(GL_LINEAR);
			break;
		}
		stbi_set_flip_vertically_on_load(true);

		unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

		if (data) {
			switch (fileType)
			{
			case Texture::PNG:
				Tsetting(GL_RGBA);
				break;
			case Texture::JPG:
				Tsetting(GL_RGB);
				break;
			}
		}
		else {
			fprintf(stderr, TEXT_COLOR_RED "Failed to Load Texture" TEXT_COLOR_RESET);
		}
		
	}

private:
	// TODO: file name parsing find RGBA or RGB
};