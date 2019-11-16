#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <GLRenders.h>
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh {
public:
	/*  网格数据  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TexImage> textures;
	/*  函数  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TexImage> textures);
	//void Draw(Shader shader);
private:
	/*  渲染数据  */
	//unsigned int VAO, VBO, EBO;
	/*  函数  */
	//void setupMesh();
};