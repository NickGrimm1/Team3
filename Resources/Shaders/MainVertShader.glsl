#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;
uniform mat4 shadowProjMatrix[10];
uniform vec4 nodeColour;

in vec3 position;
in vec3 colour;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out Vertex { 
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	vec4 shadowProj[10];
	vec3 shadowPos;
} OUT;

void main(void)	{
	OUT.colour = nodeColour;
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));  //TODO - pass into shader rather than calc each time
	OUT.normal = normalize(normalMatrix * normalize(normal));
	//OUT.normal = normal;
	
	OUT.tangent = normalize(normalMatrix * normalize(tangent));
	OUT.binormal = normalize(normalMatrix * normalize(cross(normal, tangent)));
	OUT.worldPos = (modelMatrix * vec4(position, 1.0)).xyz;
	for (int i = 0; i < 10; i++) {
		OUT.shadowProj[i] = shadowProjMatrix[i] * modelMatrix * vec4(position + (normal * 0.5) , 1);
	}
	OUT.shadowPos = (modelMatrix * vec4(position + (normal * 0.5), 1.0)).xyz; // world space space position shifted towards light source to avoid shadow acne
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}