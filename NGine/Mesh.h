#pragma once

#include <vector>
#include <glm/glm.hpp>

enum class EMeshLayout
{
	VertexElement,
	VertexOnly
};

enum class EMeshDrawMode
{
	Triangles,
	Lines
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
	std::vector<unsigned int> Indices;
	std::vector<float> UVs;
	std::vector<float> Colors;

	glm::mat4 WorldMatrix = glm::mat4(1);
	glm::mat4 MVP = glm::mat4(1);

	void Create(Shader* shader);
	void Render(Shader *shader, Camera *camera);
	void SetMeshLayout(EMeshLayout layout);
	void SetMeshDrawMode(EMeshDrawMode mode);

	bool ColorsEnabled = false;

private:

	unsigned int VAO, VBO, EBO;
	unsigned int MatrixID;

	EMeshLayout MeshLayout = EMeshLayout::VertexElement;
	EMeshDrawMode MeshDrawMode = EMeshDrawMode::Triangles;
	
};