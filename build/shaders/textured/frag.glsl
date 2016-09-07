uniform sampler2D myTexture;
varying vec2 vTexCoord;

void main(void) {
	gl_FragColor = texture2D(myTexture, vTexCoord).rgba * vec4(1, 1, 1, 1.0);
}
