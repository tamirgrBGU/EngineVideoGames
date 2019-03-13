#version 130

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;

uniform vec4 lightColor;
uniform sampler2D sampler;
uniform vec4 lightDirection;


void main()
{
    vec3 tmp = dot(-lightDirection.xyz, normal0) * color0 ;
	//gl_FragColor = texture2D(sampler, texCoord0) *clamp(vec4(tmp,1.0), 0.0, 1.0);
    gl_FragColor = clamp(vec4(tmp,1),0.0,1.0);
}
