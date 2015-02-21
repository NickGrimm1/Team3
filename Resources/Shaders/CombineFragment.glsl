#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D normalTex; //w = shadow data
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;

uniform vec4 ambientLight;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main() {
	vec3 diffuse  = texture(diffuseTex, IN.texCoord).xyz;
	vec3 light    = texture(emissiveTex, IN.texCoord).xyz;
	vec3 specular = texture(specularTex, IN.texCoord).xyz;

	float shadow = texture(normalTex, IN.texCoord).w; // shadow factor from MainFragShader pass
														// multiply by diffuse and specular components to make sure shadows at least gets ambient light
	gl_FragColor.xyz = diffuse * ambientLight.xyz; // ambient
	gl_FragColor.xyz += diffuse * light * shadow; // lambert
	gl_FragColor.xyz += specular * shadow; // specular
	gl_FragColor.a = 1.0;
	//gl_FragColor = vec4(shadow, shadow, shadow, 1);
}