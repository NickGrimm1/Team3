#include "Spline.h"


Spline::Spline(const Vector3& a, const Vector3& b, const Vector3& c, unsigned int subdivisions)
{
	type = GL_LINE_STRIP;
	segments = subdivisions;
	ctrlPoints = new Vector3[3];
	ctrlPoints[0] = a;
	ctrlPoints[1] = b;
	ctrlPoints[2] = c;
	numVertices = subdivisions + 1;
	vertices = new Vertex[numVertices];

	// Build spline
	for (unsigned int i = 0; i <= subdivisions; ++i) { // bezier spline has subdivisions segments
		float t = (float) i / (float) subdivisions;
		Vector3 tvec = Vector3(t*t, t, 1);
		Vector3 bezier;
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
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(Vector3), ctrlPoints, GL_STATIC_DRAW);

	glBindVertexArray(0);
}


Spline::~Spline()
{
	glDeleteBuffers(1, &controlVBO);
	delete[] ctrlPoints;
}

void Spline::DrawControlPoints() {
	type = GL_POINTS;
	glPointSize(5.0f);
	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, controlVBO);
	glVertexAttribPointer(VertexAttributes::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0); //Specifies that in the currently bound array index VERTEX_BUFFER points to 3 Floats. 
	glEnableVertexAttribArray(VertexAttributes::POSITION); // Enables VERTEX BUFFER to be an index in the VAO?
	glDrawArrays(type, 0, 3);

	glBindVertexArray(0); // unbind current VAO
}

void Spline::Draw() { // Method assumes that the Shader in use has been bound
	type = GL_LINE_STRIP;
	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
	glVertexAttribPointer(VertexAttributes::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0); //Specifies that in the currently bound array index VERTEX_BUFFER points to 3 Floats. 
	glEnableVertexAttribArray(VertexAttributes::POSITION); // Enables VERTEX BUFFER to be an index in the VAO?
	glDrawArrays(type, 0, numVertices);

	glBindVertexArray(0); // unbind current VAO
}