#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

class Camera;

class Chunk
{
public:
	Chunk();
	~Chunk();

	void Create(glm::vec3 position, std::shared_ptr<Shader> shader);
	void Render(Camera *cam);

	static const int ChunkSize = 16;

private:

	void CreateVoxelData();
	void CreateMesh();

	bool ShouldAddTop(int x, int y, int z);
	bool ShouldAddBottom(int x, int y, int z);
	bool ShouldAddLeft(int x, int y, int z);
	bool ShouldAddRight(int x, int y, int z);
	bool ShouldAddFront(int x, int y, int z);
	bool ShouldAddBack(int x, int y, int z);

	void AddTopFace(int x, int y, int z, int& idx);
	void AddBottomFace(int x, int y, int z, int& idx);
	void AddFrontFace(int x, int y, int z, int& idx);
	void AddBackFace(int x, int y, int z, int& idx);
	void AddLeftFace(int x, int y, int z, int& idx);
	void AddRightFace(int x, int y, int z, int& idx);

	int Data[ChunkSize][ChunkSize][ChunkSize];

	std::unique_ptr<Mesh> ChunkMesh;
	std::shared_ptr<Shader> ChunkShader;

	glm::vec3 Position = glm::vec3(0);
};