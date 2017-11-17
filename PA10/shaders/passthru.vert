#version 330

layout (location = 0) in vec3 v_in;

smooth out vec3 color;

uniform mat4 projection_matrix, view_matrix;

void main(void)
{
  gl_Position = projection_matrix * view_matrix * vec4(v_in, 1.0);

  color = vec3(1.0, 1.0, 1.0);
}
