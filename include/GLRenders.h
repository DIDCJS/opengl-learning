#pragma once


#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>

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
	std::string textureType;
};

GLint CreateTexture(TexImage& tex, int width, int height, GLint internalformat, GLenum type, GLenum  format, GLint minFilter, GLint magFilter, void* data = NULL);

void TextureFromFile(std::string path, TexImage& tex);

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
class glAttribute {
public:
	GLint attribute_id;
	bool bNeedDisable;

	glAttribute() {
		attribute_id = -1;
		bNeedDisable = false;
	};

};

class GLRenders
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

	GLint findUniform(std::string attribute_name);
	GLint findAttribute(std::string attribute_name);

	bool setTextureID(std::string attribute_name, const GLenum texture_slot, const int texture_local_id, const GLint textureID, const bool IsExtTexture);
	bool setIntUniform(std::string attribute_name, const GLint _data);
	bool setFltUniform(std::string attribute_name, const GLfloat _data);
	bool setFlt2Uniform(std::string attribute_name, const float data_x, const float data_y);
	bool setFlt3Uniform(std::string attribute_name, const float data_x, const float data_y, const float data_z);
	bool setFlt4Uniform(std::string attribute_name, const float data_x, const float data_y, const float data_z, const float data_w);
	bool setMat4Uniform(std::string attribute_name, const glm::mat4 trans);
	bool setVectexAttribute(std::string attribute_name, const GLint size, const int vectex_num, const float* _data);
	bool setTextureCoordsAttribute(std::string attribute_name, const float* _data, const GLint size, const int vectex_num);
	bool setFltArrayUniform(std::string attribute_name, const float* _data, const int vectex_num);

	bool disableVertexAttribArray(std::string attribute_name);

	bool drawArrays(GLenum mode, GLint first, GLsizei count);


	void Use() { glUseProgram(m_ProgramHandle); };
	GLRenders();
	~GLRenders(void);
protected:
	std::map<std::string, glAttribute> m_attributes;
	//bool m_IsExtTexture;//是否为拓展纹理
	//shader脚本
	GLuint m_ProgramHandle;

};

