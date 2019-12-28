#include "ChunkManager.h"


#include "Shader.h"
#include "Camera.h"


#include <chrono>
#include <iostream>

int ChunkManager::ChunkGenRadius = 5;

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

void ChunkManager::Update(Camera* camera, float dt)
{
	//figure out chunk where camera is
	glm::vec3 camChunkPos = camera->GetPosition() / glm::vec3(Chunk::ChunkSize, Chunk::ChunkSize, Chunk::ChunkSize);
	camChunkPos = glm::vec3(floorf(camChunkPos.x) * Chunk::ChunkSize, floorf(camChunkPos.y) * Chunk::ChunkSize, floorf(camChunkPos.z) * Chunk::ChunkSize);

	// loop for theoretical chunks around camera
	for (int x = -ChunkGenRadius; x < ChunkGenRadius; x++)
	{
		for (int y = -ChunkGenRadius; y < ChunkGenRadius; y++)
		{
			for (int z = -ChunkGenRadius; z < ChunkGenRadius; z++)
			{
				// check if chunk exists
				glm::vec3 posToCheck = camChunkPos + glm::vec3(x * Chunk::ChunkSize, y * Chunk::ChunkSize, z * Chunk::ChunkSize);
				
				
				auto result = ChunkMap.find(posToCheck);
			
				if (result == ChunkMap.end()) // if doesn't exist (iterator looped through everything)
				{
					//create chunk
					std::shared_ptr<Chunk> newChunk = std::shared_ptr<Chunk>(new Chunk());

					// check surrounding chunks;
					auto topResult = ChunkMap.find(posToCheck + glm::vec3(0, Chunk::ChunkSize, 0));
					if (topResult != ChunkMap.end())
					{
						newChunk->TopChunk = topResult->second;

						topResult->second->BottomChunk = newChunk;
						topResult->second->ShouldRebuild = true;
					}

					auto bottomResult = ChunkMap.find(posToCheck + glm::vec3(0, -Chunk::ChunkSize, 0));
					if (bottomResult != ChunkMap.end())
					{
						newChunk->BottomChunk = bottomResult->second;

						bottomResult->second->TopChunk = newChunk;
						bottomResult->second->ShouldRebuild = true;
					}

					auto leftResult = ChunkMap.find(posToCheck + glm::vec3(-Chunk::ChunkSize, 0, 0));
					if (leftResult != ChunkMap.end())
					{
						newChunk->LeftChunk = leftResult->second;

						leftResult->second->RightChunk = newChunk;
						leftResult->second->ShouldRebuild = true;
					}

					auto rightResult = ChunkMap.find(posToCheck + glm::vec3(Chunk::ChunkSize, 0, 0));
					if (rightResult != ChunkMap.end())
					{
						newChunk->RightChunk = rightResult->second;

						rightResult->second->LeftChunk = newChunk;
						rightResult->second->ShouldRebuild = true;
					}
					
					auto frontResult = ChunkMap.find(posToCheck + glm::vec3(0, 0, Chunk::ChunkSize));
					if (frontResult != ChunkMap.end())
					{
						newChunk->FrontChunk = frontResult->second;

						frontResult->second->BackChunk = newChunk;
						frontResult->second->ShouldRebuild = true;
					}

					auto backResult = ChunkMap.find(posToCheck + glm::vec3(0, 0, -Chunk::ChunkSize));
					if (backResult != ChunkMap.end())
					{
						newChunk->BackChunk = backResult->second;

						backResult->second->FrontChunk = newChunk;
						backResult->second->ShouldRebuild = true;
					}

					//create and assign
					newChunk->Create(posToCheck, ChunkShader);
					//ChunkMap.insert(std::make_pair<glm::vec3, Chunk>(posToCheck, newChunk));
					ChunkMap[posToCheck] = std::move(newChunk);
				}
			}
		}
	}

	for (auto it = ChunkMap.begin(); it != ChunkMap.end(); )
	{
		if(it->second != nullptr)
		if (it->second->ShouldBeDeleted)
		{
			it = ChunkMap.erase(it);
		}
		else
		{
			it->second->Update(camera, dt);
			++it;
		}
	}
}

void ChunkManager::Render(Camera* camera)
{
	for (auto& [key, val] : ChunkMap)
	{
		val->Render(camera);
	}
}
