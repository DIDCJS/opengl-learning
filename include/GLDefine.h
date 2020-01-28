#pragma once
#include "glad/glad.h"
#include "cstdio"
#define NEED_CHECK_GL 
#define CHECK_GL {GLenum glError = glGetError(); if(glError != 0){printf("###glError : %d [%d] [%s] \n",glError,__LINE__,__FILE__);}}

#define IMG_PATH R"(img/)"
#define MAX_LEN 2000
#define byte unsigned char

static char s_RunPath[MAX_LEN];
static char logInfo[MAX_LEN];


#define LOG(...) \
sprintf(logInfo, __VA_ARGS__); \
printf("[DID] %s \n",logInfo);

#define LOGE(...) \
sprintf(logInfo, __VA_ARGS__); \
printf("\033[1;31;40m[DID] %s \n \033[0m",logInfo);




//#define LOG(...) printf(__VA_ARGS__);