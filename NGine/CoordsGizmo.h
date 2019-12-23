#pragma once

#include "Mesh.h"

class Shader;
class Camera;

class CoordsGizmo
{
public:
	CoordsGizmo();

	void Create(Shader* shader);
	void Render(Shader* shader, Camera* camera);
private:
	Mesh CGMesh;
};