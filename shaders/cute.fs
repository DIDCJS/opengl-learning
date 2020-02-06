#version 330 core  
in vec3 cuteTexCoords;

uniform samplerCube u_skybox;
layout(location = 0) out vec4 outColor;
void main(){
	    outColor = texture(u_skybox, cuteTexCoords);
}