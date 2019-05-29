#version 330

in vec3 color0;

uniform sampler2D sampler;
uniform vec4 lightDirection;
uniform vec4 lightColor;

void main()
{
	gl_FragColor = vec4(color0,1);
}
