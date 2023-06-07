#version 450 core
out vec4 FragColor;

in vec3 fragmentPos;  
in vec3 normal;
in vec4 fragmentPosLight;

uniform vec3 lightPos;

uniform sampler2D shadowMap;
int sampleSize = 7;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz * 0.5 + 0.5;
    if (projCoords.x > 1.0 || projCoords.x < 0 || projCoords.y > 1.0 || projCoords.y < 0)
        return 0;
    if(projCoords.z > 1.0)
        return 0;

    float currentDepth = projCoords.z;
    float bias = 0.003;

    

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -3; x <= 3; ++x)
    {
        for(int y = -3; y <= 3; ++y)
        {
            float pcf = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcf  ? 1.0 : 0.0;        
        }    
    }
    shadow /= sampleSize * sampleSize;
    
    
        
    return shadow;
}

void main()
{

    vec3 ambient = vec3(0.1, 0.1, 0.1);
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragmentPos);
    float diff = clamp(dot(norm, lightDir), 0.0, 1.0);
    vec3 diffuse = diff * vec3(0.9, 0.9, 0.9);
        
    vec3 result = (ambient + diffuse * (1.0-ShadowCalculation(fragmentPosLight))) * vec3(1.0, 1.0, 1.0);
    FragColor = vec4(result, 1.0);
} 