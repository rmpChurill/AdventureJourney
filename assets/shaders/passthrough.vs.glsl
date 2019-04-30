#version 430 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 clr;

layout(std140, binding = 0) uniform objectDataBlock
{
    mat4 model;
    mat3 normal;
} object;

layout(std140, binding = 1) uniform cameraDataBlock
{
    mat4 view;
    mat4 invView;
    mat4 projection;
    mat4 invProjection;
} camera;

out vec4 vclr;

void main()
{
	gl_Position = camera.projection * camera.view * object.model * vec4(pos, 1.0);

    vclr = vec4(clr / 255.0, 1.0);
}