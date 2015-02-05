#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
out mat4 inverseProjView; // Should really be doing this in code - once

void main() {
	gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
	inverseProjView = inverse(projMatrix * viewMatrix); // screen space to world space
}