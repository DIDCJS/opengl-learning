
#include "GLRenders.h"
#include <opencv2/opencv.hpp>
#include "GLDefine.h"

#ifndef GL_TEXTURE_EXTERNAL_OES
#define GL_TEXTURE_EXTERNAL_OES 0x8D65
#endif

#define MAP 1

GLRenders::GLRenders()
{
	m_ProgramHandle = 0;

}

GLRenders::~GLRenders(void)
{

}

bool GLRenders::setProgramHandle(const GLuint ProgramHandle)
{
	if (ProgramHandle != 0)
	{
		m_ProgramHandle = ProgramHandle;
		return true;
	}
	return false;
}

static GLint GetAttributeID(GLuint programHandle, std::string attribute_name)
{
	GLint attri_id = -1;
	if (programHandle == 0)
	{
		printf("program id is NULL");
	}
	else
	{
		attri_id = glGetAttribLocation(programHandle, attribute_name.c_str());
	}

	if (attri_id == -1)
	{
		printf("there is an error in loading Attribute name:%s ", attribute_name.c_str());
	}
	return attri_id;
}

static GLint GetUniformID(GLuint programHandle, std::string attribute_name)
{
	GLint attri_id = -1;
	if (programHandle == 0)
	{
		printf("program id is NULL\n");
	}
	else
	{
		attri_id = glGetUniformLocation(programHandle, attribute_name.c_str());
	}

	if (attri_id == -1)
	{
		printf("there is an error in loading Attribute name:%s \n", attribute_name.c_str());
	}
	return attri_id;
}

GLint GLRenders::findUniform(std::string attribute_name)
{
#if MAP
	if (m_attributes.find(attribute_name) == m_attributes.end())
	{
		//printf("the attribute named %s has not been initialized\n", attribute_name.c_str());
		m_attributes[attribute_name].attribute_id = GetUniformID(m_ProgramHandle, attribute_name);
	}

	return  m_attributes[attribute_name].attribute_id;
#else
	return GetUniformID(m_ProgramHandle, attribute_name);
#endif
}

bool GLRenders::setTextureID(std::string attribute_name, const GLenum texture_slot, const int texture_local_id, const GLint textureID, const bool IsExtTexture)
{

	GLint uniform_id = findUniform(attribute_name);
	if (uniform_id == GL_INVALID_VALUE || uniform_id == GL_INVALID_OPERATION || uniform_id == GL_INVALID_OPERATION) {
		printf("### error : can not find attribute_name in shaders");
	}
	else {
		//printf("### uniform_id : %d\n", uniform_id);
	}
	//CHECK_GL
	if (textureID != 0)
	{
		glActiveTexture(texture_slot);
		glBindTexture(IsExtTexture == true ? GL_TEXTURE_EXTERNAL_OES : GL_TEXTURE_2D, textureID);
		glUniform1i(uniform_id, texture_local_id);
		return true;
	}
	else {
		printf("texture id is NULL\n");
	}
	return false;
}

bool GLRenders::setIntUniform(std::string attribute_name, const GLint _data)
{
	GLint uniform_id = findUniform(attribute_name);

	glUniform1i(uniform_id, _data);

	return true;
}


bool GLRenders::setFltUniform(std::string attribute_name, const GLfloat _data)
{

	GLint uniform_id = findUniform(attribute_name);

	glUniform1f(uniform_id, _data);
	return true;
}


bool GLRenders::setFlt2Uniform(std::string attribute_name, const float data_x, const float data_y)
{

	GLint uniform_id = findUniform(attribute_name);


	glUniform2f(uniform_id, data_x, data_y);
	return true;
}

bool GLRenders::setFlt3Uniform(std::string attribute_name, const float data_x, const float data_y, const float data_z)
{

	GLint uniform_id = findUniform(attribute_name);


	glUniform3f(uniform_id, data_x, data_y, data_z);
	return true;
}


bool GLRenders::setFlt4Uniform(std::string attribute_name, const float data_x, const float data_y, const float data_z, const float data_w)
{

	GLint uniform_id = findUniform(attribute_name);

	glUniform4f(uniform_id, data_x, data_y, data_z, data_w);
	return true;
}

bool GLRenders::setMat4Uniform(std::string attribute_name, const glm::mat4 trans) {
	GLint uniform_id = findUniform(attribute_name);

	if (uniform_id < 0) return false;
	glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(trans));
	return true;
}


bool GLRenders::setFltArrayUniform(std::string attribute_name, const float* _data, const int vectex_num)
{
	GLint uniform_id = findUniform(attribute_name);
#if  MAP
	glUniform1fv(m_attributes[attribute_name].attribute_id, vectex_num, _data);
#else
	glUniform1fv(uniform_id, vectex_num, _data);
#endif //  MAP
	return true;
}

