#version 330
  
  in vec3 diffuse_color; 
  in vec3 specular_color; 
  in vec2 uv;

  uniform sampler2D mySampler;  

  void main(void) 
  { 
     gl_FragColor = vec4(diffuse_color * vec3(texture(mySampler, uv)) + specular_color, 1.0); 
  } 
