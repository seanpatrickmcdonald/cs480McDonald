#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "shader.h"

class Object
{
  public:
    Object();

    //Standard Render-Only Constructor
    Object(std::string objFilename, std::string texFilename);

    //Standard Render-Only Constructor non origin origin
    Object(std::string objFilename, std::string texFilename, btVector3 origin);

    //Physics Constructor
    Object(std::string texFilename);

    ~Object();

    void Update(unsigned int dt);
    void Render(Shader*);
    void ShadowRender(GLuint);

    glm::mat4 GetModel();

    GLuint loadTexture(std::string textureName);
    const aiScene* LoadAssimp(std::string objFilename);
    glm::mat4 model;

  protected:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint texture_int;

    void InitializeVertices();

    float radius;
};

#endif /* OBJECT_H */
