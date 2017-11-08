#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec2 uv;
out vec3 v_N;
out vec3 v_L;
out vec3 v_V;
out vec3 world_position;

uniform vec3 light_position = vec3(-100.0, 100.0, 100.0);

void main(void)
{
  uv = v_uv;

  world_position = mat3(model_matrix) * v_position; 

  mat4 vm_matrix = view_matrix * model_matrix;
  
  //Calculate view-space coord
  vec4 P = vm_matrix * vec4(v_position, 1.0);
 
  //Calculate Normal in View Space
  v_N = mat3(vm_matrix) * v_normal;
  //Calculate Light Vector
  v_L = light_position - P.xyz;
  //Calculate View Vector
  v_V = -P.xyz;

  //clip space out
  gl_Position = projection_matrix * P;
}
