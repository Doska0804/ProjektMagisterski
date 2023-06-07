#version 450 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;
	
layout(r32f, binding = 0) uniform image2D imgOutput;
layout(r32f, binding = 1) uniform image2D imageInput;

uniform float gaussianKernel[25];
	
void main() {
    ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
    float value = 0;
    ivec2 size = imageSize(imageInput);
    for (int i = -2; i <= 2; i++)
        for (int j = -2; j <= 2; j++)
        {
            int x = texelCoord.x + j;
            if (x < 0)
                x = 0;
            else if (x > size.x-1)
                x = size.x-1;
            int y = texelCoord.y + i;
            if (y < 0)
                y = 0;
            else if (y > size.y-1)
                y = size.y-1;

            value += imageLoad(imageInput, ivec2(x,y)).r * gaussianKernel[(j+2)*5 + i+2];
        }


    imageStore(imgOutput, texelCoord, vec4(value, 1, 1, 1));
}