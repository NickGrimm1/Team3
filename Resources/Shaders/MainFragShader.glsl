#version 430 core

const unsigned int MAX_SHADOWS = 10; // subject to change

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform int useDiffuseTex;
uniform int useNormalTex;	// Enables use of the normal map texture for calculation
uniform sampler2DShadow shadowTex[MAX_SHADOWS];

// TODO - some of this data isn't needed for this pass - light colour at least
uniform	vec4 lightColour[MAX_SHADOWS];
uniform	vec4 lightSpecColour[MAX_SHADOWS];
uniform vec3 lightPos[MAX_SHADOWS];
uniform vec3 lightDir[MAX_SHADOWS];
uniform	float lightRadius[MAX_SHADOWS];
uniform float lightAngle[MAX_SHADOWS];
uniform int lightType[MAX_SHADOWS];

uniform vec3 cameraPos;

uniform int numShadows; // The total number of lights in the scene

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	vec4 shadowProj[MAX_SHADOWS];
} IN;

out vec4 gl_FragColor[2];

/* Spot lights - emits a cone of light angle of alpha
A fragment will be lit if
1. Within spotlight radius (not beyond max distance)
2. within cone of light
dot(a,b) = |a| * |b| * cos angle
calc dot product between cones central axis and vector from cone origin and fragment (normalised)
if greater than cos alpha, then within cone.
*/

// PI is not defined in GLSL
const float PI = 3.14159265358979323846264;
bool inSpotlight(unsigned int i) {
	vec3 fragvec = normalize(IN.worldPos - lightPos[i]);
	float dotprod = dot(fragvec, normalize(lightDir[i]));
	float coscone = cos(lightAngle[i] / 2 * PI / 180.0); //cos expects radians
	return (coscone <= dotprod);
}

void main(void)	{

	gl_FragColor[0] = IN.colour;
	// Sample the colour texture
	if (useDiffuseTex > 0) {
		vec4 diffuse = texture2D(diffuseTex, IN.texCoord);
		if (diffuse.a < 0.0001) discard; // transparent fragment - ignore
		gl_FragColor[0] = diffuse;
	}

	// Setup our surface normal depending on whether a normal map has been supplied or not
	vec3 normal;
	if (useNormalTex > 0) {
		mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);
		normal = normalize(TBN * (texture2D(normalTex, IN.texCoord).rgb * 2.0 - 1.0)); // convert to vec3 space (-1 -> 1)
	}
	else normal = IN.normal;

	// Process shadow maps
	float shadow = 0.0; // start at zero, shadowed, if depth text indicates its not in shadow in one of the lights, will be 1.0 or greater
	bool shadowData = false;

	for (int i = 0; i < numShadows; i++) {

		if (lightType[i] == 2 && !inSpotlight(i)) continue; // if not in spotlight, no need to calculate for this light
		shadowData = true;
		if (IN.shadowProj[i].w > 0.0) {
			shadow += textureProj(shadowTex[i], IN.shadowProj[i]); // samples based on a different clip space - if in shadow returns 0, otherwise 1
		}
	}

	if (shadowData)
		// passing shadow data in the w coord of the normal through to combine shader.
		gl_FragColor[1] = vec4(normal.xyz * 0.5 + 0.5, shadow / numShadows);  // [-0.5,0.5] -> [0.0, 1] tex coords
	else
		gl_FragColor[1] = vec4(normal.xyz * 0.5 + 0.5, 1);  // [-0.5,0.5] -> [0.0, 1] tex coords
}