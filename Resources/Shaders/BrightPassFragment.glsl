/*
	~ Bright-pass Fragment Shader ~
 Author: Sherif Salem
 Date: 	09/02/2015
 Summary:
	Produces an image with the 'bright' areas accentuated, and non-bright areas made black.
*/

#version 430 core

uniform sampler2D diffuseTex;
uniform float luminance;
uniform float middleGrey;
uniform float whiteCutOff;


in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

//'Average' luminance of scene.
//const float luminance = 0.09;
//Target luminance of scene.
//const float middleGrey = 0.18;
//Minimum value to make white.
//const float whiteCutOff = 0.8;

void main(void) {
	gl_FragColor = texture2D(diffuseTex, IN.texCoord);
	
	//Increases scene brightness.
	gl_FragColor *= middleGrey / ( luminance + 0.001);
	gl_FragColor *= (1.0 + ( gl_FragColor / (whiteCutOff * whiteCutOff)));
	
	//Removes non-bright areas.
	gl_FragColor -= 5.0f;
	
	gl_FragColor = max(gl_FragColor, 0.0);
	
	gl_FragColor /= (10.0 + gl_FragColor);
    gl_FragColor.a = 1.0;
}

    