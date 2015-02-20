/*
 Author: Sherif Salem
 Date: 	09/02/2015
 Summary:
	Takes values in the 'velocity buffer', uses that and the velocityScale
	to find the velocity of each object in texels, that is then used
	to define the intensity and size of the blur.
*/
#version 430 core

uniform sampler2D diffuseTex;	//Texture
uniform sampler2D velocityTex;	//Velocity buffer

uniform float velocityScale; //(currentFPS/targetFPS)

out vec4 gl_FragColor;

in Vertex {
	vec2 texCoord;
} IN;

int MAX_SAMPLES = 20;

void main(void) {
	vec2 texSize = 1.0 / vec2(textureSize(diffuseTex, 0));
	
	vec2 velocity = texture2D(velocityTex, IN.texCoord).rg;
	velocity 	*= velocityScale;
	
	//How many samples to take for blur, adapts to velocity.
	float speed = length(velocity / texSize); //Gives us the speed in texels.
	int samples = clamp(int(speed), 1, MAX_SAMPLES);

	gl_FragColor = texture2D(diffuseTex, IN.texCoord);
	
	for (int i = 1; i < samples; ++i) {
		vec2 offset = velocity * (float(i) / float(samples - 1) - 0.5);
		gl_FragColor += texture2D(diffuseTex, IN.texCoord + offset);
	}
	
	gl_FragColor /= float(samples);
}