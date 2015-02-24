#version 330 core

// Performs 5x5 matrix Gaussian blur via 2 passes (vertical/horizontal as determined by isVertical)

uniform sampler2D diffuseTex; // our generated texture to post process
uniform vec2 pixelSize;
uniform int isVertical; // controls whether blurring vertically/horizontally

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 gl_FragColor;


const float weights[9] = float[](0.05, 0.09, 0.11, 0.15, 0.2, 0.15, 0.11, 0.09, 0.05);

void main(void) {
	vec2 values[9];

	if (isVertical == 1) { // Vertical pass
	values = vec2[](vec2(0.0, -pixelSize.y * 4),
						vec2(0.0, -pixelSize.y * 3),
						vec2(0.0, -pixelSize.y * 2), 
						vec2(0.0, -pixelSize.y),
						vec2(0.0, 0.0),
						vec2(0.0, pixelSize.y),
						vec2(0.0, pixelSize.y * 2),
						vec2(0.0, pixelSize.y * 3),
						vec2(0.0, pixelSize.y * 3));
	}
	else {
		values = vec2[](vec2(-pixelSize.x * 4, 0.0),
						vec2(-pixelSize.x * 3, 0.0),
						vec2(-pixelSize.x * 2, 0.0), 
						vec2(-pixelSize.x , 0.0), 
						vec2(0.0, 0.0),
						vec2(pixelSize.x, 0.0),
						vec2(pixelSize.x * 2, 0.0),
						vec2(pixelSize.x * 3, 0.0),
						vec2(pixelSize.x * 4, 0.0));
	}

	for (int i = 0; i < 9; ++i) {
		vec4 tmp = texture2D(diffuseTex, IN.texCoord + values[i]);
		gl_FragColor += tmp * weights[i];
	}
}