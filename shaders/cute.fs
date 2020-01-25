#version 330 core  
in vec3 cuteTexCoords;

uniform samplerCube u_skybox;
void main(){
	    gl_FragColor = texture(u_skybox, cuteTexCoords);
}