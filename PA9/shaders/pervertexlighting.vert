#version 330
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec2 v_uv;

out vec3 diffuse_color;
out vec3 specular_color;
out vec2 uv;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform vec3 light_position = vec3(0.0, 10.0, 10.0);
uniform vec3 diffuse_albedo = vec3(1.0, 1.0, 1.0);
uniform vec3 specular_albedo = vec3(1.0);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

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

    diffuse_color = ambient + diffuse;
    specular_color = specular;

    gl_Position = projection_matrix * P;
}
