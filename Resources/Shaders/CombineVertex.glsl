#version 150 core

uniform mat4 projMatrix;

in vec3 position;
in vec2 texCoord;

out Vertex {
	vec2 texCoord;
} OUT;

void main() {
	gl_Position = projMatrix * vec4(position, 1.0); // just projecting texture to quad orthographically
	OUT.texCoord = texCoord;
}