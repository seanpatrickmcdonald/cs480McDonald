#version 330 core

out vec4 out_color;

in vec2 uv;

uniform sampler2D mySampler;
uniform sampler2DShadow shadowSpotSampler;
uniform samplerCube shadowCubeSampler;

in vec3 v_N;
in vec3 v_L;
in vec3 v_V;
in vec3 v_Lspot[25];
in vec3 v_Ls[4];

in vec3 world_position;
in vec4 ShadowCoord;

uniform vec3 diffuse_albedo = vec3(0.8);
uniform vec3 specular_albedo = vec3(0.5);
uniform float specular_power = 94.117647;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);
uniform float brightness = 1.0;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform int num_spots;
uniform vec3 spot_position[25];
uniform vec3 spot_focus[25];
uniform float spot_inner_radius = 30.0;
uniform float spot_outer_radius = 32.0;
uniform float spot_max_brightness = 1.0;

uniform float light_strength;
uniform float light_falloff = 2;


uniform int num_lights;
uniform vec3 light_positions[4];

//255, 102, 0
uniform vec3 light_color = vec3(1.0, 102.0/255.0, 0.0);
//uniform vec3 light_color = vec3(1.0, 1.0, 1.0);

#define EPSILON 0.00001

float CalcShadowFactor(vec3 LightDirection)
{
    float SampledDistance = texture(shadowCubeSampler, LightDirection, EPSILON).r;
    //float SampledDistance = 1;

    float Distance = length(LightDirection);

    if (Distance < SampledDistance + EPSILON)
        return 1.0; // Inside the light
    else
        return 0.0; // Inside the shadow
} 

void main(void)
{

  vec3 N = normalize(v_N);
  vec3 L;
  vec3 V = normalize(v_V);

  vec3 R;

  vec4 light = vec4(0, 0, 0, 0);

  for (int i = 0; i < num_lights; i++)
  {
    L = normalize(v_Ls[i]);
    R = reflect(-L, N);

    float x_diff = pow(world_position.x - light_positions[i].x, 2);
    float y_diff = pow(world_position.y - light_positions[i].y, 2);
    float z_diff = pow(world_position.z - light_positions[i].z, 2);
    float distance_from_light = sqrt(x_diff + y_diff + z_diff);
    float distance_factor = distance_from_light / light_strength;

    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo / (distance_factor) * light_color;
    vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo * light_color;

    float shadowFactor;

    if (i == 0)
    shadowFactor = CalcShadowFactor(world_position - light_positions[0]);

    else
    shadowFactor = 1.0;

    light = vec4(shadowFactor * (diffuse + specular) + ambient, 1.0);
    //light = vec4((diffuse + specular) + ambient, 1.0);
  }

  //Process Spot Lights
  for (int i = 0; i < num_spots; i++)
  {
    vec3 spotPosition = normalize(world_position - spot_position[i]);
    vec3 spotDirection = normalize(spot_focus[i] - spot_position[i]);
    float angle = degrees(acos(dot(normalize(spotDirection), normalize(spotPosition))));

    //float spot_distance = distance(spot_position[0], world_position) ;
    //angle = angle * spot_distance / 5;
    
    if (angle < spot_outer_radius && angle >= 0)
    {
      L = normalize(v_Lspot[i]);
      R = reflect(-L, N);

      vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
      vec3 specular = pow(max(dot(R, V), 0.0), specular_power) * specular_albedo;

      vec3 spot_brightness;
      float angle_modifier = spot_outer_radius / 5.0f;

      float outin_ratio = spot_inner_radius / (spot_outer_radius - spot_inner_radius);

      if (angle <= spot_inner_radius)
        spot_brightness = diffuse + specular;
      else if (angle < spot_outer_radius)
        spot_brightness =(diffuse + specular) * vec3((exp(-(angle - spot_inner_radius) * outin_ratio / angle_modifier)));

      float bias = 0.005;

      vec4 shadowCoord = ShadowCoord;
      shadowCoord.z -= bias;

      float visibility = textureProj( shadowSpotSampler, shadowCoord);   

      light += vec4(spot_brightness * visibility, 1.0);
    } 
  }

  
  //Clamp light to 1.0
  //light = vec4(clamp(light.x, 0, 1), clamp(light.y, 0, 1), clamp(light.z, 0, 1), light.w);

  out_color = texture(mySampler, uv) * light;
  //out_color = light;
}

