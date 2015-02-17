#version 430 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

in vec3 position;

out Vertex {
	vec3 normal; // not really a normal is this situation, its the vector from camera to the skybox via the vertex, transformed via camera rotation
} OUT;			// this is automatically interpolated by the graphics card before entry into the frag shader

void main(void)	{
	vec3 tempPos = position - vec3(0,0,1); // Direction Vector from camera (or just behind camera) to vertex - avoids looking flat
	// Want to rotate direction vector to point to where it would be in camera space. Do this using the inverse of the viewMatrix
	OUT.normal = transpose(mat3(viewMatrix)) * tempPos; // Because not particularly interested in translation (skybox will look same no matter position)
	//OUT.normal = inverse(mat3(viewMatrix)) * tempPos;   // can just take the rotation segment and use the transpose instead of the inverse (as the two are equal for rotation)
	gl_Position = projMatrix * vec4(tempPos, 1.0); // sort out pos for frag shader
}