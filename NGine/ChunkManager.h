#pragma once

/*template<typename T> struct matrix
{
	matrix(unsigned m, unsigned n) : m(m), n(n), vs(m* n) {}

T& operator ()(unsigned i, unsigned j) { 
return vs[i + m * j]; 
} 
private: 
	unsigned m; 
	unsigned n; 
	std::vector<T> vs;
};
//column-major/opengl: vs[i + m * j], row-major/c++: vs[n * i + j] 
*/

#include <memory>
#include <unordered_map> 
#include <map>

#include "Chunk.h"
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>


class Shader;
class Camera;

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	void CreateFixedWorld(int width, int height, int depth, std::shared_ptr<Shader> shader);
	void Update(Camera* camera, float dt);
	void Render(Camera* camera);

	static int ChunkGenRadius;

private:

	
	int Width = 0, Height = 0, Depth = 0;
	Chunk* Chunks;
	std::shared_ptr<Shader> ChunkShader;

	std::unordered_map<glm::vec3, std::unique_ptr<Chunk>> ChunkMap;

};