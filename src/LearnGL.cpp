#include <iostream>
#include "LearnGL.h"
#include "GLShaders.h"
#include "GLDefine.h"

//glm
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <GLFW/glfw3.h>

void LearnGL::Init(unsigned char* pT0, int nT0W, int nT0H, int nT0C,
								unsigned char* pT1, int nT1W, int nT1H, int nT1C/*,
									unsigned char* pT2, int nT2W, int nt2H, int nT2C*/) {
	glProgram = new GLuint[NUM_SHADERS];
	_render = new GLRenders[NUM_SHADERS];
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);
	
	
	//CreateTexture(m_TexImages[TEXTURE0], nT0W, nT0W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT0);
	//CreateTexture(m_TexImages[TEXTURE1], nT1W, nT1W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT1);
	//CreateTexture(m_TexImages[TEXTURE2], nT2W, nT2W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT2);
}	



void LearnGL::LearnGL_Main() {
	//COMPLIE SHADER
	{
		glProgram[SHADER_LEARN] = GLShaders::CreateProgram_Source(VertexShaderSource[SHADER_LEARN], FragmentShaderSource[SHADER_LEARN]);
		_render[SHADER_LEARN].setProgramHandle(glProgram[SHADER_LEARN]);
	}

	//BUFFER
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_learn), vertices_learn, GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// 位置属性
		_render[SHADER_LEARN].setVectexAttribute("aPos", 3, 3 * sizeof(float), (const float*)(0 * sizeof(float)));
		//_render[SHADER_LEARN].setVectexAttribute("aPos", 3, 8 * sizeof(float), (const float*)(0 * sizeof(float)));
		//纹理坐标
		//_render[SHADER_LEARN].setVectexAttribute("aTexCoord", 2, 8 * sizeof(float), (const float*)(6 * sizeof(float)));
	}
	
	glUseProgram(glProgram[SHADER_LEARN]);

	//uniform,attribute
	{
#if 0 
		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		glm::mat4 view;
		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));


		glm::mat4 trans;
		trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		_render[SHADER_LEARN].setMat4Uniform("transform", trans);
#endif


		//_render[SHADER_LEARN].setTextureID("ourTexture1", GL_TEXTURE0, 0, m_TexImages[TEXTURE0].glTexture, 0);
		//_render[SHADER_LEARN].setTextureID("ourTexture2", GL_TEXTURE1, 1, m_TexImages[TEXTURE1].glTexture, 0);
	}

	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void LearnGL::Release() {
	delete glProgram;
	if(VAO!=0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);

	delete[] _render;
	for (int i = 0; i < NUM_SHADERS; i++) {
		glDeleteProgram(glProgram[i]);
	}
	delete glProgram;

	for (int i = 0; i < NUM_TEXTURES; ++i)
	{
		if (m_TexImages[i].glTexture != 0)
		{
			glDeleteTextures(1, &m_TexImages[i].glTexture);
			m_TexImages[i].glTexture = 0;
		}
	}
}