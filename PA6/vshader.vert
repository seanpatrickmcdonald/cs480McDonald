#version 330
layout (location = 0) attribute vec2 v_position;
     layout (location = 1) varying vec2 uvCoord;


uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

attribute vec2 v_position;
varying vec2 uvCoord;


void main(void){

gl_TexCoord[0] = uvCoord;

gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(v_position, 1.0); 

}
