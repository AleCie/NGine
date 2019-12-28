#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Mesh.h"

#include <thread>
#include <future>

class Camera;

class Chunk
{
public:
	Chunk();
	~Chunk();

	void Create(glm::vec3 position, std::shared_ptr<Shader> shader);
	void Update(Camera* cam, float dt);
	void Render(Camera *cam);

	//void RebuildMesh();

	glm::vec3 GetPosition();
	glm::mat4 GetWorldMatrix();

	static const int ChunkSize = 16;

	float NoiseTreshold = 0.0f;
	float NoiseScale = 1.0f;

	static int GlobalChunkVertexCount;

	bool ShouldBeDeleted = false;

	int Data[ChunkSize][ChunkSize][ChunkSize];

	std::weak_ptr<Chunk> TopChunk;
	std::weak_ptr<Chunk> BottomChunk;
	std::weak_ptr<Chunk> LeftChunk;
	std::weak_ptr<Chunk> RightChunk;
	std::weak_ptr<Chunk> FrontChunk;
	std::weak_ptr<Chunk> BackChunk;

	bool ShouldRebuild = false;

private:

	void Create();
	void CleanMesh();

	void CreateVoxelData();
	void CreateMesh();
	void CreateOpenGLMesh();
	void CreateChunkThreadFunc(bool& result, std::atomic<bool>& shouldTerminate, std::atomic<bool>& wasTerminated);

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

	

	

	std::unique_ptr<Mesh> ChunkMesh;
	std::shared_ptr<Shader> ChunkShader;

	glm::vec3 Position = glm::vec3(0);

	bool IsChunkEmpty = true;
	bool DidThreadFinish = false;
	bool WasMeshCreated = false;

	std::thread ChunkThread;
	std::atomic<bool> ShouldTerminateThread = false;
	std::atomic<bool> WasThreadTerminated = false;
};