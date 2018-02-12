#version 150

// input vertex position and normal, in world-space
in vec3 position;
in vec3 normal;
in vec2 texCoord;

// vertex position and normal, in view-space
// these will be passed to fragement program(interpolated by hardware)
out vec3 viewPosition;
out vec3 viewNormal;
out vec2 tc;

// transformation matrices
uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 normalMatrix;

void main()
{
	// view-space position of the vertex
	vec4 viewPosition4 = modelViewMatrix * vec4(position, 1.0f);
	viewPosition = viewPosition4.xyz;

	// view-space normal
	vec4 viewNormal4 = normalMatrix * vec4(normal, 0.0f);
	viewNormal = normalize(viewNormal4.xyz);

	// final position in the normalized device coordinates space
	gl_Position = projectionMatrix * viewPosition4;
	// pass-through the texture coordinate
	tc = texCoord;
}
