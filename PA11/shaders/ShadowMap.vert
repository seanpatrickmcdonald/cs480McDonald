#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 v_position;
/*
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_uv;
*/

// Values that stay constant for the whole mesh.
uniform mat4 depthMVP;
uniform mat4 model_matrix = mat4(1.0f);

void main(){
 vec4 vertexPosition_modelspace = model_matrix * vec4(v_position, 1.0);
 gl_Position =  depthMVP * vertexPosition_modelspace;
}