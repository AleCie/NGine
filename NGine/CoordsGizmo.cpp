#include "CoordsGizmo.h"

CoordsGizmo::CoordsGizmo()
{
	

}

void CoordsGizmo::Create(Shader* shader)
{
	CGMesh.SetMeshDrawMode(EMeshDrawMode::Lines);
	CGMesh.SetMeshLayout(EMeshLayout::VertexOnly);
	CGMesh.ColorsEnabled = true;

	CGMesh.Vertices.push_back(4); CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(0);
	CGMesh.Vertices.push_back(-4); CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(0);

	CGMesh.Vertices.push_back(4); CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(0);
	CGMesh.Vertices.push_back(-3); CGMesh.Vertices.push_back(1); CGMesh.Vertices.push_back(0);

	CGMesh.Vertices.push_back(4); CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(0);
	CGMesh.Vertices.push_back(-3); CGMesh.Vertices.push_back(-1); CGMesh.Vertices.push_back(0);



	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(4); CGMesh.Vertices.push_back(0);
	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(-4); CGMesh.Vertices.push_back(0);

	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(4); CGMesh.Vertices.push_back(0);
	CGMesh.Vertices.push_back(1); CGMesh.Vertices.push_back(-3); CGMesh.Vertices.push_back(0);

	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(4); CGMesh.Vertices.push_back(0);
	CGMesh.Vertices.push_back(-1); CGMesh.Vertices.push_back(-3); CGMesh.Vertices.push_back(0);



	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(4);
	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(-4);

	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(4);
	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(1); CGMesh.Vertices.push_back(-3);

	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(4);
	CGMesh.Vertices.push_back(0); CGMesh.Vertices.push_back(-1); CGMesh.Vertices.push_back(-3);

	// colors
	CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0);

	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1); CGMesh.Colors.push_back(0);

	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1);
	CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(0); CGMesh.Colors.push_back(1);

	CGMesh.Create(shader);
}

void CoordsGizmo::Render(Shader* shader, Camera* camera)
{
	CGMesh.Render(shader, camera);
}
