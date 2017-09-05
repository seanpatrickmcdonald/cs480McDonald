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

    static void FlipRotation(Object* objectToFlip);
    static void FlipOrbit(Object* objectToFlip);
    static void SetRotationSpeed(Object* objectToChange, float speedIn);
    static void SetOrbitRadius(Object* objectToChange, float radius);
    static void SetOrbitSpeed(Object* objectToChange, float orbitSpeed );
    static void SetScale(Object* objectToChange, float scaleIn);
    static void SetParent(Object* objectToChange, Object* parentIn);
 
    float orbitRadius;
 
    glm::mat4 GetModel();

    glm::vec3 center;

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float rotationAngle;
    float rotationSpeed;
    float orbitAngle;
    float orbitSpeed;
    glm::vec3 scale;

    Object* parent;
    Object* child;

    signed char orbitDirection;
    signed char rotDirection;
};

#endif /* OBJECT_H */
