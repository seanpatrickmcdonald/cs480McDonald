#ifndef LIGHT_H_
#define LIGHT_H_

#include "graphics_headers.h"
#include "ShadowMap.h"

enum LightType
{
	DIRECTIONAL = 0,
	SPOT = 1,
	POINT = 2
};

struct LightInitStruct
{
	glm::vec3 light_position;
	glm::vec3 light_direction;

	float angle_inner;
	float angle_outer;

	float light_strength;
	glm::vec3 light_color;
	LightType light_type;
};

class Light
{
public:

	Light();
	~Light();

	void InitDirectional(LightInitStruct &lightStruct);
	void InitSpot(LightInitStruct &lightStruct);
	void InitPoint(LightInitStruct &lightStruct);

	void SetPosition(glm::vec3 lightPosition);
	void LookAt(glm::vec3 lightDirection);

	glm::mat4 depthProjectionMatrix;
	glm::mat4 depthViewMatrix;
	glm::mat4 depthPVMat;

	glm::vec3 light_position;
	glm::vec3 light_direction;

	float angle_inner;
	float angle_outer;

	float light_strength;
	glm::vec3 light_color;
	LightType m_light_type;

	ShadowMap *m_shadow_map;

	glm::mat4 GetPVMat();
};

#endif