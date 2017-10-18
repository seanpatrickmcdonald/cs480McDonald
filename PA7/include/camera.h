#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"
#include "object.h"

const glm::vec3 y_up = glm::vec3(0.0, 1.0, 0.0);

const glm::vec3 scaled_eye = glm::vec3(0.0, 400.0, 0.0125);
const glm::vec3 scaled_foc = glm::vec3(0.0, 0.0, 0.0);

const glm::vec3 detail_eye = glm::vec3(0.0, 3.0, -16);
const glm::vec3 detail_foc = glm::vec3(0.0, 0.0, 0.0);


class Camera
{
  public:
    Camera();
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();
 
    void setFocusPoint(glm::vec3 focusPoint);  
    void setEyePosition(glm::vec3 focusPoint); 
    void Update(Object *focusObject);
    void toggleView();
    bool scaledView;
    bool detailUp; 
    int parentEnum;

  private:
    glm::mat4 projection;
    glm::mat4 view;

    glm::vec3 cur_eye;
    glm::vec3 cur_foc;

    float orbitAngle;
    
};

#endif /* CAMERA_H */
