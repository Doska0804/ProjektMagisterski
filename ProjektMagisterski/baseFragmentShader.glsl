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
    float currentDepth = projCoords.z;
    
    float bias = 0.005;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcf = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcf  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 25.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{

    vec3 ambient = vec3(0.1, 0.1, 0.1);
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragmentPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.9, 0.9, 0.9);
        
    vec3 result = (ambient + diffuse* (1-ShadowCalculation(fragmentPosLight))) * vec3(1.0, 1.0, 1.0);
    FragColor = vec4(result, 1.0);
} 