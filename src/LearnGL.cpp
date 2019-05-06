#include <iostream>
#include "LearnGL.h"
#include "GLShaders.h"
#include "GLDefine.h"


//glm
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

const float PI = 3.141592653;

void LearnGL::Init(unsigned char* pT0, int nT0W, int nT0H, int nT0C,
	unsigned char* pT1, int nT1W, int nT1H, int nT1C/*,
		unsigned char* pT2, int nT2W, int nt2H, int nT2C*/) {


	glProgram = new GLuint[NUM_SHADERS];
	_render = new GLRenders[NUM_SHADERS];
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &LightVAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);


	//CreateTexture(m_TexImages[TEXTURE0], nT0W, nT0H, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT0);
	//CreateTexture(m_TexImages[TEXTURE1], nT1W, nT1W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT1);
	//CreateTexture(m_TexImages[TEXTURE2], nT2W, nT2W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT2);
}



void LearnGL::LearnGL_Main(int nWidth, int nHeight, Camera& camera, float fov) {

	glm::vec3 lightPos = glm::vec3(2.0f, 1.0f, 2.0f);
	glm::vec3 newLightPos = glm::vec3(2.0f, 1.0f, 2.0f);
	float precent = glfwGetTime() / 5.0f;
	precent -= (int)precent;
	precent *= 2 * PI;
	float R = sqrt((lightPos.x * lightPos.x) + ((lightPos.y * lightPos.y)));
	newLightPos.z = sin(precent) * R;
	newLightPos.x = cos(precent) * R;
	lightPos = newLightPos;
	//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
	//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;
	//std::cout << precent << std::endl;
	//COMPLIE SHADER

	glProgram[SHADER_LEARN] = GLShaders::CreateProgram_Source(VertexShaderSource[SHADER_LEARN], FragmentShaderSource[SHADER_LEARN]);
	_render[SHADER_LEARN].setProgramHandle(glProgram[SHADER_LEARN]);


	//BUFFER

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	if (firstDraw == true) {
		// 位置属性
		_render[SHADER_LEARN].setVectexAttribute("aPos", 3, 6 * sizeof(float), (const float*)(0 * sizeof(float)));
		_render[SHADER_LEARN].setVectexAttribute("aNormal", 3, 6 * sizeof(float), (const float*)(3 * sizeof(float)));
	}

	glUseProgram(glProgram[SHADER_LEARN]);

	//uniform,attribute
	//_render[SHADER_LEARN].setTextureID("ourTexture0", GL_TEXTURE0, 0, m_TexImages[TEXTURE0].glTexture, 0);
	//_render[SHADER_LEARN].setTextureID("ourTexture2", GL_TEXTURE1, 1, m_TexImages[TEXTURE1].glTexture, 0);


	glEnable(GL_DEPTH_TEST);

	//model view projection
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)nWidth / nHeight, 0.1f, 100.0f);
	_render[SHADER_LEARN].setMat4Uniform("model", model);
	_render[SHADER_LEARN].setMat4Uniform("view", view);
	_render[SHADER_LEARN].setMat4Uniform("projection", projection);

	//lightColor objectColor lightPos
	_render[SHADER_LEARN].setFlt4Uniform("lightPos", lightPos.x, lightPos.y, lightPos.z, 1.0f);
	_render[SHADER_LEARN].setFlt4Uniform("lightColor", 1.0f, 1.0f, 1.0f, 1.0f);
	_render[SHADER_LEARN].setFlt4Uniform("objectColor", 1.0f, 0.5f, 0.31f, 1.0f);
	_render[SHADER_LEARN].setFlt4Uniform("viewPos", camera.Position.x, camera.Position.y, camera.Position.z, 1.0f);
	std::cout << "x: " << camera.Position.x << std::endl;
	std::cout << "y: " << camera.Position.y << std::endl;
	std::cout << "z: " << camera.Position.z << std::endl;
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Light
	//COMPLIE SHADER

	glProgram[SHADER_LIGHT] = GLShaders::CreateProgram_Source(VertexShaderSource[SHADER_LIGHT], FragmentShaderSource[SHADER_LIGHT]);
	_render[SHADER_LIGHT].setProgramHandle(glProgram[SHADER_LIGHT]);

	glBindVertexArray(LightVAO);

	if (firstDraw == true) {
		// 位置属性
		_render[SHADER_LIGHT].setVectexAttribute("aPos", 3, 6 * sizeof(float), (const float*)(0 * sizeof(float)));
		//纹理坐标
		//_render[SHADER_LEARN].setVectexAttribute("aTexCoord", 2, 5 * sizeof(float), (const float*)(3 * sizeof(float)));
		firstDraw = false;
	}
	glUseProgram(glProgram[SHADER_LIGHT]);
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
	_render[SHADER_LIGHT].setMat4Uniform("model", model);
	_render[SHADER_LIGHT].setMat4Uniform("view", view);
	_render[SHADER_LIGHT].setMat4Uniform("projection", projection);

	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void LearnGL::Release() {
	delete glProgram;
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (LightVAO != 0) glDeleteVertexArrays(1, &LightVAO);
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