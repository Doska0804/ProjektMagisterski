#version 450 core
out vec4 FragColor;

in vec3 fragmentPos;  
in vec3 normal;
in vec4 fragmentPosLight;

uniform vec3 lightPos;

uniform sampler2D shadowMap;

int constant = 80;
float epsilon = 0.02;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz * 0.5 + 0.5;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    if (projCoords.x > 1.0|| projCoords.x < 0|| projCoords.y > 1.0 || projCoords.y < 0)
        return 1;

    float value = exp(-constant * projCoords.z) * texture(shadowMap, projCoords.xy).r;
    if (value > 1+epsilon)
    {
        float mean = clamp(value, 0.0, 1.0);
        for (int i = -1; i <= 1; i++)
        {
            if (i == 0)
                continue;
            mean += clamp(exp(-constant * projCoords.z) * texture(shadowMap, projCoords.xy + vec2(texelSize.x * i, 0)).r, 0.0, 1.0);
            mean += clamp(exp(-constant * projCoords.z) * texture(shadowMap, projCoords.xy + vec2(0, texelSize.y * i)).r, 0.0, 1.0);
        }
        mean /= 5;
        return mean;
    }

    return clamp(value, 0.0, 1.0);
}

void main()
{

    vec3 ambient = vec3(0.1, 0.1, 0.1);
  	
    // diffuse 
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragmentPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(0.9, 0.9, 0.9);
        
    vec3 result = (ambient + diffuse* ShadowCalculation(fragmentPosLight)) * vec3(1.0, 1.0, 1.0);
    FragColor = vec4(result, 1.0);
} 