#version 430 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

uniform mat4 prevMVP;

in vec3 position;

out Vertex	{
	smooth vec4 vPosition;
	smooth vec4 vPrevPosition;
} OUT;
	
void main(void) {
	OUT.vPosition = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
    OUT.vPrevPosition = prevMVP * vec4(position, 1.0);

    gl_Position = OUT.vPosition;
}