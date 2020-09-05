
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

#include <iostream>
//#include <direct.h>



const float PI = 3.141592653;
#define RENDER_FROM_MODEL 1
#define NEED_POINT_LIGHT
//#define NEED_SPOT_LIGHT
//#define NEED_DIR_LIGHT

void LearnGL::Init(unsigned char* pT0, int nT0W, int nT0H, int nT0C,
	unsigned char* pT1, int nT1W, int nT1H, int nT1C/*,
		unsigned char* pT2, int nT2W, int nt2H, int nT2C*/) {


	glProgram = new GLuint[NUM_SHADERS];
	_render = new GLRenders[NUM_SHADERS];
	memset(glProgram, 0, sizeof(GLuint)* NUM_SHADERS);
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &LightVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


//	CreateTexture(m_TexImages[TEXTURE0], nT0W, nT0H, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT0);
	//CreateTexture(m_TexImages[TEXTURE1], nT1W, nT1W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT1);
	//CreateTexture(m_TexImages[TEXTURE2], nT2W, nT2W, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, pT2);


	//diffuses map , spercular map
	cv::Mat img1 = cv::imread(IMG_PATH"brickwall.jpg", cv::IMREAD_COLOR);
	cv::Mat img2 = cv::imread(IMG_PATH"brickwall_normal.jpg", cv::IMREAD_COLOR);
    cvtColor(img1, img1, CV_BGR2RGBA);
    cvtColor(img2, img2, CV_BGR2RGBA);
	CreateTexture(m_TexImages[TEXTURE1], img1.cols, img1.rows, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, img1.data);
	CreateTexture(m_TexImages[TEXTURE2], img2.cols, img2.rows, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, img2.data);
	CreateCuteTexture(m_TexImages[TEXTURE3], GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA, GL_LINEAR, GL_LINEAR, IMG_PATH  R"(skybox/skybox_0/)");

	LoadModel(MODEL_PATH);
}

