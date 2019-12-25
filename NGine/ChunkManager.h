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

class Chunk;
class Shader;
class Camera;

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	void CreateFixedWorld(int width, int height, int depth, std::shared_ptr<Shader> shader);
	void Update(float dt);
	void Render(Camera* camera);

private:

	int Width = 0, Height = 0, Depth = 0;
	Chunk* Chunks;
	std::shared_ptr<Shader> ChunkShader;

};