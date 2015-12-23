attribute vec4 vPosition;
attribute vec4 vNormal;
attribute vec4 vTangent;
attribute vec4 vBinormal;
attribute vec2 vTextureCoordinate;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec2 fTextureCoord;
varying mat4 inverseTBN;
varying vec3 shadowCoordDepth;
varying vec3 vPositionLight;
varying vec4 vPositionWorld;

uniform mat4 model;
uniform mat4 camera;
uniform mat4 projection;
uniform mat4 lightSource;
uniform vec4 cameraPosition;
uniform int shadowMode;
uniform mat4 lightProjection;

void main()
{
	// compute vPosition in world space
	vPositionWorld = model * vPosition;
	
	inverseTBN = mat4(vTangent, vBinormal, vNormal, vec4(0.0, 0.0, 0.0, 0.0));
	
	// compute normal in world space
	N = (model * vNormal).xyz;

	// compute eye direction
	E = (cameraPosition - vPositionWorld).xyz;

	if (lightSource[3].w == 0.0)
		L = lightSource[3];
	else
		L = lightSource[3] - vPositionWorld;

	if (shadowMode == 1)
	{
		vPositionLight = (lightProjection * model * vPosition).xyz;
		shadowCoordDepth = vec3((vPositionLight.x + 1.0) / 2.0, (vPositionLight.y + 1.0) / 2.0, (vPositionLight.z + 1.0) / 2.0 - 0.002);
		//shadowCoordDepth = vec3(vPositionLight.x, vPositionLight.y, vPositionLight.z);
	}
	
	// pass texture coordinates to be interpolated over fragments
	fTextureCoord = vec2((vTextureCoordinate.x), (vTextureCoordinate.y));
	
	// compute gl_Position
	gl_Position = projection * camera * vPositionWorld;
}
