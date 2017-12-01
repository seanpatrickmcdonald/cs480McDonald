#version 330

out vec4 out_color;

in vec2 uv;
uniform sampler2D mySampler;

in vec3 v_N;
in vec3 v_L;
in vec3 v_V;

in vec3 world_position;

uniform vec3 diffuse_albedo = vec3(0.5);
uniform vec3 specular_albedo = vec3(0.5);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.0, 0.0, 0.0);
uniform float brightness = 0.250;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform vec3 spot_position = vec3(0.0, 10.0, 0.0);
uniform vec3 spot_focus = vec3(0.0, -1.0, 0.0);
uniform float spot_angle = 5.0;

void main(void)
{
  vec3 N = normalize(v_N);
  vec3 L = normalize(v_L);
  vec3 V = normalize(v_V);

  vec3 R = reflect(-L, N);

  vec3 diffuse = max(dot(N, L), 0.0) + diffuse_albedo;
  vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

  vec4 light = vec4(brightness * (diffuse + specular) + ambient, 1.0);

  vec3 lightPosition = normalize(world_position - spot_position);
  vec3 spotDirection = normalize(spot_focus - spot_position);
  //vec4 vertex = view_matrix * model_matrix * vec4(world_position, 1.0);
  //vec3 lightDirection = normalize(vertex.xyz - lightPosition.xyz);
  float angle = degrees(acos(dot(normalize(spotDirection), normalize(lightPosition))));
  if (angle < spot_angle)
  {
     float spot_brightness = angle / 5.0f;
     light = vec4( spot_brightness, spot_brightness, spot_brightness, 1.0); 
  }





  out_color = texture(mySampler, uv) * light;
  //out_color = light;
}

