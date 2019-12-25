#include "ChunkManager.h"

#include "Chunk.h"


#include <glm/glm.hpp>
#include "Shader.h"
#include "Camera.h"


#include <chrono>
#include <iostream>

ChunkManager::ChunkManager()
{
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::CreateFixedWorld(int width, int height, int depth, std::shared_ptr<Shader> shader)
{
	Width = width;
	Height = height;
	Depth = depth;

	ChunkShader = shader;

	// Record start time
	auto start = std::chrono::high_resolution_clock::now();
	Chunks = new Chunk[width * height * depth];

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int z = 0; z < depth; z++)
			{
				Chunks[x + width * (y + depth * z)].Create(glm::vec3(x * Chunk::ChunkSize, y * Chunk::ChunkSize, z * Chunk::ChunkSize), shader);
			}
		}
	}

	auto finish = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double> elapsed = finish - start;
	std::cout << "Elapsed time: " << elapsed.count() << " s\n";

	std::cout << "Vertex count: " << Chunk::GlobalChunkVertexCount << std::endl;
}

void ChunkManager::Update(float dt)
{
	for (int i = 0; i < Width * Height * Depth; i++)
	{
		Chunks[i].Update(dt);
	}
}

void ChunkManager::Render(Camera* camera)
{
	for (int i = 0; i < Width * Height * Depth; i++)
	{
		Chunks[i].Render(camera);
	}
}
