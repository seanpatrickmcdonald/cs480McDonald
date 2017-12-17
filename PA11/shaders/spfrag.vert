#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform mat4 depthMVP;
uniform mat4 cubeDepthMVP;

out vec2 uv;
out vec3 v_N;
out vec3 v_L;
out vec3 v_Ls[25];
out vec3 v_Lspot[25];
out vec3 v_V;
out vec3 world_position;
out vec4 ShadowCoord;

uniform int num_lights = 0;
uniform vec3 light_positions[25]; 

uniform int num_spots = 0;
uniform vec3 spot_position[25];

void main(void)
{
  //Shadow Calculations
  mat4 biasMatrix = mat4(
      0.5, 0.0, 0.0, 0.0, 
      0.0, 0.5, 0.0, 0.0,
      0.0, 0.0, 0.5, 0.0,
      0.5, 0.5, 0.5, 1.0
  );

  mat4 depthBiasMatrix = biasMatrix * depthMVP;

  uv = v_uv;

  world_position = (model_matrix * vec4(v_position, 1.0)).xyz; 

  mat4 vm_matrix = view_matrix * model_matrix;
  
  //Calculate view-space coord
  vec4 P = vm_matrix * vec4(v_position, 1.0);
 
  //Calculate Normal in View Space
  v_N = mat3(vm_matrix) * v_normal;
  //Calculate Light Vector
  v_L = light_positions[0].xyz - P.xyz;

  for (int i = 0; i < num_lights; i++)
  {
    v_Ls[i] = light_positions[i].xyz - P.xyz;
  }

  for (int i = 0; i < num_spots; i++)
  {
    v_Lspot[i] = spot_position[i] - P.xyz;
  }

  //Calculate View Vector
  v_V = -P.xyz;

  //clip space out
  gl_Position = projection_matrix * P;


  ShadowCoord = depthBiasMatrix * vec4(v_position,1);
}
