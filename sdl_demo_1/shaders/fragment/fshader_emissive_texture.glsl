varying vec2 fTextureCoord;

uniform sampler2D tex;

void main()
{
	gl_FragColor = texture2D(tex, fTextureCoord);
}
