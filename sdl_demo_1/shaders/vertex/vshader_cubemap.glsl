attribute vec4 vPosition;
attribute vec4 vNormal;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec3 cubeMapCoord;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 lightSource;
uniform vec4 cameraPosition;
uniform bool reflective;

void main()
{
	// compute vPosition in world space
	vec4 vPositionWorld = model * vPosition;
	
	// compute normal in world space
	N = normalize((model * vNormal).xyz);

	// compute eye direction
	E = (cameraPosition - vPositionWorld).xyz;

	if (lightSource[3].w == 0.0)
		L = lightSource[3];
	else
		L = lightSource[3] - vPositionWorld;
	
	// reflective
	if (reflective)
	{
		vec3 v = normalize(-E);
		cubeMapCoord = v - 2 * dot(v, N) * N;
	}
	// non reflective
	else
	{
		//cubeMapCoord = (vPositionWorld - model[3]).xyz;
		cubeMapCoord = vPosition.xyz;
	}
	
	// compute gl_Position
	gl_Position = projection * camera * vPositionWorld;
}
