attribute vec4 vPosition;
attribute vec4 vNormal;
attribute vec4 vTangent;
attribute vec4 vBinormal;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec3 cubeMapCoord;
varying mat4 inverseTBN;

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
	
	inverseTBN = mat4(vTangent, vBinormal, vNormal, vec4(0.0, 0.0, 0.0, 0.0));
	
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
