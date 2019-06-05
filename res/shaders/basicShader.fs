#version 330


in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform vec4 lightColor;
uniform sampler2D sampler;
uniform vec4 lightDirection;


void main()
{
    vec3 eyePosition = vec3(0,0,0);
	vec3 specularContribution =vec3(0.9,0.9,0.9);
	float shiness = 10.0f;
	vec3 tnorm = -normal0;

    float a = 0.5f,b = 10.0f;
    float x = texCoord0.s;
    float y = texCoord0.t;
    tnorm.x =  - normal0.x - a*((b*cos(b*x*y))*sin(b*y*x) + 2*sin(b*x)*(cos(b*y))) ;
    tnorm.y =  - normal0.y - a*(sin(b*x)*(cos(b*y)) +(b*cos(b*x*y))*sin(b*y*x));
   tnorm.z =  - normal0.z -1;
   tnorm = normalize(tnorm); 
	
	vec3 lightVec = normalize(vec3(lightDirection));
	vec3 reflectVec = reflect(lightVec, tnorm);
	vec3 viewVec = normalize(eyePosition - position0);
	float diffuse = max(dot(lightVec, tnorm), 0.0);
	float spec = 0;
	if(diffuse > 0.0)
	{
		spec = max(dot(reflectVec, viewVec), 0.0);
		spec = pow(spec, shiness);
	}
//	vec3 LightIntensity=vec3(lightColor);
	vec3 LightIntensity = clamp(vec3(lightColor)*vec3(0.3,0.3,0.3) + diffuse * color0 * vec3(lightColor) + spec * vec3(lightColor) *specularContribution,0.0,1.0);

	gl_FragColor = texture2D(sampler, texCoord0)* vec4(LightIntensity,1.0); //you must have gl_FragColor
    //gl_FragColor = clamp(vec4(tmp,1),0.0,1.0);
}
