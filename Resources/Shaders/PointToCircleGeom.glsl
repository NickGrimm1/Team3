#version 150
 
uniform float particleSize;
uniform float circleSegments;

layout(points) in; // There is only one point
layout(triangle_strip, max_vertices = 100) out;


in Vertex {
	vec4 colour;
} IN[];

out Vertex {
	vec4 colour;
	vec2 texCoord;
} OUT;

// PI is not defined in GLSL
const float PI = 3.14159265358979323846264;

void main() {	

	float particleRadius = particleSize / 2.0;

	for(int i = 0; i < gl_in.length(); ++i) {

		OUT.colour = IN[i].colour;

		float subdiv_angle = 2 * PI / circleSegments;
		float alpha = 0.0f;

		for (int j = 0; j <= circleSegments; ++j) { // length is subdivs + 2
			gl_Position = gl_in[i].gl_Position; // Emit circle centre as vertex
			OUT.texCoord = vec2(0,0);
			EmitVertex();

			float s = 0.5f * sin(alpha + subdiv_angle); // do alpha + subdiv first to maintain CCW order
			float c = 0.5f * cos(alpha + subdiv_angle);

			gl_Position.x = gl_in[i].gl_Position.x + s * particleRadius;
			gl_Position.y = gl_in[i].gl_Position.y + c * particleRadius;
			OUT.texCoord = vec2(s, c);
			EmitVertex();
						
			s = 0.5f * sin(alpha);
			c = 0.5f * cos(alpha);
			gl_Position.x = gl_in[i].gl_Position.x + s * particleRadius;
			gl_Position.y = gl_in[i].gl_Position.y + c * particleRadius;
			OUT.texCoord = vec2(s, c); // Make sure texture is set to repeat to handle other quadrants
			EmitVertex();

			alpha += subdiv_angle;
		}
		EndPrimitive();
	}
}