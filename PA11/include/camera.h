#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void Translate(glm::vec3 translationVector);
    void LookAt(glm::vec3 lookAtVector);
    void Update(unsigned int dt);

    int movement[3][3];
  
  private:
    glm::mat4 projection;
    glm::mat4 view;

    float camera_speed = 0.01;

    glm::vec3 eye_position = glm::vec3(0.0, 8, 8);
    glm::vec3 eye_focus = glm::vec3(0.0, -1.0, 0.0);
    glm::vec3 y_up = glm::vec3(0.0, 1.0, 0.0);
};

#endif /* CAMERA_H */
