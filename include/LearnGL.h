#ifndef __LEARNGL_H__
#define  __LEARNGL_H__

#include <vector>
#include "glad/glad.h"
#include "GLRenders.h"
#include "Mesh.h"
#include "Camera.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>
#define IMG_PATH "img//"

enum {
	TEXTURE0 = 0,
	TEXTURE1,
	TEXTURE2,
	NUM_TEXTURES
};
class LearnGL {
private:
	GLuint VAO, LightVAO;
	GLuint VBO, EBO, FBO;
	GLuint* glProgram;
	GLRenders* _render;
	TexImage m_TexImages[NUM_TEXTURES];
	bool firstDraw = true;
	//����ģ�ͳ���
	const aiScene *m_pScene = nullptr;
	//�洢ÿ���ڵ��Mesh(����Pos,Index,texUV)����
	std::vector<Mesh> m_vMesh;
	std::string m_sDirectory;
public:
	void Init(unsigned char* pT0, int nT0W, int nT0H, int nT0C,
		unsigned char* pT1, int nT1W, int nT1H, int nT1C/*,
		/*unsigned char* pT2, int nT2W, int nt2H, int nT2C*/) ;
	void LearnGL_Main(int nWidth, int nHeight, Camera& camera, float fov);
	void Release();
private:
	void Draw();
	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);
	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<TexImage> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};
#endif