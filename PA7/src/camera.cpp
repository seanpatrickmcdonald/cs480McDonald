#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}


bool Camera::Initialize(int w, int h)
{
  scaledView = false;
  scaledUp = true;
  orbitAngle = 0.0f;
  parentEnum = 0;
 
  view = glm::lookAt( detail_eye, detail_foc, y_up); 

  projection = glm::perspective( 45.0f, 
                                 float(w)/float(h),
                                 0.01f, 
                                 100000.0f);
  return true;
}


void Camera::Update(Object *focusObject)
{
  if (!scaledView)
  {
    cur_eye = focusObject->trans_vector + glm::vec3(0.0f, focusObject->scale + 1.0f, 2.0f);
    cur_foc = focusObject->trans_vector;
    view = lookAt(cur_eye, cur_foc, y_up);
  }
}

void Camera::toggleView()
{
  if (scaledView)
  {
    view = glm::lookAt(detail_eye, detail_foc, y_up);
    scaledView = false;
  }

  else
  {    
    if (scaledUp)
    view = glm::lookAt(scaled_all_eye, scaled_all_foc, y_up);

    else
    view = glm::lookAt(scaled_inn_eye, scaled_inn_foc, y_up);

    scaledView = true;
  }

}

void Camera::setUp()
{
  if (!scaledUp && scaledView)
  {
    view = glm::lookAt(scaled_all_eye, scaled_all_foc, y_up);
    scaledUp = true;
  }
}

void Camera::setDown()
{
  if (scaledUp && scaledView)
  {
    view = glm::lookAt(scaled_inn_eye, scaled_inn_foc, y_up);
    scaledUp = false;
  }
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


