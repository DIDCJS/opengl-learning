#version 330 core  

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main(){
	//∑¥…‰
	//gl_FragColor = vec4(1.0);
	//vec3 I = normalize(Position - cameraPos);
    //vec3 R = reflect(I, normalize(Normal));
    //gl_FragColor = vec4(texture(skybox, R).rgb, 1.0);
	//’€…‰
	 float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    gl_FragColor = vec4(texture(skybox, R).rgb, 1.0);
}