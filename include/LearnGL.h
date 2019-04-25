#ifndef __LEARNGL_H__
#define  __LEARNGL_H__

#include "glad/glad.h"
#include "GLRenders.h"

#define IMG_PATH "img//"

enum {
	TEXTURE0 = 0,
	TEXTURE1,
	TEXTURE2,
	NUM_TEXTURES
};

class LearnGL {
public:
	GLuint VAO;
	GLuint VBO, EBO, FBO;
	GLuint* glProgram;
	GLRenders* _render;
	TexImage m_TexImages[NUM_TEXTURES];
public:
	void Init(unsigned char* pT0, int nT0W, int nT0H, int nT0C,
		unsigned char* pT1, int nT1W, int nT1H, int nT1C/*,
		/*unsigned char* pT2, int nT2W, int nt2H, int nT2C*/) ;
	void LearnGL_Main();
	void Release();

};
#endif