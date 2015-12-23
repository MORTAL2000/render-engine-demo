attribute vec4 vPosition;
attribute vec4 vNormal;
attribute vec2 vTextureCoordinate;

varying vec3 N;
varying vec3 L;
varying vec3 E;
varying vec2 fTextureCoord;

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
	
	// pass texture coordinates to be interpolated over fragments
	fTextureCoord = vec2((vTextureCoordinate.x), (vTextureCoordinate.y));
	
	// compute gl_Position
	gl_Position = projection * camera * vPositionWorld;
}
