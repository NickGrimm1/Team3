#version 150 core

uniform sampler2D diffuseTex;

in Vertex	{
	vec2 texCoord;
	float depth;
} IN;

out vec4 gl_FragColor;

void main(void)	{
//	if (texture(diffuseTex, IN.texCoord).a < 0.0001) discard;
	gl_FragColor.r = IN.depth;
}