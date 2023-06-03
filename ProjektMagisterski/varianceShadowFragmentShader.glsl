#version 450 core

layout(location = 0) out vec4 Output;

void main()
{             
	Output = vec4(gl_FragCoord.z, gl_FragCoord.z*gl_FragCoord.z, 1.0, 1.0);
}  