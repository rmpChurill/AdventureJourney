#version 430 core

layout(std140,binding = 4) uniform materialDataBlock
{
    vec4 albedo;
    vec4 properties;
} material;

vec4 GetMaterialProps()
{
    return material.properties;
}

