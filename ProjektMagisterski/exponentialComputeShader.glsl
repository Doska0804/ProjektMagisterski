#version 450 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
	
layout(r32f, binding = 0) uniform image2D imageInput;
layout(r32f, binding = 1) uniform image2D imgOutput;

uniform float gaussianKernel[25];
	
void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    float value = 0;
    for (int i = -2; i <= 2; i++)
        for (int j = -2; j <= 2; j++)
        {
            int x = texelCoord.x + i;
            if (x < 0)
                x = 0;
            else if (x > 1023)
                x = 1023;
            int y = texelCoord.y + j;
            if (j < 0)
                j = 0;
            else if (j > 1023)
                j = 1023;
            value = imageLoad(imageInput, texelCoord).r * gaussianKernel[(i+2)*5 + j+2];
        }


    imageStore(imgOutput, texelCoord, vec4(value, 0, 0, 0));
}