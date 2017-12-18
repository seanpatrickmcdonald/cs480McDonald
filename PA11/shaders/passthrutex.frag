#version 330 core

// Ouput data
layout(location = 0) out vec4 frag_color;

uniform sampler2D mySampler;

in vec2 UV;

void main(){
	vec4 color = texture(mySampler, UV);
	frag_color = vec4(color.r, color.r, color.r, color.w);
}