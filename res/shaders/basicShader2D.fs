#version 330

in vec3 color0;
in vec2 texCoord0;

uniform sampler2D sampler1;
uniform sampler2D sampler2;
uniform sampler2D sampler3;
uniform int time;

void main()
{
    if(time>0)
	    gl_FragColor = texture2D(sampler1, texCoord0)*texture2D(sampler3, texCoord0).r;
    else
        gl_FragColor = texture2D(sampler1, texCoord0)*texture2D(sampler2, texCoord0);
}
