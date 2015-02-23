#version 150 core

uniform sampler2D depthTex;
uniform sampler2D normTex;

uniform vec2 pixelSize;
uniform vec3 cameraPos;

uniform vec3 lightPos;
uniform vec3 lightDir;
uniform vec4 lightColour;
uniform vec4 lightSpecColour;
uniform float lightRadius;
uniform float lightAngle;
uniform int lightType;

in mat4 inverseProjView;
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
bool inSpotlight(vec3 world_pos) {
	vec3 fragvec = normalize(world_pos - lightPos);
	float dotprod = dot(fragvec, normalize(lightDir));
	float coscone = cos(lightAngle / 2 * PI / 180.0); //cos expects radians
	return (coscone <= dotprod);
}

void main() {
	vec3 pos = vec3((gl_FragCoord.x * pixelSize.x), (gl_FragCoord.y * pixelSize.y), 0.0);
	pos.z = texture(depthTex , pos.xy).r;

	vec3 normal = normalize(texture(normTex , pos.xy).xyz * 2.0 - 1.0);

	vec4 clip = inverseProjView * vec4(pos * 2.0 - 1.0, 1.0);
	pos = clip.xyz / clip.w;


	float dist = length(lightPos - pos);
	float atten = 1.0;
	if (lightType != 1) { // no attenuation of directional lights
		atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
	}
	
	if (lightType == 1) {
		if (dot(normal, lightDir) > 0) discard;
	}

	if (atten == 0.0 || (lightType == 2 && !inSpotlight(pos))) {
		discard;
	}

	vec3 incident;
	if (lightType == 1) {
		incident = -lightDir;
	}
	else {
		incident = normalize(lightPos - pos);
	}

	vec3 viewDir = normalize(cameraPos - pos);
	vec3 halfDir = normalize(incident + viewDir);

	float lambert = clamp(dot(incident, normal), 0.0, 1.0);
	float rFactor = clamp(dot(halfDir, normal), 0.0, 1.0);
	float sFactor = pow(rFactor, 33.0 );
	
	gl_FragColor[0] = vec4(lightColour.xyz * lambert * atten, 1.0);
	gl_FragColor[1] = vec4(lightSpecColour.xyz * sFactor * atten * 0.33, 1.0);
}