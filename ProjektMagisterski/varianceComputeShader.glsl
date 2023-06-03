#version 450 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
	
layout(rg32f, binding = 1) uniform image2D imageInput;
layout(rg32f, binding = 0) uniform image2D imgOutput;

uniform float gaussianKernel[25];
	
void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    vec4 value = vec4(0, 0, 1.0, 1.0);
    for (int i = -2; i < 3; i++)
        for (int j = -2; j < 3; j++)
        {
            int x = texelCoord.x + j;
            if (x < 0)
                x = 0;
            else if (x > 1023)
                x = 1023;
            int y = texelCoord.y + i;
            if (y < 0)
                y = 0;
            else if (y > 1023)
                y = 1023;
            value.r += imageLoad(imageInput, ivec2(x,y)).r * gaussianKernel[(j+2)*5 + i+2];
            value.g += imageLoad(imageInput, ivec2(x,y)).g * gaussianKernel[(j+2)*5 + i+2];
        }

    imageStore(imgOutput, texelCoord, value);
}