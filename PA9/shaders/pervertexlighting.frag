#version 330
  
 in vec2 uv;
 in vec3 light;

 out vec4 out_color;

 uniform sampler2D mySampler;  

 void main(void) 
 { 
    out_color = texture(mySampler, uv) * vec4(light, 1.0) ;
 } 
