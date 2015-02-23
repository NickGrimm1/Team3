/*
	~ Downsampling Fragment Shader ~
 Author: Sherif Salem
 Date: 	09/02/2015
 Summary:
	Produces a lower resolution version of the Bright-pass image, that is used
	in the simulation of a much larger gaussian blur kernel.
*/

#version 430 core

uniform sampler2D diffuseTex;
uniform float sampleLevel;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void) {
	float uppersamp = sampleLevel * sampleLevel;
	gl_FragColor = texture2D(diffuseTex, (IN.texCoord * uppersamp) / sampleLevel);
}