#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <vector>

#include "graphics_headers.h"

#include <fstream>
#include <sstream>
#include <vector>

class ShadowMap
{
public:
    ShadowMap();
    ~ShadowMap();

    void Enable();

	GLuint shadow_buffer;
	GLuint shadow_tex;	
    GLuint m_shaderProg;    
    GLuint depthMVP;
    //GLuint model_matrix;
};






#endif