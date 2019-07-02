#version 330

in vec3 color0;

//varying vec2 MCposition;
in vec3 LightIntensity;
in vec2 TexCoord;
uniform sampler2D ourTexture;

void main()
{
	gl_FragColor = vec4(color0,1);
	//vec4 t = texture2D(ourTexture, TexCoord.xy);
	//vec4 t = texture2D(ourTexture, gl_TexCoord[0].xy);
	//gl_FragColor =  t* vec4(LightIntensity,1); //you must have gl_FragColor
}