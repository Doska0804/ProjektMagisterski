#version 450 core
layout (location = 0) in vec3 inPos;

uniform mat4 vpMatrix;
uniform mat4 model;

void main()
{
    gl_Position = vpMatrix * model * vec4(inPos, 1.0);
}  