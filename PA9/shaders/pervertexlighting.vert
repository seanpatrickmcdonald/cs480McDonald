#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

out vec2 uv;
out vec3 light;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform vec3 light_position = vec3(0.0, 10.0, 10.0);
uniform vec3 diffuse_albedo = vec3(0.8);
uniform vec3 specular_albedo = vec3(0.8);
uniform float specular_power = 1.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

uniform vec3 spot_position = vec3(0.0, 10.0, 0.0);
uniform vec3 spot_focus = vec3(0.0, -10.0, 0.0);
uniform float spot_angle = 5.0;

void main(void)
{
    uv = v_uv;   

    vec4 position = vec4(v_position, 1.0);
    mat4 mv_matrix = view_matrix * model_matrix;
    vec4 P = mv_matrix * position;

    vec3 N = mat3(mv_matrix) * v_normal;
    vec3 L = light_position - P.xyz;
    vec3 V = -P.xyz;

    N = normalize(N);
    L = normalize(L);
    V = normalize(V);
 
    vec3 R = reflect(-L, N);   

    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
    vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

    light = diffuse + ambient + specular;

    vec3 lightPosition = normalize(spot_position - (model_matrix * vec4(v_position, 1.0)).xyz);
    vec3 spotDirection = normalize(spot_focus - spot_position);
    vec4 vertex = view_matrix * model_matrix * vec4(v_position, 1.0);
    vec3 lightDirection = normalize(vertex.xyz - lightPosition.xyz);
    float angle = degrees(acos(dot(normalize(spotDirection), -normalize(lightPosition))));
    if (angle < spot_angle)
       light = vec3(0.95, 0.95, 0.95); 

    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(v_position, 1.0);
}
