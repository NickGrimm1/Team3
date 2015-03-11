#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D depthTex;
uniform vec2 pixelSize;

in Vertex {
	vec2 texCoord ;
	vec4 colour ;
} IN ;

out vec4 gl_FragColor ;

const mat3 G[9] = mat3[](
	1.0/(2.0*sqrt(2.0)) * mat3( 1.0, sqrt(2.0), 1.0, 0.0, 0.0, 0.0, -1.0, -sqrt(2.0), -1.0 ),
	1.0/(2.0*sqrt(2.0)) * mat3( 1.0, 0.0, -1.0, sqrt(2.0), 0.0, -sqrt(2.0), 1.0, 0.0, -1.0 ),
	1.0/(2.0*sqrt(2.0)) * mat3( 0.0, -1.0, sqrt(2.0), 1.0, 0.0, -1.0, -sqrt(2.0), 1.0, 0.0 ),
	1.0/(2.0*sqrt(2.0)) * mat3( sqrt(2.0), -1.0, 0.0, -1.0, 0.0, 1.0, 0.0, 1.0, -sqrt(2.0) ),
	1.0/2.0 * mat3( 0.0, 1.0, 0.0, -1.0, 0.0, -1.0, 0.0, 1.0, 0.0 ),
	1.0/2.0 * mat3( -1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0 ),
	1.0/6.0 * mat3( 1.0, -2.0, 1.0, -2.0, 4.0, -2.0, 1.0, -2.0, 1.0 ),
	1.0/6.0 * mat3( -2.0, 1.0, -2.0, 1.0, 4.0, 1.0, -2.0, 1.0, -2.0 ),
	1.0/3.0 * mat3( 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 )
);

void main (void) {
	mat3 I;
	float cnv[9];
	vec3 sample;
	vec3 outvec;
	float f = 10000.0;
	float n = 1.0;
	float z;
	
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			sample = texelFetch(depthTex, ivec2(gl_FragCoord) + ivec2(i-1, j-1), 0).rgb;
			z = (2 * n) / (f + n - sample.x * (f - n));	
			I[i][j] = length(z);
		}
	}
	
	for (int i = 0; i < 9; ++i) {
		float dp3 = dot(G[i][0], I[0]) + dot(G[i][1], I[1]) + dot(G[i][2], I[2]);
		cnv[i] = dp3 * dp3;
	}
	
	float M = (cnv[0] + cnv[1]) + (cnv[2] + cnv[3]);
	float S = (cnv[4] + cnv[5]) + (cnv[6] + cnv[7]) + (cnv[8] + M);
		
	//If this produces black, make line black, otherwise use normal colour. vec4(sqrt(M/S));
	float weight = 0.01;
	bvec4 stepVec = greaterThan(vec4(weight, weight, weight, weight), vec4(sqrt(M/S)));
	
	if (stepVec.r && stepVec.g && stepVec.b && stepVec.a) {
		gl_FragColor = texture2D(diffuseTex, IN.texCoord);
	}
	else {
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
}