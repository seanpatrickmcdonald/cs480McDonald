#version 330
          
          varying vec2 texture;
		  uniform sampler2D gSampler;
          
          void main(void) 
          { 
             gl_FragColortotexture2D(gSampler, texture.xy)

          } 
