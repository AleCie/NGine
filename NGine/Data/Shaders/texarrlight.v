#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 vertexUV;
layout (location = 2) in vec3 aNormal;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec3 Normal;

out vec3 FragPos;  

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 M;

void main()
{
	// Output position of the vertex, in clip space : MVP * position
    gl_Position =  MVP * vec4(aPos,1);
	
FragPos = vec3(M * vec4(aPos, 1.0));

    // UV of the vertex. No special space for this one.
    UV = vertexUV;

Normal = mat3(transpose(inverse(M))) * aNormal;  
	//Normal = aNormal;
}