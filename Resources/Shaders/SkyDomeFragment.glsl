#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D skyTex;
uniform int clipBelow;
uniform float clipHeight;

in Vertex 
{
	vec2 texCoord;
	vec4 worldPos;
} IN;

out vec4 gl_FragColor;

void main (void)
{
	bool clip = false;
	if (clipHeight > 0)
	{
		if (clipBelow == 0)
		{
			clip = true;
			gl_FragColor = vec4(0, 0, 0, 0);
		}
	}

	if (!clip)
	{
		vec4 topColor = texture2D(skyTex, IN.texCoord);
			//vec4(0.3f, 0.3f, 0.8f, 1);
		vec4 bottomColor = vec4(0, 0, 0, 1);

		vec4 baseColor = mix(bottomColor, topColor, clamp(IN.worldPos.y / 0.4f, 0.0f, 1.0f));
		vec4 cloudValue = texture(diffuseTex, IN.texCoord);
		
		gl_FragColor = mix(baseColor, vec4(1, 1, 1, 1), cloudValue);
	}
}