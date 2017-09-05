#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "graphics_headers.h"

class Object
{
  public:
    Object();
    ~Object();
    void Update(unsigned int dt);
    void Render();

    void FlipRotation();
    void FlipOrbit();
    void SetRotationSpeed();
    void SetOrbitRadius(float radius);
    void SetOrbitSpeed(float orbitSpeed );

    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float rotationAngle;
    float rotationSpeed;
    float orbitRadius;
    float orbitAngle;
    float orbitSpeed;

    signed char orbitDirection;
    signed char rotDirection;
};

#endif /* OBJECT_H */
