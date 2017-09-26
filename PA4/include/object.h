#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"
#include "obj_loader.h"

class Object
{
  public:
    Object();
    Object(std::string objFileName);
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

    float rotationAngle;
    float orbitAngle;
    float orbitRadius;
};

#endif /* OBJECT_H */