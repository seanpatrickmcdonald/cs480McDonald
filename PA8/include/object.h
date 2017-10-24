#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "bmploader.h"
#include "json.hpp"

class Object
{
  public:
    Object();
    Object(std::string objFilename);
    ~Object();
    void Update(unsigned int dt);
    void Render(GLint);

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;
    GLuint texture_int;

   // float rotationAngle;
    //float orbitAngle;
    //float orbitRadius;
};

#endif /* OBJECT_H */
