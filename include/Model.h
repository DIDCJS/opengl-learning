//#pragma once 
//
//class Model
//{
//public:
//	/*  ����   */
//	Model(char *path)
//	{
//		loadModel(path);
//	}
//	void Draw(Shader shader);
//private:
//	/*  ģ������  */
//	vector<Mesh> meshes;
//	string directory;
//	/*  ����   */
//	void loadModel(string path);
//	void processNode(aiNode *node, const aiScene *scene);
//	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
//	vector<TexImage> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
//		string typeName);
//};