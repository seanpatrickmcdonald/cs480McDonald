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
    static void SetRotationSpeed(Object* objectToChange);
    static void ToggleRotationPause(Object* objectToChange);
    static void SetOrbitRadius(Object* objectToChange, float radius);
    static void SetOrbitSpeed(Object* objectToChange, float orbitSpeed );
    static void ToggleOrbitPause(Object* objectToChange);


    glm::mat4 GetModel();

  private:
    glm::mat4 model;
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    GLuint VB;
    GLuint IB;

    float rotationAngle;
    float rotationSpeed;
    float rotDirection;

    bool rotationPaused;

    float orbitRadius;
    float orbitAngle;
    float orbitSpeed;
    float orbitDirection;

    bool orbitPaused;
};

#endif /* OBJECT_H */
