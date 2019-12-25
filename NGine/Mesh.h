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
	std::vector<float> Normals;

	std::vector<float> TexIDs;

	glm::mat4 WorldMatrix = glm::mat4(1);
	glm::mat4 MVP = glm::mat4(1);

	void Create(Shader* shader);
	void Render(Shader *shader, Camera *camera);
	void SetMeshLayout(EMeshLayout layout);
	void SetMeshDrawMode(EMeshDrawMode mode);

	bool ColorsEnabled = false;
	bool UVsEnabled = false;
	bool IndicesEnabled = true;
	bool NormalsEnabled = false;

	bool TexIDEnabled = false;

	int ColorsAttribute = 1;
	int UVsAttribute = 1;
	int NormalsAttribute = 1;

	int TexIDAttribute = 1;

private:

	unsigned int VAO, VBO, EBO;
	unsigned int MatrixID;

	EMeshLayout MeshLayout = EMeshLayout::VertexElement;
	EMeshDrawMode MeshDrawMode = EMeshDrawMode::Triangles;
	
};