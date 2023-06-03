#version 450 core
out vec4 FragColor;

in vec3 fragmentPos;  
in vec3 normal;
in vec4 fragmentPosLight;

uniform vec3 lightPos;

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz * 0.5 + 0.5;
    vec2 moments = texture(shadowMap, projCoords.xy).rg;
    if (moments.x > projCoords.z)
        return 1;

    float variance = max(moments.y - moments.x * moments.x, 0.00001);
    float compareValue = projCoords.z - moments.x;

    float pMax = variance/(variance+compareValue*compareValue);
        
    return clamp(pMax, 0.0, 1.0);
}

void main()
{

    vec3 ambient = vec3(0.2, 0.2, 0.2);
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragmentPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.8, 0.8, 0.8);
        
    vec3 result = (ambient + diffuse* ShadowCalculation(fragmentPosLight)) * vec3(1.0, 1.0, 1.0);
    FragColor = vec4(result, 1.0);
} 