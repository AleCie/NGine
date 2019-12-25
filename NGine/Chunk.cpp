#include "Chunk.h"

#include <string>

#include "Camera.h"

#include "Filepath.h"

#include <glm/ext/matrix_transform.hpp>
#include "FastNoiseSIMD/FastNoiseSIMD.h"

Chunk::Chunk()
{
}

Chunk::~Chunk()
{
}

void Chunk::Create(glm::vec3 position, std::shared_ptr<Shader> shader)
{
	Position = position;
	ChunkShader = shader;

	CreateVoxelData();
	CreateMesh();

	
	//ChunkShader = std::unique_ptr<Shader>(new Shader((fp::ShadersFolder + shaderName + fp::ExtVertex).c_str(), (fp::ShadersFolder + shaderName + fp::ExtFragment).c_str()));
}

void Chunk::Render(Camera *cam)
{
	if (IsChunkEmpty == false)
	{
		ChunkMesh->Render(ChunkShader.get(), cam);
	}
}

void Chunk::RebuildMesh()
{
	IsChunkEmpty = true;

	ChunkMesh->Vertices.clear();
	ChunkMesh->Indices.clear();
	ChunkMesh->Normals.clear();
	ChunkMesh->UVs.clear();

	ChunkMesh.reset();

	CreateVoxelData();
	CreateMesh();


}

glm::mat4 Chunk::GetWorldMatrix()
{
	return ChunkMesh->WorldMatrix;
}

void Chunk::CreateVoxelData()
{
	FastNoiseSIMD* myNoise = FastNoiseSIMD::NewFastNoiseSIMD();
	
	// Get a set of 16 x 16 x 16 Simplex Fractal noise
	float* noiseSet = myNoise->GetSimplexFractalSet(Position.x, Position.y, Position.z, ChunkSize, ChunkSize, ChunkSize, NoiseScale);
	int noiseIdx = 0;

	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{

				if (noiseSet[noiseIdx] > NoiseTreshold)
				{
					Data[x][y][z] = 0;
				}
				else
				{
					Data[x][y][z] = -1;
				}
				noiseIdx++;
				/*if (y < 8)
				{
					if (x < 6 && z < 6)
					{
						Data[x][y][z] = 0;
					}
					else
					{
						Data[x][y][z] = 1;
					}
					
				} 
				else if ( y > 8)
				{
					Data[x][y][z] = -1;
				}

				if (y == 9 || y == 10)
				{
					if (x > 4 && x < 12 && z > 4 && z < 12)
					{
						Data[x][y][z] = 1;
					}
				}*/
			}
		}
	}

	FastNoiseSIMD::FreeNoiseSet(noiseSet);
}

void Chunk::CreateMesh()
{
	ChunkMesh = std::unique_ptr<Mesh>(new Mesh());
	ChunkMesh->IndicesEnabled = true;
	ChunkMesh->UVsEnabled = true;
	ChunkMesh->UVsAttribute = 1;
	ChunkMesh->NormalsEnabled = true;
	ChunkMesh->NormalsAttribute = 2;
	ChunkMesh->TexIDEnabled = true;
	ChunkMesh->TexIDAttribute = 3;

	int idx = 0;
	for (int x = 0; x < ChunkSize; x++)
	{
		for (int y = 0; y < ChunkSize; y++)
		{
			for (int z = 0; z < ChunkSize; z++)
			{
				if (Data[x][y][z] >= 0) // voxel exists
				{
					if (ShouldAddTop(x, y, z)) AddTopFace(x, y, z, idx);
					if (ShouldAddBottom(x, y, z)) AddBottomFace(x, y, z, idx);
					if (ShouldAddFront(x, y, z)) AddFrontFace(x, y, z, idx);
					if (ShouldAddBack(x, y, z)) AddBackFace(x, y, z, idx);
					if (ShouldAddLeft(x, y, z)) AddLeftFace(x, y, z, idx);
					if (ShouldAddRight(x, y, z)) AddRightFace(x, y, z, idx);
				}
				//if (ShouldAddTop(x, y, z)) AddTopFace(x, y, z, idx);
				
			}
		}
	}

	if (ChunkMesh->Vertices.size() > 0)
	{
		ChunkMesh->Create(ChunkShader.get());

		IsChunkEmpty = false;
	}
	else
	{
		IsChunkEmpty = true;
	}

	ChunkMesh->WorldMatrix = glm::translate(ChunkMesh->WorldMatrix, Position);
}

