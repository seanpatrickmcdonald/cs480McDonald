//Phong reflection model; Fragment Shader
//To keep it simple didn't add ambient and emissive lights;
//only diffuse and specular with white intensity
#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

out vec4 out_color;
 
uniform vec3 light_position;
uniform vec3 eye_position;
 
uniform int material_shininess;
 
uniform float material_kd;
uniform float material_ks;
 
in vec3 world_pos;
in vec3 world_normal;
 
void main(){
 
 
vec3 L = normalize( light_position - world_pos);//light direction
vec3 V = normalize( eye_position - world_pos);//view direction
 
float LdotN = max(0, dot(L,world_normal));
 
float diffuse = material_kd * LdotN;
 
float specular = 0;
 
if(LdotN > 0.0)
{
 
//choose H or R to see the difference
vec3 R = -normalize(reflect(L, world_normal));//Reflection
specular = material_ks * pow(max(0, dot(R, V)), material_shininess);
 
//Blinn-Phong
// vec3 H = normalize(L + V );//Halfway
//specular = material_ks * pow(max(0, dot(H, world_normal)), material_shininess);
 
}
 
float light = diffuse + specular;
 
out_color = vec4(light,light, light,1);
}
