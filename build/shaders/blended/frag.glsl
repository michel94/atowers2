uniform sampler2D myTexture;
uniform vec4 colorMix;
varying vec2 vTexCoord;

void main(void) {
	gl_FragColor = texture2D(myTexture, vTexCoord).rgba * colorMix;
}
