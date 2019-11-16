//#pragma once 
//
//class Model
//{
//public:
//	/*  函数   */
//	Model(char *path)
//	{
//		loadModel(path);
//	}
//	void Draw(Shader shader);
//private:
//	/*  模型数据  */
//	vector<Mesh> meshes;
//	string directory;
//	/*  函数   */
//	void loadModel(string path);
//	void processNode(aiNode *node, const aiScene *scene);
//	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
//	vector<TexImage> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
//		string typeName);
//};