#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include "graphics_headers.h"

#include <fstream>
#include <sstream>
#include <vector>


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
    glm::vec3 GetUniform3f(GLint);
    float GetUniformf(GLint);
    std::vector<GLint> uniforms;

    GLint m_projectionMatrix;
    GLint m_viewMatrix;
    GLint m_modelMatrix;

    std::string UniformName[8] =
    {
      "model_matrix",
      "view_matrix",
      "projection_matrix",
      "ambient",
      "diffuse_albedo",
      "specular_albedo",
      "specular_power",
      "light_position"
    };

  private:
    GLuint m_shaderProg;    
    std::vector<GLuint> m_shaderObjList;
};


enum Uniform
{
    PROJMATRIX = 0,
    VIEWMATRIX = 1,
    MODELMATRIX = 2,
    AMBIENT = 3,
    DIFFUSE = 4,
    SPECULARALB = 5,
    SPECULARPOW = 6,
    LIGHTPOS = 7
};



#endif  /* SHADER_H */
