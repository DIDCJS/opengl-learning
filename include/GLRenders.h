#pragma once


#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>

#include "UniformAttribute.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

GLint CreateTexture(TexImage& tex, int width, int height, GLint internalformat, GLenum type, GLenum  format, GLint minFilter, GLint magFilter, void* data = NULL);


#define MAX_VERTEX_LENGTH 1024

enum {
	INT1_UNIFORM,
	FLT1_UNIFORM,
	FLT2_UNIFORM,
	FLT4_UNIFORM,
	FLT44_UNIFORM,
	FLT_ARRAY_UNIFORM,
	TEXTURE_UNIFORM,
	ATTRIBUTES_POSITION,
	ATTRIBUTES_TEXTURE_COORD,
	NUMS_UNIFORM_TYPES
};


//template<gl_attribute_type attribute_type>
class glAttribute{
public:
	GLint attribute_id;
	bool bNeedDisable;

	glAttribute() {
		attribute_id = -1;
		bNeedDisable = false;
	};

};

class GLRenders : public  UniformAttribute
{
public:
	enum {
		ARRAY_TYPE_VECTEX = 0,
		ARRAY_TYPE_TEXTURE,
		ARRAY_TYPE_COLOR,
		NUMS_TYPE_ARRAY
	};


public:
	bool setProgramHandle(const GLuint ProgramHandle);
	bool drawArrays(GLenum mode, GLint first, GLsizei count);


	void Use() { glUseProgram(m_ProgramHandle); };
	GLRenders();
	~GLRenders(void);
protected:
	std::map<std::string, glAttribute> m_attributes;
	//bool m_IsExtTexture;//是否为拓展纹理
	//shader脚本


};

