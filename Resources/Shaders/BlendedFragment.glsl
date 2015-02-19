#version 150
uniform sampler2D diffuseTex;
uniform vec4 blendColour;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void){
	vec4 texColor = texture(diffuseTex, IN.texCoord);
	gl_FragColor = texColor * blendColour;
}