#include "TrackSegment.h"


TrackSegment::TrackSegment(const Vector3& a, const Vector3& b, const Vector3& c, unsigned int subdivisions, float trackWidth) :
	Spline(a, b, c, subdivisions)
{
	// Now spline is built, generate mesh around it
	trackMesh = new Vertex[numVertices * 2]; // left boundary then right boundary
	Vector3 right;
	for (unsigned int i = 0; i < subdivisions; i++) {
		Vector3 direction = vertices[i+1].GetPosition() - vertices[i].GetPosition();
		direction.Normalise();
		Vector3 up = Vector3(0,0,1);
		right = Vector3::Cross(direction, up);
		right.Normalise();
		trackMesh[i].SetPosition(vertices[i].GetPosition() - right * trackWidth);
		trackMesh[i + subdivisions + 1].SetPosition(vertices[i].GetPosition() + right * trackWidth);
		trackMesh[i].SetTexCoord(Vector2(0, (float) i));
		trackMesh[i + subdivisions + 1].SetTexCoord(Vector2(1.0f, (float) i));
	}
	// Set last vertices
	trackMesh[subdivisions].SetPosition(vertices[subdivisions].GetPosition() - right * trackWidth);
	trackMesh[(2 * numVertices) - 1].SetPosition(vertices[subdivisions].GetPosition() + right * trackWidth);
	trackMesh[subdivisions].SetTexCoord(Vector2(0, (float) subdivisions));
	trackMesh[(2 * numVertices) - 1].SetTexCoord(Vector2(1, (float) subdivisions));

	// Build index VBO
	numIndices = subdivisions * 6;
	indices = new unsigned int[numIndices];
	unsigned int index = 0;
	for (unsigned int i = 0; i < subdivisions; i++) {
		indices[index++] = i;
		indices[index++] = i + numVertices;
		indices[index++] = i + 1;
		
		indices[index++] = i + numVertices;
		indices[index++] = i + numVertices + 1;
		indices[index++] = i + 1;
	}

	// Set up VBOs for the track mesh
	glBindVertexArray(arrayObject); // makes our VAO object associated with the object name in arrayObject, the currently bound/active/operated on VAO
	glGenBuffers(1, &trackVBO);
	glBindBuffer(GL_ARRAY_BUFFER, trackVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * numVertices * sizeof(Vector3), trackMesh, GL_STATIC_DRAW);

	glGenBuffers(1, &bufferObject[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}


TrackSegment::~TrackSegment(void)
{	
	// Mesh destructor should handle deletion of indices/texture coords
	glDeleteBuffers(1, &trackVBO);
	delete[] trackMesh;
}

void TrackSegment::Draw() {
	type = GL_TRIANGLES;

	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, trackVBO);
	glVertexAttribPointer(VertexAttributes::POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0); //Specifies that in the currently bound array index VERTEX_BUFFER points to 3 Floats. 
	glEnableVertexAttribArray(VertexAttributes::POSITION); // Enables VERTEX BUFFER to be an index in the VAO?
	//glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(TEXTURE_BUFFER);

	glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0); 
	
	glBindVertexArray(0); // unbind current VAO
}

void TrackSegment::DrawSpline() {
	Spline::Draw();
}