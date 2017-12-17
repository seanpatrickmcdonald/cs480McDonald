#version 330

in vec3 WorldPos;

uniform vec3 gLightWorldPos = vec3(0.0, 1.0, 0.0);

out float FragColor;

void main()
{
    vec3 LightToVertex = WorldPos - gLightWorldPos;

    float LightToPixelDistance = length(LightToVertex);

    FragColor = LightToPixelDistance;
} 