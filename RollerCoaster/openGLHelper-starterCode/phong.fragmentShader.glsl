#version 150

// interpolated from vertex program outputs
in vec3 viewPosition;
in vec3 viewNormal;

// output color
out vec4 c;

// properties of the directional light
uniform vec4 lightAmbient;
uniform vec4 lightDiffuse;
uniform vec4 lightSpecular;
uniform vec3 viewLightDirection; // in view space

// properties of the mesh material
uniform vec4 matKa;
uniform vec4 matKd;
uniform vec4 matKs;
uniform float matKsExp;

void main()
{
	// camera is at (0, 0, 0) after the modelview transformation
	vec3 eyeDir = normalize(vec3(0,0,0) - viewPosition);
	// reflected light direction
	vec3 reflectDir = -reflect(viewLightDirection, viewNormal);
	// Phong lighting
	float kd = max(dot(viewLightDirection, viewNormal), 0.0f);
	float ks = max(dot(reflectDir, eyeDir), 0.0f);
	// compute the final color
	c = matKa * lightAmbient + matKd * lightDiffuse * kd + matKs * lightSpecular * pow(ks, matKsExp);
}

