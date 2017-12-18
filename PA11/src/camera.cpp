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

    camera_speed *= 2;
	euler_rotation_angle = 3.141592653/2 + 3.141592653;
  
	return true;
}

void Camera::Update(unsigned int dt)
{

	float speed = camera_speed * dt;
    /*
	glm::vec3 translationVector = glm::vec3(speed * movement[CAMERA_TRANSLATE][CAMERA_X],
								 		    speed * movement[CAMERA_TRANSLATE][CAMERA_Y],
								 		    speed * movement[CAMERA_TRANSLATE][CAMERA_Z]);
	Translate(translationVector);

	*/

    if (movement[CAMERA_ROTATE][CAMERA_X] == 1)
    {
    	euler_rotation_angle -= speed;
    }

    else if (movement[CAMERA_ROTATE][CAMERA_X] == -1)
    {
    	euler_rotation_angle += speed;
    }
}

void Camera::Translate(glm::vec3 translationVector)
{
	eye_position = eye_position + translationVector;
	eye_focus = eye_focus + translationVector;

	view = glm::lookAt( eye_position, eye_focus, y_up);
}

void Camera::LookAt(glm::vec3 lookAtVector)
{
	eye_focus = lookAtVector;

	view = glm::lookAt( eye_position, eye_focus, y_up);
}

void Camera::SetPosition(glm::vec3 positionVector)
{
	eye_position = glm::vec3(4 * cos(euler_rotation_angle) + positionVector.x, 
							 positionVector.y, 
							 4 * sin(euler_rotation_angle) + positionVector.z);

	view = glm::lookAt( eye_position, eye_focus, y_up);
}

void Camera::Rotate(float eulerAngle)
{
	//float speed = camera_speed * dt;

	euler_rotation_angle += eulerAngle;
}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return view;
}