bool Chunk::ShouldAddTop(int x, int y, int z)
{
	if (y + 1 >= ChunkSize)
	{
		// out of bounds, decide if should display quad, or fetch data from child chunk if exists

		return true; // for now, display quad if out of bounds
	}

	if (Data[x][y + 1][z] >= 0) // if Data is greater than zero, means there is a voxel there, so should not add that face
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Chunk::ShouldAddBottom(int x, int y, int z)
{
	if (y - 1 < 0)
	{
		return true;
	}

	if (Data[x][y - 1][z] >= 0) // if Data is greater than zero, means there is a voxel to display
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Chunk::ShouldAddLeft(int x, int y, int z)
{
	if (x - 1 < 0)
	{
		return true;
	}

	if (Data[x - 1][y][z] >= 0) // if Data is greater than zero, means there is a voxel to display
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Chunk::ShouldAddRight(int x, int y, int z)
{
	if (x + 1 >= ChunkSize)
	{
		return true;
	}

	if (Data[x + 1][y][z] >= 0) // if Data is greater than zero, means there is a voxel to display
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Chunk::ShouldAddFront(int x, int y, int z)
{
	if (z + 1 >= ChunkSize)
	{
		return true;
	}

	if (Data[x][y][z + 1] >= 0) // if Data is greater than zero, means there is a voxel to display
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool Chunk::ShouldAddBack(int x, int y, int z)
{
	if (z - 1 < 0)
	{
		return true;
	}

	if (Data[x][y][z - 1] >= 0) // if Data is greater than zero, means there is a voxel to display
	{
		return false;
	}
	else
	{
		return true;
	}
}

void Chunk::AddTopFace(int x, int y, int z, int& idx)
{
	// top face verts

	ChunkMesh->Vertices.push_back(1 + x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(1 + z);
	ChunkMesh->Vertices.push_back(1 + x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(0 + z);
	ChunkMesh->Vertices.push_back(0 + x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(0 + z);
	ChunkMesh->Vertices.push_back(0 + x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(1 + z);

	// indices

	ChunkMesh->Indices.push_back(idx + 0); ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 3);
	ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 2); ChunkMesh->Indices.push_back(idx + 3);
	idx += 4;

	// normals

	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(1); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(1); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(1); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(1); ChunkMesh->Normals.push_back(0);

	// uvs

	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(1);
	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(1);

	// texid

	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
}

void Chunk::AddBottomFace(int x, int y, int z, int& idx)
{
	// bottom face
	ChunkMesh->Vertices.push_back(1 + x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(1 + z);
	ChunkMesh->Vertices.push_back(1 + x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(0 + z);
	ChunkMesh->Vertices.push_back(0 + x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(0 + z);
	ChunkMesh->Vertices.push_back(0 + x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(1 + z);

	// bottom

	ChunkMesh->Indices.push_back(idx + 3); ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 0);
	ChunkMesh->Indices.push_back(idx + 3); ChunkMesh->Indices.push_back(idx + 2); ChunkMesh->Indices.push_back(idx + 1);
	idx += 4;

	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(-1); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(-1); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(-1); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(-1); ChunkMesh->Normals.push_back(0);

	// uvs

	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(1);
	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(1);

	// texid

	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
}

void Chunk::AddFrontFace(int x, int y, int z, int& idx)
{
	// front face verts
	ChunkMesh->Vertices.push_back(1 + x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(z + 1);
	ChunkMesh->Vertices.push_back(1 + x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(z + 1);
	ChunkMesh->Vertices.push_back(0 + x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(z + 1);
	ChunkMesh->Vertices.push_back(0 + x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(z + 1);

	// indices

	ChunkMesh->Indices.push_back(idx + 3); ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 0);
	ChunkMesh->Indices.push_back(idx + 3); ChunkMesh->Indices.push_back(idx + 2); ChunkMesh->Indices.push_back(idx + 1);
	idx += 4;

	// normals

	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(1);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(1);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(1);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(1);

	// uvs

	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(1);
	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(1);

	// texid

	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
}

void Chunk::AddBackFace(int x, int y, int z, int& idx)
{
	// back face
	ChunkMesh->Vertices.push_back(1 + x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(z);
	ChunkMesh->Vertices.push_back(1 + x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(z);
	ChunkMesh->Vertices.push_back(0 + x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(z);
	ChunkMesh->Vertices.push_back(0 + x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(z);

	// back

	ChunkMesh->Indices.push_back(idx + 0); ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 3);
	ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 2); ChunkMesh->Indices.push_back(idx + 3);
	idx += 4;

	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(-1);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(-1);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(-1);
	ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(-1);

	// uvs

	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(1);
	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(1);

	// texid

	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
}

void Chunk::AddLeftFace(int x, int y, int z, int& idx)
{
	// left face
	ChunkMesh->Vertices.push_back(x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(z + 1);
	ChunkMesh->Vertices.push_back(x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(z + 1);
	ChunkMesh->Vertices.push_back(x); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(z);
	ChunkMesh->Vertices.push_back(x); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(z);

	// left

	ChunkMesh->Indices.push_back(idx + 3); ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 0);
	ChunkMesh->Indices.push_back(idx + 3); ChunkMesh->Indices.push_back(idx + 2); ChunkMesh->Indices.push_back(idx + 1);
	idx += 4;

	ChunkMesh->Normals.push_back(-1); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(-1); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(-1); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(-1); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0);

	// uvs

	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(1);
	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(1);

	// texid

	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
}

void Chunk::AddRightFace(int x, int y, int z, int& idx)
{
	// right face
	ChunkMesh->Vertices.push_back(x + 1); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(z + 1);
	ChunkMesh->Vertices.push_back(x + 1); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(z + 1);
	ChunkMesh->Vertices.push_back(x + 1); ChunkMesh->Vertices.push_back(y); ChunkMesh->Vertices.push_back(z);
	ChunkMesh->Vertices.push_back(x + 1); ChunkMesh->Vertices.push_back(1 + y); ChunkMesh->Vertices.push_back(z);

	// right

	ChunkMesh->Indices.push_back(idx + 0); ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 3);
	ChunkMesh->Indices.push_back(idx + 1); ChunkMesh->Indices.push_back(idx + 2); ChunkMesh->Indices.push_back(idx + 3);
	idx += 4;

	ChunkMesh->Normals.push_back(1); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(1); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(1); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0);
	ChunkMesh->Normals.push_back(1); ChunkMesh->Normals.push_back(0); ChunkMesh->Normals.push_back(0);

	// uvs

	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(0);
	ChunkMesh->UVs.push_back(1); ChunkMesh->UVs.push_back(1);
	ChunkMesh->UVs.push_back(0); ChunkMesh->UVs.push_back(1);

	// texid

	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
	ChunkMesh->TexIDs.push_back((float)Data[x][y][z]);
}