void LearnGL::Release() {
	delete[] glProgram;
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (LightVAO != 0) glDeleteVertexArrays(1, &LightVAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);

	delete[] _render;
	for (int i = 0; i < NUM_SHADERS; i++) {
		glDeleteProgram(glProgram[i]);
	}
	delete[] glProgram;

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

void LearnGL::LearnGL_Main(int nWidth, int nHeight, Camera& camera, float fov) {
#if RENDER_FROM_MODEL == 0
//	Draw3DMesh(nWidth, nHeight, camera, fov);
#else
	Draw3D(nWidth, nHeight, camera, fov);
//    DrawTest2D(nWidth, nHeight);
#endif
//	DrawCute(nWidth, nHeight, camera, fov);
}

void LearnGL::DrawLight(int nWidth, int nHeight, Camera& camera, float fov,
											glm::mat4 view, glm::mat4 projection, glm::vec3* pointLightPositions, int nPointSize) {
	//Light
//COMPLIE SHADER
	glProgram[SHADER_LIGHT] = GLShaders::CreateProgram_Source(
		GLShaders::LoadShaderPath(VertexShaderPath[SHADER_LIGHT]), GLShaders::LoadShaderPath(FragmentShaderPath[SHADER_LIGHT]));
	auto& nowRender = _render[SHADER_LIGHT];
	nowRender.setProgramHandle(glProgram[SHADER_LIGHT]);
	nowRender.Use();

	glBindVertexArray(LightVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	if (firstDraw == true) {
		// λ������
		nowRender.setVectexAttribute("aPos", 3, 8 * sizeof(float), (const float*)(0 * sizeof(float)));
		nowRender.setVectexAttribute("aNormal", 3, 8 * sizeof(float), (const float*)(3 * sizeof(float)));
		//��������
		//_render[SHADER_LEARN].setVectexAttribute("aTexCoord", 2, 5 * sizeof(float), (const float*)(3 * sizeof(float)));
		firstDraw = false;
	}
	nowRender.setTextureID("skybox", GL_TEXTURE0, 0, m_TexImages[TEXTURE3].glTexture, false);
	//model view projection
	nowRender.setMat4Uniform("view", view);
	nowRender.setMat4Uniform("projection", projection);
	nowRender.setFlt3Uniform("cameraPos", camera.Position.x, camera.Position.y, camera.Position.z);
	glm::mat4 model;
	for (int i = 0; i < nPointSize; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		nowRender.setMat4Uniform("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void LearnGL::Draw3DMesh(int nWidth, int nHeight, Camera& camera, float fov) {
	glProgram[SHADER_MESH] = GLShaders::CreateProgram_Source(
		GLShaders::LoadShaderPath(VertexShaderPath[SHADER_MESH]), GLShaders::LoadShaderPath(FragmentShaderPath[SHADER_MESH]));
	auto &nowRender = _render[SHADER_MESH];
	nowRender.setProgramHandle(glProgram[SHADER_MESH]);
	nowRender.Use();


	//model view projection
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	glm::mat4 view = glm::mat4(1.0f);
	view = camera.GetViewMatrix();
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)nWidth / nHeight, 0.1f, 100.0f);
	nowRender.setMat4Uniform("view", view);
	nowRender.setMat4Uniform("projection", projection);
	nowRender.setMat4Uniform("model", model);

	glBindVertexArray(VAO);

	//����Դ
	glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	//glm::vec3(2.3f, -3.3f, -4.0f),
	//glm::vec3(-4.0f,  2.0f, -12.0f),
	//glm::vec3(0.0f,  0.0f, -3.0f)
	};
	int nPointSize = sizeof(pointLightPositions) / sizeof(glm::vec3);
	std::cout << nPointSize << std::endl;

	for (int i = 0; i < nPointSize; i++) {
		std::string IndexPointLight = "pointLights[";
		IndexPointLight += std::to_string(i);
		IndexPointLight += "].";
		nowRender.setFlt3Uniform(IndexPointLight + "position", pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
		nowRender.setFlt3Uniform(IndexPointLight + "ambient", 1.0f, 1.0f, 1.0f);
		nowRender.setFlt3Uniform(IndexPointLight + "diffuse", 1.0f, 1.0f, 1.0f);
		nowRender.setFlt3Uniform(IndexPointLight + "specular", 1.0f, 1.0f, 1.0f);
		nowRender.setFltUniform(IndexPointLight + "constant", 1.0f);
		nowRender.setFltUniform(IndexPointLight + "linear", 0.14f);
		nowRender.setFltUniform(IndexPointLight + "quadratic", 0.07f);
	}

	//viewPos �������Դ��������
	nowRender.setFlt3Uniform("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

	//LOG("### m_vMesh.size() : %d", m_vMesh.size());
	for (int i = 0; i < m_vMesh.size(); i++) {
		Mesh mesh = m_vMesh[i];

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mesh.vertices.size(), &(mesh.vertices[0]), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.indices.size(), &(mesh.indices[0]), GL_STATIC_DRAW);

		nowRender.setVectexAttribute("aPos", 3, sizeof(Vertex), (const float*)(0 * sizeof(float)));
		nowRender.setVectexAttribute("aNormal", 3, sizeof(Vertex), (const float*)(3 * sizeof(float)));
		nowRender.setVectexAttribute("aTexCoords", 2, sizeof(Vertex), (const float*)(6 * sizeof(float)));

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		//LOG("### mesh.textures.size() : %d", mesh.textures.size());
		for (unsigned int i = 0; i < mesh.textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // �ڰ�֮ǰ������Ӧ������Ԫ
			// ��ȡ������ţ�diffuse_textureN �е� N��
			std::string number;
			std::string name = mesh.textures[i].textureType;
			if (name == "texture_diffuse") {
				number = std::to_string(diffuseNr++);
				nowRender.setTextureID(name + number, GL_TEXTURE0 + i, i, mesh.textures[i].glTexture, false);
			}
			else if (name == "texture_specular") {
				//LOG("### now the texture name is texture_specular");
				number = std::to_string(specularNr++);
			}
			
		}

		glDrawElements(GL_TRIANGLES, mesh.indices.size() , GL_UNSIGNED_INT, 0);

		//glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	DrawLight(nWidth, nHeight, camera, fov, view, projection, pointLightPositions, nPointSize);
}

void LearnGL::Draw3D(int nWidth, int nHeight, Camera& camera, float fov){
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

    if(glProgram[SHADER_LEARN] <= GL_FALSE) {
        glProgram[SHADER_LEARN] = GLShaders::CreateProgram_Source(
                GLShaders::LoadShaderPath(VertexShaderPath[SHADER_LEARN]),
                GLShaders::LoadShaderPath(FragmentShaderPath[SHADER_LEARN]), "");
    }
    auto &nowRender = _render[SHADER_MESH];
    nowRender.setProgramHandle(glProgram[SHADER_LEARN]);
    nowRender.Use();

    //BUFFER
    glBindVertexArray(VAO);

//    int nSize = 0;
//    float* pVertices = GetVerties(nSize);
//
//    for(int i = 0 ; i< nSize ;i++){
//        printf("%.3f, ",pVertices[i]);
//    }
//    printf("\n");


    glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
    glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


    float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, nSize, pVertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_matrix), indices_matrix, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    if (firstDraw == true) {
        // λ������
        int numPoint = 14;
        nowRender.setVectexAttribute("aPos", 3, numPoint * sizeof(float), (const float*)(0 * sizeof(float)));
        nowRender.setVectexAttribute("aNormal", 3, numPoint * sizeof(float), (const float*)(3 * sizeof(float)));
        nowRender.setVectexAttribute("aTexCoords", 2, numPoint * sizeof(float), (const float*)(6 * sizeof(float)));
        nowRender.setVectexAttribute("aTangent", 3, numPoint * sizeof(float), (const float*)(8 * sizeof(float)));
        nowRender.setVectexAttribute("aBitangent", 3, numPoint * sizeof(float), (const float*)(11 * sizeof(float)));
    }
//    glEnable(GL_DEPTH_TEST);

    //objectColor viewPos
    nowRender.setFlt3Uniform("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

#ifdef NEED_DIR_LIGHT
    _render[SHADER_LEARN].setFlt3Uniform("dirLight.direction", camera.Front.r, camera.Front.g, camera.Front.b);
    _render[SHADER_LEARN].setFlt3Uniform("dirLight.ambient", 1.0f, 1.0f, 1.0f);
    _render[SHADER_LEARN].setFlt3Uniform("dirLight.diffuse", 1.0f, 1.0f, 1.0f);
    _render[SHADER_LEARN].setFlt3Uniform("dirLight.specular", 1.0f, 1.0f, 1.0f);
#endif

#ifdef NEED_POINT_LIGHT
    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f,  0.2f,  0.1f),
    };
    int nPointSize = sizeof(pointLightPositions) / sizeof(glm::vec3);
    for (int i = 0; i < nPointSize; i++) {
        std::string IndexPointLight = "pointLights[";
        IndexPointLight += std::to_string(i);
        IndexPointLight += "].";
        nowRender.setFlt3Uniform(IndexPointLight + "position", pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
        nowRender.setFlt3Uniform(IndexPointLight + "ambient", 1.0f, 1.0f, 1.0f);
        nowRender.setFlt3Uniform(IndexPointLight + "diffuse", 1.0f, 1.0f, 1.0f);
        nowRender.setFlt3Uniform(IndexPointLight + "specular", 1.0f, 1.0f, 1.0f);
        nowRender.setFltUniform(IndexPointLight + "constant", 1.0f);
        nowRender.setFltUniform(IndexPointLight + "linear", 0.09f);
        nowRender.setFltUniform(IndexPointLight + "quadratic", 0.032f);
    }
#endif

#ifdef NEED_SPOT_LIGHT
    nowRender.setFlt3Uniform("spotLight.direction", camera.Front.r, camera.Front.g, camera.Front.b);
    nowRender.setFlt3Uniform("spotLight.position", camera.Position.x, camera.Position.y, camera.Position.z);
    nowRender.setFltUniform("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    nowRender.setFltUniform("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    nowRender.setFlt3Uniform("spotLight.ambient", 1.0f, 1.0f, 1.0f);
    nowRender.setFlt3Uniform("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    nowRender.setFlt3Uniform("spotLight.specular", 1.0f, 1.0f, 1.0f);
    nowRender.setFltUniform("spotLight.constant", 1.0f);
    nowRender.setFltUniform("spotLight.linear", 0.09f);
    nowRender.setFltUniform("spotLight.quadratic", 0.032f);
#endif

    //material struct
    nowRender.setFlt3Uniform("material.ambient", 0.329412, 0.223529, 0.027451);
    nowRender.setFltUniform("material.shininess", 32.0f);
    nowRender.setTextureID("material.diffuse", GL_TEXTURE1, 1, m_TexImages[TEXTURE1].glTexture, 0);
    nowRender.setTextureID("material.normal", GL_TEXTURE2, 2, m_TexImages[TEXTURE2].glTexture, 0);


    //model view projection
    glm::mat4 view = glm::mat4(1.0f);
    view = camera.GetViewMatrix();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(fov), (float)nWidth / nHeight, 0.1f, 100.0f);
    nowRender.setMat4Uniform("view", view);
    nowRender.setMat4Uniform("projection", projection);
//    glm::mat4 model;
//    model = glm::mat4(1.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model,  -10.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    nowRender.setMat4Uniform("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 6);
//    glDrawElements(GL_TRIANGLES, sizeof(indices_matrix)/sizeof(indices_matrix[0]) , GL_UNSIGNED_INT, 0);

	DrawLight(nWidth, nHeight, camera, fov, view, projection, pointLightPositions, nPointSize);


}

void LearnGL::DrawTest2D(int nWidth, int hHeight){
    if(glProgram[SHADER_BASE] <= GL_FALSE) {
        glProgram[SHADER_BASE] = GLShaders::CreateProgram_Source(
                GLShaders::LoadShaderPath(VertexShaderPath[SHADER_BASE]),
                GLShaders::LoadShaderPath(FragmentShaderPath[SHADER_BASE]), "");
    }
    auto &nowRender = _render[SHADER_BASE];
    nowRender.setProgramHandle(glProgram[SHADER_BASE]);
    nowRender.Use();

    //fbo
//    unsigned int framebuffer;
//    glGenFramebuffers(1, &framebuffer);
//    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
//    // 生成纹理
//    unsigned int texColorBuffer;
//    glGenTextures(1, &texColorBuffer);
//    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glBindTexture(GL_TEXTURE_2D, 0);
//
//// 将它附加到当前绑定的帧缓冲对象
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    //BUFFER

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_matrix), vertices_matrix, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    if (firstDraw == true) {
        // λ������
        nowRender.setVectexAttribute("aPos", 3, 5 * sizeof(float), (const float*)(0 * sizeof(float)));
    }
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

//    byte* pTmp = new byte[800 * 600 * 4];
//    glReadPixels(0, 0, 800, 600, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pTmp);
//
//    cv::Mat outputImage(hHeight, nWidth, CV_8UC4, pTmp);
//    cv::imshow("test", outputImage);
//    delete[] pTmp;

}

void LearnGL::DrawCute(int nWidth, int nHeight, Camera& camera, float fov) {
	glProgram[SHADER_CUTE] = GLShaders::CreateProgram_Source(
		GLShaders::LoadShaderPath(VertexShaderPath[SHADER_CUTE]), GLShaders::LoadShaderPath(FragmentShaderPath[SHADER_CUTE]));
	auto &nowRender = _render[SHADER_CUTE];
	nowRender.setProgramHandle(glProgram[SHADER_CUTE]);
	nowRender.Use();

	//model view projection
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
	//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	//view = (camera.GetViewMatrix());

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(fov), (float)nWidth / nHeight, 0.1f, 100.0f);
	nowRender.setMat4Uniform("view", view);
	nowRender.setMat4Uniform("projection", projection);
	nowRender.setMat4Uniform("model", model);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//if (firstDraw == true) {
		// λ������
		nowRender.setVectexAttribute("aPos", 3, 3 * sizeof(float), (const float*)(0 * sizeof(float)));
		/*nowRender.setVectexAttribute("aNormal", 3, 8 * sizeof(float), (const float*)(3 * sizeof(float)));
		nowRender.setVectexAttribute("aTexCoords", 2, 8 * sizeof(float), (const float*)(6 * sizeof(float)));*/
	//	firstDraw = false;
	//}

	nowRender.setTextureID("u_skybox", GL_TEXTURE0, 0, m_TexImages[TEXTURE3].glTexture, false);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glDrawArrays(GL_TRIANGLES, 0, 36);


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
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_vMesh.push_back(ProcessMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
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
		// ������λ�á����ߺ���������
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		vertices.push_back(vertex);

		//LOG("Position[%d].x : %.3f", i, vertex.Position.x);
		//LOG("Position[%d].y : %.3f", i, vertex.Position.y);
		//LOG("Position[%d].z : %.3f", i, vertex.Position.z);

	}
	// ��������
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
		// �������
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

float* LearnGL::GetVerties(int &nSize){
    glm::vec3 pos1(-1.0f,  1.0f, 0.0f);
    glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
    glm::vec3 pos3( 1.0f, -1.0f, 0.0f);
    glm::vec3 pos4( 1.0f,  1.0f, 0.0f);
    // texture coordinates
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


    float quadVertices[] = {
            // positions            // normal         // texcoords  // tangent                          // bitangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };
    nSize = sizeof(quadVertices);
    return quadVertices;

}