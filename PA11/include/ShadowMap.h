#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#include <vector>

#include "graphics_headers.h"

#include <fstream>
#include <sstream>
#include <vector>

class ShadowMap
{
#define SHADOWMAP_SIZE 1024

public:
    ShadowMap();
    ~ShadowMap();

    void InitSpotProg();
    void InitCubeProg();

    void MakeSpot();
    void MakeCube();

    void Enable(GLenum CubeFace);

    enum ShadowType
    {
    	DIRECTIONAL = 0,
    	SPOT = 1,
    	POINT = 2
    };

    ShadowType m_shadow_type;

    GLuint depth_buffer;
	  GLuint shadow_buffer;
	  GLuint shadow_tex;	
    static GLuint m_shaderProg;  
    static GLuint m_cubeProg;  

    GLuint depthMVP;
    GLuint model;
    GLuint gWorldPos;

    struct CameraDirection
	{
      GLenum CubemapFace;
      glm::vec3 Target;
      glm::vec3 Up;
	};

	CameraDirection gCameraDirections[6] =
	{
      { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
      { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
      { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
      { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
      { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
      { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
	}; 
};





#endif