#include <iostream>
#include <direct.h>

#include "LearnGL.h"
#include "GLShaders.h"
#include "GLDefine.h"

//glm
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>

#include <Mesh.h>


const float PI = 3.141592653;
#define NR_POINT_LIGHTS 4
#define RENDER_FROM_MODEL 1

void LearnGL::Init(unsigned char* pT0, int nT0W, int nT0H, int nT0C,
	unsigned char* pT1, int nT1W, int nT1H, int nT1C/*,
		unsigned char* pT2, int nT2W, int nt2H, int nT2C*/) {


	glProgram = new GLuint[NUM_SHADERS];
	_render = new GLRenders[NUM_SHADERS];
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &LightVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	//CreateTexture(m_TexImages[TEXTURE0], nT0W, nT0H, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT0);
	//CreateTexture(m_TexImages[TEXTURE1], nT1W, nT1W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT1);
	//CreateTexture(m_TexImages[TEXTURE2], nT2W, nT2W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT2);


	//diffuses map , spercular map
	cv::Mat diffuseImg = cv::imread(IMG_PATH"diffuse.png");
	cv::Mat spercularImg = cv::imread(IMG_PATH"specular.png");
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

	LoadModel(R"(model/nanosuit/nanosuit.obj)");
}

void LearnGL::LearnGL_Main(int nWidth, int nHeight, Camera& camera, float fov) {
#if RENDER_FROM_MODEL == 1
	Draw(nWidth, nHeight, camera, fov);
#else


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

	glProgram[SHADER_LEARN] = GLShaders::CreateProgram_Source(
		GLShaders::LoadShaderPath(VertexShaderPath[SHADER_LEARN]), GLShaders::LoadShaderPath(FragmentShaderPath[SHADER_LEARN]) );
	_render[SHADER_LEARN].setProgramHandle(glProgram[SHADER_LEARN]);


	glUseProgram(glProgram[SHADER_LEARN]);

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
	glEnable(GL_DEPTH_TEST);

	//objectColor viewPos
	_render[SHADER_LEARN].setFlt3Uniform("objectColor", 1.0f, 0.5f, 0.31f);
	_render[SHADER_LEARN].setFlt3Uniform("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

	//DirLight
	/*_render[SHADER_LEARN].setFlt3Uniform("dirLight.direction", camera.Front.r, camera.Front.g, camera.Front.b);
	_render[SHADER_LEARN].setFlt3Uniform("dirLight.ambient", 1.0f, 1.0f, 1.0f);
	_render[SHADER_LEARN].setFlt3Uniform("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
	_render[SHADER_LEARN].setFlt3Uniform("dirLight.specular", 1.0f, 1.0f, 1.0f);*/
	//定灯源
	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};
	
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		std::string IndexPointLight = "pointLights[";
		IndexPointLight += std::to_string(i);
		IndexPointLight += "].";
		_render[SHADER_LEARN].setFlt3Uniform(IndexPointLight + "position", pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
		_render[SHADER_LEARN].setFlt3Uniform(IndexPointLight + "ambient", 1.0f, 1.0f, 1.0f);
		_render[SHADER_LEARN].setFlt3Uniform(IndexPointLight + "diffuse", 1.0f, 1.0f, 1.0f);
		_render[SHADER_LEARN].setFlt3Uniform(IndexPointLight + "specular", 1.0f, 1.0f, 1.0f);
		_render[SHADER_LEARN].setFltUniform(IndexPointLight + "constant", 1.0f);
		_render[SHADER_LEARN].setFltUniform(IndexPointLight + "linear", 0.09f); 
		_render[SHADER_LEARN].setFltUniform(IndexPointLight + "quadratic", 0.032f);
	}

	//spotLight
	_render[SHADER_LEARN].setFlt3Uniform("spotLight.direction", camera.Front.r, camera.Front.g, camera.Front.b);
	_render[SHADER_LEARN].setFlt3Uniform("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
	_render[SHADER_LEARN].setFltUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	_render[SHADER_LEARN].setFltUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	_render[SHADER_LEARN].setFlt3Uniform("spotLight.ambient", 1.0f, 1.0f, 1.0f);
	_render[SHADER_LEARN].setFlt3Uniform("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	_render[SHADER_LEARN].setFlt3Uniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
	_render[SHADER_LEARN].setFltUniform("spotLight.constant", 1.0f);
	_render[SHADER_LEARN].setFltUniform("spotLight.linear", 0.09f);
	_render[SHADER_LEARN].setFltUniform("spotLight.quadratic", 0.032f);

	//material struct
	_render[SHADER_LEARN].setFlt3Uniform("material.ambient", 0.329412	, 0.223529, 0.027451);
	_render[SHADER_LEARN].setFltUniform("material.shininess", 32.0f);
	_render[SHADER_LEARN].setTextureID("material.diffuse", GL_TEXTURE1, 1, m_TexImages[TEXTURE1].glTexture, 0);
	_render[SHADER_LEARN].setTextureID("material.specular", GL_TEXTURE2, 2, m_TexImages[TEXTURE2].glTexture, 0);


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
	glProgram[SHADER_LIGHT] = GLShaders::CreateProgram_Source(
		GLShaders::LoadShaderPath(VertexShaderPath[SHADER_LIGHT]), GLShaders::LoadShaderPath(FragmentShaderPath[SHADER_LIGHT]));
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
	_render[SHADER_LIGHT].setMat4Uniform("view", view);
	_render[SHADER_LIGHT].setMat4Uniform("projection", projection);
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		_render[SHADER_LIGHT].setMat4Uniform("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
#endif
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

	delete m_pScene;
}



void LearnGL::Draw(int nWidth, int nHeight, Camera& camera, float fov) {

	


	glProgram[SHADER_MESH] = GLShaders::CreateProgram_Source(
		GLShaders::LoadShaderPath(VertexShaderPath[SHADER_MESH]), GLShaders::LoadShaderPath(FragmentShaderPath[SHADER_MESH]));
	_render[SHADER_MESH].setProgramHandle(glProgram[SHADER_MESH]);

	glUseProgram(glProgram[SHADER_MESH]);

	//model view projection
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	glm::mat4 view = glm::mat4(1.0f);
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)nWidth / nHeight, 0.1f, 100.0f);
	_render[SHADER_MESH].setMat4Uniform("view", view);
	_render[SHADER_MESH].setMat4Uniform("projection", projection);
	_render[SHADER_MESH].setMat4Uniform("model", model);

	glBindVertexArray(VAO);

	//定灯源
	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
	};

	for (int i = 0; i < 1; i++) {
		std::string IndexPointLight = "pointLights[";
		IndexPointLight += std::to_string(i);
		IndexPointLight += "].";
		_render[SHADER_MESH].setFlt3Uniform(IndexPointLight + "position", pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
		_render[SHADER_MESH].setFlt3Uniform(IndexPointLight + "ambient", 1.0f, 1.0f, 1.0f);
		_render[SHADER_MESH].setFlt3Uniform(IndexPointLight + "diffuse", 1.0f, 1.0f, 1.0f);
		_render[SHADER_MESH].setFlt3Uniform(IndexPointLight + "specular", 1.0f, 1.0f, 1.0f);
		_render[SHADER_MESH].setFltUniform(IndexPointLight + "constant", 1.0f);
		_render[SHADER_MESH].setFltUniform(IndexPointLight + "linear", 0.14f);
		_render[SHADER_MESH].setFltUniform(IndexPointLight + "quadratic", 0.07f);
	}

	//viewPos 用于与光源点算向量
	_render[SHADER_MESH].setFlt3Uniform("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

	//printf("### m_vMesh.size() : %d\n", m_vMesh.size());
	for (int i = 0; i < m_vMesh.size(); i++) {
		Mesh mesh = m_vMesh[i];

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.vertices.size(), &(mesh.vertices[0]), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), &(mesh.indices[0]), GL_STATIC_DRAW);

		_render[SHADER_MESH].setVectexAttribute("aPos", 3, sizeof(Vertex), (const float*)(0 * sizeof(float)));
		_render[SHADER_MESH].setVectexAttribute("aNormal", 3, sizeof(Vertex), (const float*)(3 * sizeof(float)));
		_render[SHADER_MESH].setVectexAttribute("aTexCoords", 2, sizeof(Vertex), (const float*)(6 * sizeof(float)));

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		//printf("### mesh.textures.size() : %d\n", mesh.textures.size());
		for (unsigned int i = 0; i < mesh.textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // 在绑定之前激活相应的纹理单元
			// 获取纹理序号（diffuse_textureN 中的 N）
			std::string number;
			std::string name = mesh.textures[i].textureType;
			if (name == "texture_diffuse") {
				number = std::to_string(diffuseNr++);
				_render[SHADER_MESH].setTextureID(name + number, GL_TEXTURE0 + i, i, mesh.textures[i].glTexture, false);
			}
			else if (name == "texture_specular") {
				//printf("### now the texture name is texture_specular\n");
				number = std::to_string(specularNr++);
			}
			
		}

		glDrawElements(GL_TRIANGLES, mesh.indices.size() , GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}

void LearnGL::LoadModel(std::string path) {
	Assimp::Importer import;
	m_pScene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!m_pScene || m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_pScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(m_pScene->mRootNode, m_pScene);
}
void LearnGL::ProcessNode(aiNode *node, const aiScene *scene) {
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_vMesh.push_back(ProcessMesh(mesh, scene));
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}


std::vector<TexImage> LearnGL::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<TexImage> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		TexImage TexImage;
		std::string path = str.C_Str();
		TextureFromFile(path, TexImage);
		TexImage.textureType = typeName;
		textures.push_back(TexImage);
	}
	return textures;
}

Mesh LearnGL::ProcessMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TexImage> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//
		Vertex vertex;
		// 处理顶点位置、法线和纹理坐标
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vertices.push_back(vertex);

		//printf("Position[%d].x : %.3f\n", i, vertex.Position.x);
		//printf("Position[%d].y : %.3f\n", i, vertex.Position.y);
		//printf("Position[%d].z : %.3f\n", i, vertex.Position.z);

	}
	// 处理索引
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		// 处理材质
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<TexImage> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<TexImage> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);

}