#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

bool Camera::Initialize(int w, int h)
{
	view = glm::lookAt( eye_position, eye_focus, y_up); 

	projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                   float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                   0.01f, //Distance to the near plane, normally a small value like this
                                   100.0f); //Distance to the far plane,

	for (int i = 0; i < 3; i++){ for (int j = 0; j < 3; j++){ movement[i][j] = 0; }}

	return true;
}

void Camera::Update(unsigned int dt)
{
	float speed = camera_speed * dt;

	glm::vec3 translationVector = glm::vec3(speed * movement[CAMERA_TRANSLATE][CAMERA_X],
								 		    speed * movement[CAMERA_TRANSLATE][CAMERA_Y],
								 		    speed * movement[CAMERA_TRANSLATE][CAMERA_Z]);

	Translate(translationVector);
}

void Camera::Translate(glm::vec3 translationVector)
{
	eye_position = eye_position + translationVector;
	eye_focus = eye_focus + translationVector;

	view = glm::lookAt( eye_position, eye_focus, y_up);
}

void Camera::LookAt(glm::vec3 lookAtVector)
{
	eye_focus = eye_focus + lookAtVector;

	view = glm::lookAt( eye_position, eye_focus, y_up);
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


