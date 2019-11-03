#ifndef __GLSHADER_H__
#define __GLSHADER_H__

#include "glad/glad.h"

enum {
	SHADER_CRAYON,
	SHADER_MASK_TEXT,
	SHADER_LEARN,
	SHADER_LIGHT,
	NUM_SHADERS,
};
//Crayon

const static char *pVertexShaderCrayon = R"(
attribute vec4 Position;
attribute vec4 Normal;
attribute vec2 TexCoord;
attribute lowp vec4 a_color;
varying vec2 DestinationTexCoord;
varying lowp vec4  v_color;
uniform float width;
uniform float height;
void main(void)
{
    
    v_color = a_color;
    v_color = clamp(v_color, 0.0, 1.0);
//    v_texCoord = a_texCoord0;
    DestinationTexCoord = TexCoord;
  gl_Position = vec4(Position.x/width*2.0-1.0,Position.y/height*2.0-1.0,0.0,1.0 ) ;
}

)";



const static char *pFragmentShaderCrayon = R"(
precision highp float;
uniform sampler2D Texture;
uniform float magnitude;
varying highp vec2 DestinationTexCoord;
varying lowp    vec4 v_color;//原图颜色
varying highp   vec3 v_texCoord;

void main()
{

        lowp vec4 color;
        color = v_color;
        color = clamp(color, 0.0, 1.0);
    //    lowp int      currTex;
        lowp vec4     texColor;
        highp vec3    texCoords;
    //    highp float   s, t;
        texCoords.x = DestinationTexCoord.x;
        texCoords.y = DestinationTexCoord.y;
        texColor = texture2D(Texture, texCoords.xy);
        color *= texColor;
        gl_FragColor = color;
    
}
)";


//MaskTest

const static char *pVertexShaderMaskText = R"(
attribute vec3 aPos;
attribute vec2 aTexCoord; 
varying vec2 TexCoord;  
void main(){
	
	gl_Position = vec4(aPos, 1.0); 
	TexCoord = aTexCoord;	
}
)";

static const char* pFragmentShaderMaskText = R"(
precision highp float;
varying vec2 TexCoord;

uniform vec2 Half;
uniform vec2 Translation;
uniform float scale;
uniform sampler2D src;
uniform sampler2D bg;
uniform sampler2D mask;
uniform sampler2D text;

void main() {
 vec2 newCoord =  (TexCoord - Translation - Half) * scale + Half ;
 vec3  new_src = texture2D(src, newCoord).xyz;
 vec3  new_bg = texture2D(bg, TexCoord).xyz;
 float new_mask = texture2D(mask, newCoord).x;
 vec3 new_Image = new_src * new_mask + new_bg * (1.0f - new_mask);
 vec4 new_text = texture2D(text, TexCoord); 
 gl_FragColor = vec4(new_Image * (1.0f - new_text.w) + (new_text * (new_text.w)).xyz, 1.0f);
 //gl_FragColor =  texture2D(mask, TexCoord);
}
)";


//Learn

const static char *pVertexShaderLearn = R"(
#version 330 core
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
	FragPos =(view * model * vec4(aPos, 1.0)).rgb;
	Normal = mat3(transpose(inverse(view*model))) * aNormal;
    TexCoords = aTexCoords;
}
)";


static const char* pFragmentShaderLearn = R"(
#version 330 core  
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 viewPos;

struct Light {
    vec3 position; // 使用定向光就不再需要了
    vec3 direction;
	float cutOff;
	float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Light u_light;
uniform Material u_material;

uniform sampler2D u_diffuseMap;
uniform sampler2D u_specularMap;
void main(){
	
	vec3 diffuseMap = texture2D(u_diffuseMap, TexCoords).rgb;
	vec3 specularMap = texture2D(u_specularMap, TexCoords).rgb;

	float distance    = length(u_light.position - FragPos);
	float attenuation = 1.0 / (u_light.constant + u_light.linear * distance + 
                u_light.quadratic * (distance * distance));

	vec3 lightDir = normalize(u_light.position - FragPos);    //光方向归一化

	//环境光	
    vec3 ambient = u_light.ambient * diffuseMap;

	//漫反射
	vec3 norm = normalize(Normal);  //法线归一化
	float diff = max(dot(norm, lightDir), 0.0);     //点乘模拟角度值，两向量角度越小，越接近1，范围[0,1]
	vec3 diffuse = diff * u_light.diffuse * diffuseMap;  //最后漫反射值

	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);   //第一视角向量
	vec3 reflectDir = reflect(-lightDir, norm);     //关于法线对称的光向量
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_material.shininess);   //反光度
	vec3 specular = spec * u_light.specular *  specularMap;	

	//模拟灯源衰弱
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;

	//聚光灯强度
	float theta     = dot(lightDir, normalize(-u_light.direction));
	float epsilon   = u_light.cutOff - u_light.outerCutOff;	
	float intensity = clamp((theta - u_light.outerCutOff) / epsilon, 0.0, 1.0);    
	diffuse  *= intensity;
	specular *= intensity;

	gl_FragColor = vec4( (specular+ diffuse + ambient) , 1.0);
	//gl_FragColor = vec4(diffuseMap, 1.0);
}
)";

//Light Object
const static char *pVertexShaderLight = R"(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	
	gl_Position = projection * view * model * vec4(aPos, 1.0); 
}
)";


static const char* pFragmentShaderLight = R"(
#version 330 core  

void main(){
	gl_FragColor = vec4(1.0);

}
)";

const static char* VertexShaderSource[NUM_SHADERS] = {
	pVertexShaderCrayon,
	pVertexShaderMaskText,
	pVertexShaderLearn,
	pVertexShaderLight
};
const static char* FragmentShaderSource[NUM_SHADERS] = {
	pFragmentShaderCrayon,
	pFragmentShaderMaskText,
	pFragmentShaderLearn,
	pFragmentShaderLight
};

const static float vertices_matrix[] = {
	//     ---- 位置 ----      - 纹理坐标 -
		 0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // 右上
		 0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,    0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,     0.0f, 1.0f    // 左上
};
const static unsigned int indices_matrix[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};



const static float vertices[] = {
	// positions          // normals           // texture coords
	  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	   0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	  -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	  -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	   0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	   0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	   0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	   0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	  -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	  -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	   0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	   0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	  -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	  -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
const static unsigned int indices[] = { // 注意索引从0开始! 
	0, 1, 3, // 第一个三角形
	1, 2, 3  // 第二个三角形
};

const static float vertices_learn[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};

class GLShaders {
public:
	static GLuint CreateProgram_Source(const char* pVertexSource, const char* pFragmentSource);

	static GLuint LoadShader_Source(GLenum shaderType, const char* pSource);
};

#endif