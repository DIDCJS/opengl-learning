#include <iostream>
#include "LearnGL.h"
#include "GLShaders.h"
#include "GLDefine.h"


//glm
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>

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


	//diffuses map , spercular map
	cv::Mat diffuseImg = cv::imread(IMG_PATH"diffuse.png");
	cv::Mat spercularImg = cv::imread(IMG_PATH"color-specular.png");
	if (diffuseImg.channels() == 3) {
		cv::cvtColor(diffuseImg, diffuseImg, cv::COLOR_BGR2RGBA);
		cv::cvtColor(spercularImg, spercularImg, cv::COLOR_BGR2RGBA);
	}
	else if (diffuseImg.channels() == 4) {
		cv::cvtColor(diffuseImg, diffuseImg, cv::COLOR_BGRA2RGBA);
		cv::cvtColor(spercularImg, spercularImg, cv::COLOR_BGRA2RGBA);
	}
	CreateTexture(m_TexImages[TEXTURE1], diffuseImg.cols, diffuseImg.rows, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, diffuseImg.data);
	CreateTexture(m_TexImages[TEXTURE2], spercularImg.cols, spercularImg.rows, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, spercularImg.data);
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
		_render[SHADER_LEARN].setVectexAttribute("aPos", 3, 8 * sizeof(float), (const float*)(0 * sizeof(float)));
		_render[SHADER_LEARN].setVectexAttribute("aNormal", 3, 8 * sizeof(float), (const float*)(3 * sizeof(float)));
		_render[SHADER_LEARN].setVectexAttribute("aTexCoords", 2, 8 * sizeof(float), (const float*)(6 * sizeof(float)));
	}

	glUseProgram(glProgram[SHADER_LEARN]);

	//uniform,attribute
	_render[SHADER_LEARN].setTextureID("u_diffuseMap", GL_TEXTURE1, 1, m_TexImages[TEXTURE1].glTexture, 0);
	_render[SHADER_LEARN].setTextureID("u_specularMap", GL_TEXTURE2, 2, m_TexImages[TEXTURE2].glTexture, 0);


	glEnable(GL_DEPTH_TEST);

	//objectColor viewPos
	_render[SHADER_LEARN].setFlt3Uniform("objectColor", 1.0f, 0.5f, 0.31f);
	_render[SHADER_LEARN].setFlt3Uniform("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

	//light struct
	_render[SHADER_LEARN].setFlt3Uniform("u_light.direction", camera.Front.r, camera.Front.g, camera.Front.b);
	_render[SHADER_LEARN].setFlt3Uniform("u_light.position", camera.Position.x, camera.Position.y, camera.Position.z);
	_render[SHADER_LEARN].setFltUniform("u_light.cutOff", glm::cos(glm::radians(12.5f)));
	_render[SHADER_LEARN].setFltUniform("u_light.outerCutOff", glm::cos(glm::radians(17.5f)));
	_render[SHADER_LEARN].setFlt3Uniform("u_light.ambient", 1.0f, 1.0f, 1.0f);
	_render[SHADER_LEARN].setFlt3Uniform("u_light.diffuse", 1.0f, 1.0f, 1.0f);
	_render[SHADER_LEARN].setFlt3Uniform("u_light.specular", 1.0f, 1.0f, 1.0f);
	//模拟光源衰变
	_render[SHADER_LEARN].setFltUniform("u_light.constant", 1.0f);
	_render[SHADER_LEARN].setFltUniform("u_light.linear", 0.09f);
	_render[SHADER_LEARN].setFltUniform("u_light.quadratic", 0.032f);

	//material struct
	_render[SHADER_LEARN].setFlt3Uniform("u_material.ambient", 0.329412	, 0.223529, 0.027451);
	_render[SHADER_LEARN].setFlt3Uniform("u_material.diffuse", 0.780392, 0.568627, 0.113725);
	_render[SHADER_LEARN].setFlt3Uniform("u_material.specular", 0.992157,0.941176, 0.807843);
	_render[SHADER_LEARN].setFltUniform("u_material.shininess", 32.0f);

	//model view projection
	glm::mat4 view = glm::mat4(1.0f);
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)nWidth / nHeight, 0.1f, 100.0f);
	_render[SHADER_LEARN].setMat4Uniform("view", view);
	_render[SHADER_LEARN].setMat4Uniform("projection", projection);

	//define 10 box medels
	glm::mat4 model;
	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	for (int i = 0; i < 10; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		_render[SHADER_LEARN].setMat4Uniform("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


	//Light
	//COMPLIE SHADER

	glProgram[SHADER_LIGHT] = GLShaders::CreateProgram_Source(VertexShaderSource[SHADER_LIGHT], FragmentShaderSource[SHADER_LIGHT]);
	_render[SHADER_LIGHT].setProgramHandle(glProgram[SHADER_LIGHT]);

	glBindVertexArray(LightVAO);

	if (firstDraw == true) {
		// 位置属性
		_render[SHADER_LIGHT].setVectexAttribute("aPos", 3, 8 * sizeof(float), (const float*)(0 * sizeof(float)));
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

	//glDrawArrays(GL_TRIANGLES, 0, 36);
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