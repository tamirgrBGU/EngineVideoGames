#version 130

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec2 texCoord;

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 color0;

uniform mat4 MVP;
uniform mat4 Normal;

void main()
{
	
	texCoord0 = texCoord;
	color0 = color;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	gl_Position = MVP * vec4(position, 1.0);
}
