attribute vec4 vPosition;
attribute vec4 vNormal;

varying vec3 N;
varying vec3 L;
varying vec3 E;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 lightSource;
uniform vec4 cameraPosition;

void main()
{
	// compute vPosition in world space
	vec4 vPositionWorld = model * vPosition;
	
	// compute normal in world space
	N = (model * vNormal).xyz;

	// compute eye direction
	E = (cameraPosition - vPositionWorld).xyz;

	if (lightSource[3].w == 0.0)
		L = lightSource[3];
	else
		L = lightSource[3] - vPositionWorld;
	
	// compute gl_Position
	gl_Position = projection * camera * vPositionWorld;
}
