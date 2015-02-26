/*
	~ Bloom Final Combine Fragment Shader ~
 Author: Sherif Salem
 Date: 	18/02/2015
 Summary:
	Combines the darkpass image with the original scene image to produce the final the bloom image.
*/

#version 430 core

uniform sampler2D diffuseTex;
uniform sampler2D darkTex;
uniform float mixVal;

in Vertex {
	vec2 texCoord;
} IN;


out vec4 gl_FragColor;

void main(void) {
	//gl_FragColor = mix(texture2D(darkTex, IN.texCoord), texture2D(diffuseTex, IN.texCoord), mixVal);
	gl_FragColor = texture2D(diffuseTex, IN.texCoord) + texture2D(darkTex, IN.texCoord);
}