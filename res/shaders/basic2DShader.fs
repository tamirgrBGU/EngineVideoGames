#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform int time;
uniform sampler2D sampler1;

void main()
{

	gl_FragColor = vec4(1,0,0,0);//texture2D(sampler1, texCoord0)*vec4(color0,1); //you must have gl_FragColor
}
