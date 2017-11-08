//Phong reflection model; Fragment Shader
//To keep it simple didn't add ambient and emissive lights;
//only diffuse and specular with white intensity
#version 330

out vec4 out_color;
 
uniform vec3 light_position = vec3(5.0, 15.0, -5.0);
uniform vec3 eye_position;

uniform mat4 model_matrix, view_matrix, projection_matrix;


uniform vec3 spot_position = vec3(0.0, 10.0, 0.0);
uniform vec3 spot_direction = vec3(0.0, -10.0, 0.0);
uniform float spot_angle = 5.0;
 
uniform int material_shininess = 1;
 
uniform float material_kd = 0.8;
uniform float material_ks = 0.8;
uniform float ambient = 0.1;

in vec2 uv;
in vec3 world_pos;
in vec3 world_normal;
 

uniform sampler2D mySampler; 

void main(){ 

 
vec3 L = normalize( light_position - world_pos);//light direction
vec3 V = normalize( (vec3(view_matrix * projection_matrix)) - world_pos);//view direction
 
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
 
float light = clamp(diffuse + specular + ambient, 0.0, 1.0);



//Spotlight
vec3 lightPosition = normalize(spot_position - world_pos);
vec3 spotDirection = normalize(spot_direction);
vec4 vertex = view_matrix * vec4(world_pos, 1.0);
vec3 lightDirection = normalize(vertex.xyz - lightPosition.xyz);
float angle = degrees(acos(dot(normalize(spotDirection), -normalize(lightPosition))));
//If we're in the spotlight just override previous lighting and make 95%
if (angle < spot_angle)
   light = 0.95; 




 
out_color = vec4(vec3(texture(mySampler, uv))* light,1);

}
