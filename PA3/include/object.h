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

    static void FlipOrbit(Object* objectToFlip);
    static void SetOrbitDirection(Object* objectToFlip, signed int orbitDirectionIn);
    static void SetOrbitSpeed(Object* objectToChange, float orbitSpeed);
    static void ToggleOrbitPause(Object* objectToChange);
    static void SetOrbitRadius(Object* objectToChange, float radius);

    static void FlipRotation(Object* objectToFlip);
    static void SetRotationDirection(Object* objectToFlip, signed int rotationDirectionIn);
    static void SetRotationSpeed(Object* objectToChange, float speedIn);
    static void ToggleRotationPause(Object* objectToChange);
    
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

    signed int orbitDirection;
    signed int rotDirection;

    bool orbitPaused;
    bool rotationPaused;
};

#endif /* OBJECT_H */
