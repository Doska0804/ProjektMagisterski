#version 450 core

layout(location = 0) out vec4 Output;

int constant = 80;
void main()
{             
	Output = vec4(exp(gl_FragCoord.z * constant), 1.0, 1.0, 1.0);
}  