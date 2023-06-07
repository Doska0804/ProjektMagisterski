#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;
uniform float near_plane;
uniform float far_plane;
uniform int isExponential;

void main()
{   
    float depthValue;
    if (isExponential > 0)
        depthValue = log(texture(depthMap, TexCoords).r)/80;
    else
        depthValue = texture(depthMap, TexCoords).r;

    FragColor = vec4(vec3(depthValue, depthValue, depthValue), 1.0);
}