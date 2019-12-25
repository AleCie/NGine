#include "Mesh.h"

#include <GL/glew.h>

#include "Shader.h"
#include "Camera.h"

Mesh::Mesh()
{
}

Mesh::Mesh(EMeshLayout meshLayout)
{
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Mesh::Create(Shader* shader)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	if (IndicesEnabled)
	{
		glGenBuffers(1, &EBO);
	}
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

	if (IndicesEnabled)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), &Indices[0], GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	if (ColorsEnabled)
	{
		GLuint colorbuffer;
		glGenBuffers(1, &colorbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(float), &Colors[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(ColorsAttribute);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			ColorsAttribute,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
	}

	if (UVsEnabled)
	{
		GLuint uvbuffer;
		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(float), &UVs[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(UVsAttribute);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			UVsAttribute,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
	}

	if (NormalsEnabled)
	{
		GLuint normalsbuffer;
		glGenBuffers(1, &normalsbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
		glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(float), &Normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(NormalsAttribute);
		glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
		glVertexAttribPointer(
			NormalsAttribute,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
	}

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	MatrixID = glGetUniformLocation(shader->id(), "MVP");
}

void Mesh::Render(Shader* shader, Camera* camera)
{
	shader->bind();

	MVP = camera->GetProjectionMatrix() * camera->GetViewMatrix() * WorldMatrix;
	//MVP = WorldMatrix * camera->GetViewMatrix() * camera->GetProjectionMatrix();

	// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		//glDrawArrays(GL_TRIANGLES, 0, 6);

	if (MeshDrawMode == EMeshDrawMode::Triangles && IndicesEnabled)
	{
		glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
	}
	if (MeshDrawMode == EMeshDrawMode::Lines && IndicesEnabled == false)
	{
		//glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_LINES, 0, Vertices.size());
	}
}

void Mesh::SetMeshLayout(EMeshLayout layout)
{
	MeshLayout = layout;
}

void Mesh::SetMeshDrawMode(EMeshDrawMode mode)
{
	MeshDrawMode = mode;
}
