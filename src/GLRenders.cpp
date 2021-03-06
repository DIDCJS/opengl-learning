
#include "GLRenders.h"


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
