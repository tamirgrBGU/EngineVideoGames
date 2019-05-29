#version 330

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec3 weights;
attribute vec2 texCoords;

out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;

uniform mat4 MVP;
uniform mat4 lastMVP;
uniform mat4 nextMVP;
uniform mat4 Normal;

void main()
{
	color0 = weights;	
	texCoord0 = texCoords;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	vec4 pos = weights.z * lastMVP * vec4(position, 1.0);
	pos = pos + weights.y * MVP * vec4(position, 1.0);
	gl_Position = pos + weights.x * nextMVP * vec4(position, 1.0);
}
