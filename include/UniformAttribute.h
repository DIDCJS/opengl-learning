#pragma once

#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>
#include "Program.h"

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class UniformAttribute : public Program{
public:
	GLint findUniform(std::string attribute_name);
	GLint findAttribute(std::string attribute_name);
	bool setTextureID(std::string attribute_name, const GLenum texture_slot, const int texture_local_id, const GLint textureID, const bool IsExtTexture);
	bool setIntUniform(std::string attribute_name, const GLint _data);
	bool setFltUniform(std::string attribute_name, const GLfloat _data);
	bool setFlt2Uniform(std::string attribute_name, const float data_x, const float data_y);
	bool setFlt4Uniform(std::string attribute_name, const float data_x, const float data_y, const float data_z, const float data_w);
	bool setMat4Uniform(std::string attribute_name, const glm::mat4 trans);
	bool setVectexAttribute(std::string attribute_name, const GLint size, const int vectex_num, const float* _data);
	bool setTextureCoordsAttribute(std::string attribute_name, const float* _data, const GLint size, const int vectex_num);
	bool setFltArrayUniform(std::string attribute_name, const float* _data, const int vectex_num);
	bool disableVertexAttribArray(std::string attribute_name);


};