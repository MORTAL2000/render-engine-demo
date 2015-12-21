attribute vec4 vPosition;
attribute vec4 vNormal;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec3 cubeMapCoord;
varying vec4 shadowCubeMapLightDirDepth;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 lightSource;
uniform vec4 cameraPosition;
uniform vec2 shadowZRange;
uniform bool useShadowCubeMap;
uniform bool reflective;

// http://stackoverflow.com/questions/21293726/opengl-project-shadow-cubemap-onto-scene
float vecToDepth (vec3 Vec)
{
  vec3  AbsVec     = abs (Vec);
  float LocalZcomp = max (AbsVec.x, max (AbsVec.y, AbsVec.z));

  float n = shadowZRange [0]; // Near plane when the shadow map was built
  float f = shadowZRange [1]; // Far plane when the shadow map was built

  float NormZComp = (f+n) / (f-n) - (2.0*f*n)/(f-n)/LocalZcomp;
  return (NormZComp + 1.0) * 0.5;
}

void main()
{
	// compute vPosition in world space
	vec4 vPositionWorld = model * vPosition;
	
	// compute normal in world space
	N = normalize((model * vNormal).xyz);

	// compute eye direction
	E = (cameraPosition - vPositionWorld).xyz;

	vec4 lightDir;
	if (lightSource[3].w == 0.0)
		lightDir = -lightSource[3];
	else
		lightDir = vPositionWorld - lightSource[3];

	L = -lightDir.xyz;
	
	if (useShadowCubeMap)
	{
		float lightDepth = vecToDepth(lightDir.xyz);
		shadowCubeMapLightDirDepth = vec4(lightDir.xyz, lightDepth - 0.0002);
	}
	
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
