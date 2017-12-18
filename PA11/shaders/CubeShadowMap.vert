#version 330

layout (location = 0) in vec3 Position;

uniform mat4 depthMVP;
uniform mat4 model;

out vec3 WorldPos;

void main()
{
    vec4 Pos4 = vec4(Position, 1.0);
    gl_Position = depthMVP * Pos4;
    WorldPos = (model * Pos4).xyz;
} 