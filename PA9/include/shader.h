#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

#include <fstream>
#include <sstream>


class Shader
{

  public:
    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType, int shaderIndex, char** argv);
    bool AddShader(GLenum ShaderType, std::string shaderFile);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;
    GLint m_ambient;
    GLint m_diffuse_albedo;
    GLint m_specular_albedo;
    GLint m_specular_power;
    GLint m_lightPosition;
    

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;

};

#endif  /* SHADER_H */
