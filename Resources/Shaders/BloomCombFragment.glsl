/*
	~ Bloom Combine Fragment Shader ~
 Author: Sherif Salem
 Date: 	17/02/2015
 Summary:
	Takes in the downsampled and blurred images and combines them into one image.
*/
#version 430 core

uniform sampler2D downSample0;
uniform sampler2D downSample1;
uniform sampler2D downSample2;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void) {
	gl_FragColor  = texture2D(downSample0, IN.texCoord / 1.5);
	gl_FragColor += texture2D(downSample1, IN.texCoord / 2);
	gl_FragColor += texture2D(downSample2, IN.texCoord / 3);
}