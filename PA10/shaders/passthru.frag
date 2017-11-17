#version 330

smooth in vec3 color;

out vec4 color_out;

void main(void)
{
    color_out = vec4(color.rgb, 1.0);
}
