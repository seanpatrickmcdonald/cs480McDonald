#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"

class Object
{
  public:
    Object();
    Object(std::string objFilename);
    ~Object();
    void Update(unsigned int dt);
    void Render();

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    Assimp::Importer *modelImporter;

    float rotationAngle;
    float orbitAngle;
    float orbitRadius;
};

#endif /* OBJECT_H */
