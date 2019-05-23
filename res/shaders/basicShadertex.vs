// Vertex shader for a point-light (local) source, with computation
// done in the fragment shader.
#version 330

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec3 weights;
attribute vec2 texCoords;

uniform mat4 MVP;
uniform mat4 Normal;
uniform vec4 lightColor;
uniform vec4 lightDirection;

out vec3 LightIntensity;
out vec2 TexCoord;

void main()
{
	vec4 eyePosition = vec4(0,0,0,1);
	vec3 specularContribution =vec3(0.7,0.7,0.7);
	float shiness = 10.0f;
	vec3 tnorm =  - vec3(normalize(Normal * vec4(normal,0)));
	
	vec3 lightVec = normalize(vec3(lightDirection));
	vec3 reflectVec = reflect(-lightVec, tnorm);
	vec3 viewVec = vec3(normalize(eyePosition - MVP*vec4(position,1)));
	float diffuse = max(dot(lightVec, tnorm), 0.0);
	float spec = 0;
	if(diffuse > 0.0)
	{
		spec = max(dot(reflectVec, viewVec), 0.0);
		spec = pow(spec, shiness);
	}
	//LightIntensity=color;
	LightIntensity = clamp(vec3(lightColor)*vec3(0.3,0.3,0.3) + diffuse * color * vec3(lightColor) + spec * vec3(lightColor) *specularContribution,0.0,1.0);
	TexCoord = texCoords;
	gl_Position = MVP*vec4(position,1.0); //you must have gl_Position
		
}
