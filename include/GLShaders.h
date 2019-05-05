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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	
	gl_Position = projection * view * model * vec4(aPos, 1.0); 
}
)";


static const char* pFragmentShaderLearn = R"(
#version 330 core  

uniform vec4 objectColor;
uniform vec4 lightColor;

void main(){
	gl_FragColor = vec4(lightColor * objectColor);

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
 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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