#version 330
          
          in vec2 uv; 
          
          out vec4 frag_color; 
          
	  uniform sampler2D mySampler;

          void main(void) 
          { 
             frag_color = texture(mySampler, uv); 
          } 
