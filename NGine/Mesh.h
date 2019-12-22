#pragma once

#include <vector>
#include <glm/glm.hpp>

enum class EMeshLayout
{
	Vertex,
	VertexColor,
	VertexColorNormal,
	VertexTexture,
	VertexTextureNormal,
	VertexTextureColor,
	VertexTextureColorNormal
};

class Camera;
class Shader;

class Mesh
{
public:
	Mesh();
	Mesh(EMeshLayout meshLayout);
	~Mesh();

	std::vector<float> Vertices;
	std::vector<int> Indices;
	std::vector<float> UVs;
	std::vector<float> Colors;

	glm::mat4 WorldMatrix = glm::mat4(1);
	glm::mat4 MVP = glm::mat4(1);

	void Create(Shader* shader);
	void Render(Shader *shader, Camera *camera);

private:

	unsigned int VAO, VBO, EBO;
	unsigned int MatrixID;
	
};