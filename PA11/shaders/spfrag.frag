#version 330

out vec4 out_color;

in vec2 uv;
uniform sampler2D mySampler;

in vec3 v_N;
in vec3 v_L;
in vec3 v_V;


in vec3 world_position;

uniform vec3 diffuse_albedo = vec3(0.8);
uniform vec3 specular_albedo = vec3(0.5);
uniform float specular_power = 94.117647;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);
uniform float brightness = 1.0;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform vec3 spot_position = vec3(5.0, 10.0, 0.0);
uniform vec3 spot_focus = vec3(0.0, -1.0, 0.0);
uniform float spot_inner_radius = 10.0;
uniform float spot_outer_radius = 10.5;
uniform float spot_max_brightness = 1.0;

uniform vec3 light_position;
uniform float light_strength;
uniform float light_falloff = 2;

//255, 102, 0
uniform vec3 light_color = vec3(1.0, 102.0/255.0, 0.0);

void main(void)
{
  float x_diff = pow(world_position.x - light_position.x, 2);
  float y_diff = pow(world_position.y - light_position.y, 2);
  float z_diff = pow(world_position.z - light_position.z, 2);
  float distance_from_light = sqrt(x_diff + y_diff + z_diff);
  float distance_factor = distance_from_light / light_strength;

  vec3 N = normalize(v_N);
  vec3 L = normalize(v_L);
  vec3 V = normalize(v_V);

  vec3 R = reflect(-L, N);

  vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo / (distance_factor) * light_color;
  vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo * light_color;

  vec4 light = vec4(diffuse + specular + ambient, 1.0);

  vec3 lightPosition = normalize(world_position - spot_position);
  vec3 spotDirection = normalize(spot_focus - spot_position);
  float angle = degrees(acos(dot(normalize(spotDirection), normalize(lightPosition))));

  if (angle < spot_outer_radius)
  {
    float spot_brightness;
    float angle_modifier = spot_outer_radius / 5.0f;

    float outin_ratio = spot_inner_radius / (spot_outer_radius - spot_inner_radius);

    if (angle <= spot_inner_radius)
      spot_brightness = spot_max_brightness;
    else if (angle < spot_outer_radius)
      spot_brightness = (exp(-(angle - spot_inner_radius) * outin_ratio / angle_modifier));

    if (light.x < spot_brightness)
    {
      light.x = spot_brightness;
    }

    if (light.y < spot_brightness)
    {
      light.y = spot_brightness;
    }

    if (light.z < spot_brightness)
    {
      light.z = spot_brightness;
    }
  }

  //Clamp light to 1.0
  light = vec4(clamp(light.x, 0, 1), clamp(light.y, 0, 1), clamp(light.z, 0, 1), light.w);

  out_color = texture(mySampler, uv) * light;
  //out_color = light;
}

