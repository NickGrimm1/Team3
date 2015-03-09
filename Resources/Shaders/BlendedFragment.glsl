#version 150
uniform sampler2D diffuseTex;
uniform vec4 blendColour;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void){
	vec4 texColor = texture(diffuseTex, IN.texCoord);
	if (texColor.a < 0.2) {
		discard;
	} else if ((texColor.r == 0.0) && (texColor.g == 0.0) && (texColor.b == 1.0)) {
		discard;
	}
	else {
		gl_FragColor = texColor * blendColour;
	}
}