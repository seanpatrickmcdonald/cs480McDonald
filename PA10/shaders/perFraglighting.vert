#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;
 
uniform mat4 model_matrix, view_matrix, projection_matrix;
 
//send them to fragment shader
out vec2 uv;
out vec3 world_pos;
out vec3 world_normal;

uniform float ambient = 0.1;
 
void main()
{
 //convert in world coords
 uv = v_uv;
 world_pos = mat3(model_matrix) * v_position;//careful here
 world_normal = normalize(mat3(model_matrix) * v_normal);
 gl_Position = projection_matrix*view_matrix*model_matrix*vec4(v_position,1);
}
