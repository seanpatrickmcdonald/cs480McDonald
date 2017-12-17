#include "Light.h"

Light::Light()
{
	m_shadow_map = nullptr;
}

Light::~Light()
{

}

void Light::InitDirectional(LightInitStruct &lightStruct)
{

}

void Light::InitSpot(LightInitStruct &lightStruct)
{
	depthProjectionMatrix = glm::perspective<float>(45.0f, 1.0f, 0.5f, 1000.0f);

	light_position = lightStruct.light_position;
	light_direction = lightStruct.light_direction;
	light_color = lightStruct.light_color;
	m_light_type = lightStruct.light_type;
	angle_inner = lightStruct.angle_inner;
	angle_outer = lightStruct.angle_outer;
	light_strength = lightStruct.light_strength;

    depthViewMatrix = glm::lookAt(light_position, light_direction, glm::vec3(0,1,0));

	if (m_shadow_map != nullptr)
	{
		delete m_shadow_map;
		m_shadow_map = nullptr;
	}

    m_shadow_map = new ShadowMap();
    m_shadow_map->MakeSpot();
}

void Light::InitPoint(LightInitStruct &lightStruct)
{	
	depthProjectionMatrix = glm::perspective<float>(90.0f, 1.0f, 0.5f, 1000.0f);

	light_position = lightStruct.light_position;
	light_color = lightStruct.light_color;
	m_light_type = lightStruct.light_type;
	light_strength = lightStruct.light_strength;

	if (m_shadow_map != nullptr)
	{
		delete m_shadow_map;
		m_shadow_map = nullptr;
	}

    m_shadow_map = new ShadowMap();
    m_shadow_map->MakeCube();
}

void Light::LookAt(glm::vec3 lightDirection)
{
    light_direction = lightDirection;
    depthViewMatrix = glm::lookAt(light_position, light_direction, glm::vec3(0,1,0));
    depthPVMat = depthProjectionMatrix * depthViewMatrix;
}

void Light::SetPosition(glm::vec3 lightPosition)
{
    light_position = lightPosition;
    depthViewMatrix = glm::lookAt(light_position, light_direction, glm::vec3(0,1,0));
    depthPVMat = depthProjectionMatrix * depthViewMatrix;
}

glm::mat4 Light::GetPVMat()
{
	return depthPVMat;
}