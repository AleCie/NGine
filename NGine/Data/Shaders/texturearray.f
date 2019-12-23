#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2DArray myTextureSampler;

void main(){

color = texture(myTextureSampler, vec3(UV.xy, 0));

//	vec2 coord;
 //   coord.x = UV.x;
  //  coord.y = UV.y;


	//vec4 c = texture(myTextureSampler, vec3(UV, 1));

  //  color = c;

    // Output color = color of the texture at the specified UV
    //color = texture( myTextureSampler, UV ).rgb;
}