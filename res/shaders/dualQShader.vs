#version 330

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec3 weights;
attribute vec2 texCoords;

out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;

uniform mat2x4 Quaternion;
uniform mat2x4 lastQuaternion;
uniform mat2x4 nextQuaternion;
uniform mat4 Normal;

void main()
{
	color0 = weights;	
	texCoord0 = texCoords;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	mat4 dq;
	dq[0] = weights.x * lastQuaternion[0];
	dq[1] = weights.y * Quaternion[0];
	dq[2] = weights.z * nextQuaternion[0];
	dq[3] = weights.x * nextQuaternion[1] + weights.y * nextQuaternion[1] + weights.z * nextQuaternion[1];
	gl_Position = dq*vec4(position, 1.0);
}

 