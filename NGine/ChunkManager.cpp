#include "ChunkManager.h"


#include "Shader.h"
#include "Camera.h"


#include <chrono>
#include <iostream>

int ChunkManager::ChunkGenRadius = 3;

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
	/*for (int i = 0; i < Width * Height * Depth; i++)
	{
		Chunks[i].Update(dt);
	}*/

	//c++11
	/*for (auto const& p : ChunkMap)
	{
		auto pair = p.second;
	}*/
	//c++17

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
				
				/*if (ChunkMap.count(posToCheck))
				{

				}*/
				
				auto result = ChunkMap.find(posToCheck);
			
				if (result == ChunkMap.end()) // if doesn't exist (iterator looped through everything)
				{
					//create chunk
					std::unique_ptr<Chunk> newChunk = std::unique_ptr<Chunk>(new Chunk());
					newChunk->Create(posToCheck, ChunkShader);
					//ChunkMap.insert(std::make_pair<glm::vec3, Chunk>(posToCheck, newChunk));
					ChunkMap[posToCheck] = std::move(newChunk);
				}
			}
		}
	}

	for (auto it = ChunkMap.begin(); it != ChunkMap.end(); )
	{
		/*if (it->second != nullptr)
		{
			it->second->Update(camera, dt);// ->Update(dt);
		}*/

		if(it->second != nullptr)
		if (it->second->ShouldBeDeleted)
		{
			//ChunkMap.erase(it->second->GetPosition());

			it = ChunkMap.erase(it);
		}
		else
		{
			it->second->Update(camera, dt);
			++it;
		}
	}
	/*for (auto &[key, val] : ChunkMap)
	{
		
		val->Update(camera, dt);// ->Update(dt);
		if (val->ShouldBeDeleted)
		{
			ChunkMap.erase(val->GetPosition());
			break;
		}
	}*/
}

void ChunkManager::Render(Camera* camera)
{
	for (auto& [key, val] : ChunkMap)
	{
		val->Render(camera);
	}
	/*for (int i = 0; i < Width * Height * Depth; i++)
	{
		Chunks[i].Render(camera);
	}*/
}
