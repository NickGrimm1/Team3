#version 150 core

uniform sampler2D diffuseTex0;
uniform float time;

in Vertex
{
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void)
{
	vec2 move = vec2(0, 1);
	
	gl_FragColor = texture(diffuseTex0, (IN.texCoord) + time * move) / 2;
	gl_FragColor += texture(diffuseTex0, (IN.texCoord) * 2 + time * move)/ 4;
	gl_FragColor += texture(diffuseTex0, (IN.texCoord) * 4 + time * move) / 8;
	gl_FragColor += texture(diffuseTex0, (IN.texCoord) * 8 + time * move) / 16;
	gl_FragColor += texture(diffuseTex0, (IN.texCoord) * 16 + time * move) / 32;
	gl_FragColor += texture(diffuseTex0, (IN.texCoord) * 32 + time * move) / 32;
	
	
	gl_FragColor.rgb = vec3(1.0f - pow(gl_FragColor.r, 0.75f) * 2.0f);
}