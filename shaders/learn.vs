#version 330 core
#define NEED_POINT_LIGHT;
layout(location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	// gl_Position = vec4(aPos, 1.0);
	FragPos =(view * model * vec4(aPos, 1.0)).rgb;
	Normal = mat3(transpose(inverse(view*model))) * aNormal;
	// Normal = vec3(1.0);
    TexCoords = aTexCoords;
}
