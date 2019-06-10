#version 130

varying vec2 pass_texCoords;
varying vec3 pass_normal;
varying vec3 color0;

uniform sampler2D sampler;
uniform vec3 lightDirection;
uniform vec3 lightColor;

void main()
{
    vec3 tmp = dot(-lightDirection, pass_normal) * color0 ;
	gl_FragColor = texture2D(sampler, pass_texCoords) *clamp(vec4(tmp,1.0), 0.0, 1.0);
    //gl_FragColor = clamp(vec4(color0,1),0.0,1.0);
}
