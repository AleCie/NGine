#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 vertexColor;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentColor;

uniform mat4 MVP;

void main()
{
	//gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position =  MVP * vec4(aPos, 1);

	fragmentColor = vertexColor;
}