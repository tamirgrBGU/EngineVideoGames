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
uniform vec4 Camdir;

out vec3 LightIntensity;
out vec2 TexCoord;

void main()
{
	vec3 tnorm =  - vec3(normalize(Normal * vec4(normal,0)));
	vec3 lightVec = normalize(vec3(lightDirection));
	
	float specularStrength = 0.5f;
	vec3 viewDir = vec3(Camdir.x,Camdir.y,Camdir.z);
	vec3 reflectDir = reflect(-lightVec, tnorm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
	vec3 specular = specularStrength * spec * vec3(lightColor);  
	float diffuse = max(dot(lightVec, tnorm), 0.0);

	LightIntensity = clamp(vec3(lightColor)*vec3(0.5,0.5,0.5) + diffuse * vec3(lightColor)+ specular,0,1);
	TexCoord = texCoords;
	gl_Position = MVP*vec4(position,1.0);
		
}
