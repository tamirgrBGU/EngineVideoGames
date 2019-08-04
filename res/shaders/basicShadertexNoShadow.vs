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
	LightIntensity = vec3(0.0);
	TexCoord = texCoords;
	gl_Position = MVP*vec4(position,1.0); //you must have gl_Position
		
}
