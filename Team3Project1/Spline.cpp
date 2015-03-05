

#include "Spline.h"


Spline::Spline(const T3Vector3& a, const T3Vector3& b, const T3Vector3& c, unsigned int subdivisions)
{
#if WINDOWS_BUILD
	type = GL_LINE_STRIP;
	segments = subdivisions;
	ctrlPoints = new T3Vector3[3];
	ctrlPoints[0] = a;
	ctrlPoints[1] = b;
	ctrlPoints[2] = c;
	numVertices = subdivisions + 1;
	vertices = new Vertex[numVertices];

	// Build spline
	for (unsigned int i = 0; i <= subdivisions; ++i) { // bezier spline has subdivisions segments
		float t = (float) i / (float) subdivisions;
		T3Vector3 tvec = T3Vector3(t*t, t, 1);
		T3Vector3 bezier;
		bezier.x = 0.5f * tvec.x - tvec.y + 0.5f * tvec.z;
		bezier.y = -1.0f * tvec.x + tvec.y + 0.5f * tvec.z;
		bezier.z = 0.5f * tvec.x;

		vertices[i].SetPosition(ctrlPoints[0] * bezier.x + 
								ctrlPoints[1] * bezier.y + 
								ctrlPoints[2] * bezier.z);
	}

	// Buffer data
	BufferData();

	glBindVertexArray(arrayObject); // makes our VAO object associated with the object name in arrayObject, the currently bound/active/operated on VAO
	
	glGenBuffers(1, &controlVBO);
	glBindBuffer(GL_ARRAY_BUFFER, controlVBO);
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(T3Vector3), ctrlPoints, GL_STATIC_DRAW);

	glBindVertexArray(0);
#endif
}


Spline::~Spline()
{
#if WINDOWS_BUILD
	glDeleteBuffers(1, &controlVBO);
	delete[] ctrlPoints;
#endif
}

void Spline::DrawControlPoints() {
#if WINDOWS_BUILD
	type = GL_POINTS;
	glPointSize(5.0f);
	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, controlVBO);
	glVertexAttribPointer(VertexAttributes::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Position
	glVertexAttribPointer(VertexAttributes::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3)); // Normal
	glVertexAttribPointer(VertexAttributes::COLOUR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 6)); // Color
	glVertexAttribPointer(VertexAttributes::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 10)); // TexCoord
	glVertexAttribPointer(VertexAttributes::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 12)); // Tangent

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glDrawArrays(type, 0, 3);

	glBindVertexArray(0); // unbind current VAO
#endif
}

void Spline::Draw() { // Method assumes that the Shader in use has been bound
#if WINDOWS_BUILD
	type = GL_LINE_STRIP;
	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
	
	glVertexAttribPointer(VertexAttributes::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Position
	glVertexAttribPointer(VertexAttributes::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3)); // Normal
	glVertexAttribPointer(VertexAttributes::COLOUR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 6)); // Color
	glVertexAttribPointer(VertexAttributes::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 10)); // TexCoord
	glVertexAttribPointer(VertexAttributes::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 12)); // Tangent

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glDrawArrays(type, 0, numVertices);

	glBindVertexArray(0); // unbind current VAO
#endif
}
