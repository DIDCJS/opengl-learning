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
	/*  ��������  */
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TexImage> textures;
	/*  ����  */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TexImage> textures);
	//void Draw(Shader shader);
private:
	/*  ��Ⱦ����  */
	//unsigned int VAO, VBO, EBO;
	/*  ����  */
	//void setupMesh();
};