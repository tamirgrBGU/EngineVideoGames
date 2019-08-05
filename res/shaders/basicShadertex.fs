#version 130 

//varying vec2 MCposition;
in vec3 LightIntensity;
in vec2 TexCoord;
uniform sampler2D ourTexture;



void main()
{
	vec4 t = texture2D(ourTexture, TexCoord.xy);
	gl_FragColor = clamp(t*vec4(LightIntensity,1),0,1); 
}