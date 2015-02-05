#version 430 core

uniform samplerCube cubeTex;

in Vertex {
	vec3 normal;
} IN;

out vec4 gl_FragColor;

void main() {
	gl_FragColor = texture(cubeTex, normalize(IN.normal));
}