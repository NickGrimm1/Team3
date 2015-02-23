#version 430 core

in Vertex {
	smooth vec4 vPosition;
	smooth vec4 vPrevPosition;
} IN;
	
out vec2 oVelocity;

void main(void) {
	vec2 a = (IN.vPosition.xy / IN.vPosition.w) * 0.5 + 0.5;
	vec2 b = (IN.vPrevPosition.xy / IN.vPrevPosition.w) * 0.5 + 0.5;
	oVelocity = a - b;
	//oVelocity = vec2(1.0, 0.0);
}