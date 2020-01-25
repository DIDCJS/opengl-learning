#include <cstdlib>
#include "GLShaders.h"
#include "GLDefine.h"
#include "glad/glad.h"
#include <cstdio>

GLuint GLShaders::LoadShader_Source(GLenum shaderType, const char* pSource)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader) {
		glShaderSource(shader, 1, &pSource, NULL);
		glCompileShader(shader);
		GLint compiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
		if (!compiled) {
			GLint infoLen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
			if (infoLen) {
				char* buf = (char*)malloc(infoLen);
				if (buf) {
					glGetShaderInfoLog(shader, infoLen, NULL, buf);
					LOGE("Could not compile shader %d:%s",
						shaderType, buf);
					free(buf);
				}
				glDeleteShader(shader);
				shader = 0;
			}
		}
	}
	return shader;
}

GLuint GLShaders::CreateProgram_Source(const char* pVertexSource, const char* pFragmentSource)
{
	GLuint vertexShader = LoadShader_Source(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader)
	{
		LOGE("ERROR in load vertex shader!");
		return 0;
	}
	GLuint pixelShader = LoadShader_Source(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader)
	{
		LOGE("ERROR in load pixel shader!");
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program)
	{
		glAttachShader(program, vertexShader);

		glAttachShader(program, pixelShader);

		glLinkProgram(program);
		GLint linkStatus = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
		if (linkStatus != GL_TRUE)
		{
			GLint bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
			if (bufLength)
			{
				char* buf = (char*)malloc(bufLength);
				if (buf)
				{
					glGetProgramInfoLog(program, bufLength, NULL, buf);
					LOGE("Could not link program:\n%s", buf);
					free(buf);
				}
			}
			glDeleteProgram(program);
			program = 0;
		}
	}
	if (vertexShader)
	{
		 (vertexShader);
		vertexShader = 0;
	}
	if (pixelShader)
	{
		glDeleteShader(pixelShader);
		pixelShader = 0;
	}

	return program;
}

const char* GLShaders::LoadShaderPath(const char* path) {
	FILE* pFile;
	pFile = fopen(path, "rb");

	if (pFile == NULL)
	{
		LOGE("###%s was not found");
		return NULL;
	}
	fseek(pFile, 0, SEEK_END);
	int len = ftell(pFile);
	char* str = new char[len + 1];
	if (str == NULL)
	{
		return NULL;
	}
	fseek(pFile, 0, SEEK_SET);
	fread(str, len, 1, pFile);
	str[len] = '\0';
	fclose(pFile);
	return str;
}