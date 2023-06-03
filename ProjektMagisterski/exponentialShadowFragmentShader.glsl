#version 450 core

out float depthOutput;

void main()
{             
	depthOutput = exp(-gl_FragCoord.z * 80);
}  