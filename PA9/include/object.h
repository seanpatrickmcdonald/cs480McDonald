#ifndef OBJECT_H
#define OBJECT_H

#include "graphics_headers.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"


class Object
{
  public:
    Object();
    Object(std::string objFilename, std::string texFilename);
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();

    GLuint loadBMP(std::string textureName);
    const aiMesh* LoadAssimp(std::string objFilename);

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint texture_int;
};

#endif /* OBJECT_H */
