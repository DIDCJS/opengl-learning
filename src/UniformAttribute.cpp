#include "UniformAttribute.h"

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

GLint UniformAttribute::findUniform(std::string attribute_name)
{
#if MAP
	if (m_attributes.find(attribute_name) == m_attributes.end())
	{
		printf("the attribute named %s has not been initialized\n", attribute_name.c_str());
		m_attributes[attribute_name].attribute_id = GetUniformID(m_ProgramHandle, attribute_name);
	}

	return  m_attributes[attribute_name].attribute_id;
#else
	return GetUniformID(m_ProgramHandle, attribute_name);
#endif
}

bool UniformAttribute::setTextureID(std::string attribute_name, const GLenum texture_slot, const int texture_local_id, const GLint textureID, const bool IsExtTexture)
{

	GLint uniform_id = findUniform(attribute_name);
	if (textureID != 0)
	{
		glActiveTexture(texture_slot);
		//glBindTexture(IsExtTexture == true ? GL_TEXTURE_EXTERNAL_OES : GL_TEXTURE_2D, textureID);
		glBindTexture(GL_TEXTURE_2D,textureID);
		glUniform1i(uniform_id, texture_local_id);
		return true;
	}
	else {
		printf("texture id is NULL\n");
	}
	return false;
}


bool UniformAttribute::setIntUniform(std::string attribute_name, const GLint _data)
{
	GLint uniform_id = findUniform(attribute_name);

	glUniform1i(uniform_id, _data);

	return true;
}


bool UniformAttribute::setFltUniform(std::string attribute_name, const GLfloat _data)
{

	GLint uniform_id = findUniform(attribute_name);

	glUniform1f(uniform_id, _data);
	return true;
}


bool UniformAttribute::setFlt2Uniform(std::string attribute_name, const float data_x, const float data_y)
{

	GLint uniform_id = findUniform(attribute_name);


	glUniform2f(uniform_id, data_x, data_y);
	return true;
}


bool UniformAttribute::setFlt4Uniform(std::string attribute_name, const float data_x, const float data_y, const float data_z, const float data_w)
{

	GLint uniform_id = findUniform(attribute_name);

	glUniform4f(uniform_id, data_x, data_y, data_z, data_w);
	return true;
}

bool UniformAttribute::setMat4Uniform(std::string attribute_name, const glm::mat4 trans) {
	GLint uniform_id = findUniform(attribute_name);

	if (uniform_id < 0) return false;
	glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(trans));
	return true;
}


bool UniformAttribute::setFltArrayUniform(std::string attribute_name, const float* _data, const int vectex_num)
{
	GLint uniform_id = findUniform(attribute_name);
#if  MAP
	glUniform1fv(m_attributes[attribute_name].attribute_id, vectex_num, _data);
#else
	glUniform1fv(uniform_id, vectex_num, _data);
#endif //  MAP
	return true;
}

GLint UniformAttribute::findAttribute(std::string attribute_name)
{
#if MAP
	if (m_attributes.find(attribute_name) == m_attributes.end())
	{
		printf("the attribute named %s has not been initialized\n", attribute_name.c_str());
		m_attributes[attribute_name].attribute_id = GetAttributeID(m_ProgramHandle, attribute_name);
	}
	return m_attributes[attribute_name].attribute_id;
#else
	return GetAttributeID(m_ProgramHandle, attribute_name);
#endif
}


bool UniformAttribute::setVectexAttribute(std::string attribute_name, const GLint size, const int vectex_num, const float* _data)
{
	GLint attri_id = findAttribute(attribute_name);

	glEnableVertexAttribArray(attri_id);
	glVertexAttribPointer(attri_id, size, GL_FLOAT, GL_FALSE, vectex_num, _data);
#if MAP
	m_attributes[attribute_name].bNeedDisable = true;
#endif
	return true;
}


bool UniformAttribute::setTextureCoordsAttribute(std::string attribute_name, const float* _data, const GLint size, const int vectex_num)
{
	GLint attri_id = findAttribute(attribute_name);

	glEnableVertexAttribArray(attri_id);
	glVertexAttribPointer(attri_id, size, GL_FLOAT, GL_FALSE, 0, _data);
#if MAP
	m_attributes[attribute_name].bNeedDisable = true;
#endif
	return true;
}



bool UniformAttribute::disableVertexAttribArray(std::string attribute_name)
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

