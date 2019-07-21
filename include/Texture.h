#pragma once
#include <string>
#include "glad/glad.h"
struct TexImage
{
	GLuint glTexture;
	int width;
	int height;
	GLenum format;
	GLenum type;
	GLint internalformat;
	size_t rowPitch;
};

class Texture {
public: 
	GLint CreateTexture(TexImage& tex, int width, int height, GLint internalformat, GLenum type, GLenum  format, GLint minFilter, GLint magFilter, void* data = NULL);

};