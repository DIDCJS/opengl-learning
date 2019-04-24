#pragma once
#include "glad/glad.h"
#include "cstdio"
#define NEED_CHECK_GL 1
#define CHECK_GL {GLenum glError = glGetError(); if(glError != 0){printf("###glError : %d [%d] [%s] \n",glError,__LINE__,__FILE__);}}