GLint GLRenders::findAttribute(std::string attribute_name)
{
#if MAP
	if (m_attributes.find(attribute_name) == m_attributes.end())
	{
		/*printf("the attribute named %s has not been initialized\n", attribute_name.c_str());*/
		m_attributes[attribute_name].attribute_id = GetAttributeID(m_ProgramHandle, attribute_name);
	}
	return m_attributes[attribute_name].attribute_id;
#else
	return GetAttributeID(m_ProgramHandle, attribute_name);
#endif
}


bool GLRenders::setVectexAttribute(std::string attribute_name, const GLint size, const int vectex_num, const float* _data)
{
	GLint attri_id = findAttribute(attribute_name);

	glEnableVertexAttribArray(attri_id);
	glVertexAttribPointer(attri_id, size, GL_FLOAT, GL_FALSE, vectex_num, _data);
#if MAP
	m_attributes[attribute_name].bNeedDisable = true;
#endif
	return true;
}


bool GLRenders::setTextureCoordsAttribute(std::string attribute_name, const float* _data, const GLint size, const int vectex_num)
{
	GLint attri_id = findAttribute(attribute_name);

	glEnableVertexAttribArray(attri_id);
	glVertexAttribPointer(attri_id, size, GL_FLOAT, GL_FALSE, 0, _data);
#if MAP
	m_attributes[attribute_name].bNeedDisable = true;
#endif
	return true;
}



bool GLRenders::disableVertexAttribArray(std::string attribute_name)
{
#if MAP
	if (m_attributes.find(attribute_name) == m_attributes.end())
	{
		printf("can not find the array attribute which is named with %s\n", attribute_name.c_str());
			return false;
	}
	glDisableVertexAttribArray(m_attributes[attribute_name].attribute_id);
#else
	glDisableVertexAttribArray(findAttribute(attribute_name));
#endif
	return true;
}


bool GLRenders::drawArrays(GLenum mode, GLint first, GLsizei count)
{
	glDrawArrays(mode, first, count);
#if MAP
	for (auto iter = m_attributes.begin(); iter != m_attributes.end(); ++iter)
	{
		if (iter->second.bNeedDisable)
		{
			glDisableVertexAttribArray(iter->second.attribute_id);
		}
	}
#endif
	return true;
}

void CheckGLError()
{
	while (true)
	{
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;
		printf("GL Error: %x\n", err);
		abort();
	}
}

GLint CreateTexture(TexImage& tex, int width, int height, GLint internalformat, GLenum type, GLenum format, GLint minFilter, GLint magFilter, void* data /*= NULL*/)
{
	GLint gl_err = GL_NO_ERROR;
	tex.width = width;
	tex.height = height;
	tex.internalformat = internalformat;
	tex.format = format;
	tex.type = type;
	tex.glTexture = 0;

	printf("texture width : %d, \n", tex.width);
	printf("texture height : %d, \n", tex.height);
	printf("texture internalformat : %d, \n", tex.internalformat);
	printf("texture format : %u, \n", tex.format);
	printf("texture type : %u, \n", tex.type);

	glGenTextures(1, &tex.glTexture);
	gl_err = glGetError();
	if (gl_err != GL_NO_ERROR)
	{
		printf("thE texture glGenTextures failed\n");
		return gl_err;
	}

	glBindTexture(GL_TEXTURE_2D, tex.glTexture);
	//glTexImage2D(GL_TEXTURE_2D, 0, tex.internalformat, tex.width,
		//tex.height, 0, tex.format, tex.type, data);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	gl_err = glGetError();
	if (gl_err != GL_NO_ERROR)
	{
		printf(" texture glTexImage2D failed\n");
		abort();
		return gl_err;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	gl_err = glGetError();
	if (gl_err != GL_NO_ERROR)
	{
		printf("glTexParameteri failed\n");
		return gl_err;
	}

	return gl_err;
}

void TextureFromFile(std::string path, TexImage& tex) {
	std::string prefix = R"(model/nanosuit/)";
	cv::Mat img = cv::imread(prefix + path);
	if (img.data == nullptr) {
		printf("### error : can not load texture");
	}
	if (img.channels() == 3) {
		cv::cvtColor(img, img, cv::COLOR_BGR2RGBA);
	}
	else if (img.channels() == 4) {
		cv::cvtColor(img, img, cv::COLOR_BGRA2RGBA);
	}
	CreateTexture(tex, img.cols, img.rows, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, img.data);
}
