
#version 450 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightVPMatrix;
uniform mat4 tiModel;

out vec3 fragmentPos;
out vec3 normal;
out vec4 fragmentPosLight;

void main()
{
    fragmentPos = vec3(model * vec4(inPos, 1.0));
    normal = mat3(tiModel) * inNormal;
    fragmentPosLight = lightVPMatrix * vec4(fragmentPos, 1.0);
    gl_Position = projection * view * model * vec4(inPos, 1.0);
}       
