#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;
in vec3 Normal;  

in vec3 FragPos;  

in float TexID;

// Ouput data
out vec4 color;

// Values that stay constant for the whole mesh.
uniform sampler2DArray myTextureSampler;


uniform vec3 lightPos; 

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform float ambientStrenght;

uniform vec3 camPos;

//for directional light only
uniform vec3 cusDirLight;

void main(){

// texture pixel
color = texture(myTextureSampler, vec3(UV.xy, TexID));

// light
vec3 norm = normalize(Normal);

//vec3 lightDir = normalize(lightPos - FragPos);  
vec3 lightDir = normalize(-cusDirLight);

// diffuse
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diff * lightColor;

//ambient
vec3 ambient = lightColor * ambientStrenght;

//specular
float specularStrength = 0.5;

vec3 viewDir = normalize(camPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm);  

float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
vec3 specular = specularStrength * spec * lightColor; 



vec3 result = (ambient + diffuse + specular) * vec3(color.xyz);

// setting
color  = vec4(result, 1.0);


//FragColor = vec4(result, 1.0);






//color *= vec4(ambient, 1);

}