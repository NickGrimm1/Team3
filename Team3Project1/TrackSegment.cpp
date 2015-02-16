#include "TrackSegment.h"


TrackSegment::TrackSegment(const Vector3& a, const Vector3& b, const Vector3& c, unsigned int subdivisions, float trackWidth) :
	Spline(a, b, c, subdivisions)
{
	// Now spline is built, generate mesh around it
	leftBoundary = new Vector3[numVertices];
	rightBoundary = new Vector3[numVertices];
	trackMesh = new Vector3[numVertices * 2]; // left boundary then right boundary
	textureCoords = new Vector2[numVertices * 2];
	Vector3 right;
	for (unsigned int i = 0; i < subdivisions; i++) {
		Vector3 direction = vertices[i+1] - vertices[i];
		direction.Normalise();
		Vector3 up = Vector3(0,0,1);
		right = Vector3::Cross(direction, up);
		right.Normalise();
		trackMesh[i] = vertices[i] - right * trackWidth;
		trackMesh[i + subdivisions + 1] = vertices[i] + right * trackWidth;
		leftBoundary[i] = trackMesh[i];
		rightBoundary[i] = trackMesh[i + subdivisions + 1];
		textureCoords[i] = Vector2(0, (float) i);
		textureCoords[i + subdivisions + 1] = Vector2(1.0f, (float) i);
	}
	// Set last vertices
	trackMesh[subdivisions] = vertices[subdivisions] - right * trackWidth;
	trackMesh[(2 * numVertices) - 1] = vertices[subdivisions] + right * trackWidth;
	leftBoundary[subdivisions] = trackMesh[subdivisions];
	rightBoundary[subdivisions] = trackMesh[(2 * numVertices) - 1];
	textureCoords[subdivisions] = Vector2(0, (float) subdivisions);
	textureCoords[(2 * numVertices) - 1] = Vector2(1, (float) subdivisions);

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
/*
	for (int i = 0; i < numVertices * 2; i++) {
		cout << "Vertex " << i << " = " << trackMesh[i];
	}

	unsigned int count = 0;
	for (int i = 0; i < numIndices; i = i + 3) {
		cout << "Triangle " << count++ << " = " << indices[i] << ", " << indices[i+1] << ", " << indices[i+2] << endl;
	}
*/

	// Set up VBOs for the track boundaries
	glBindVertexArray(arrayObject); // makes our VAO object associated with the object name in arrayObject, the currently bound/active/operated on VAO
	
	// buffer track boundaries
	glGenBuffers(1, &leftVBO);
	glBindBuffer(GL_ARRAY_BUFFER, leftVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), leftBoundary, GL_STATIC_DRAW);

	glGenBuffers(1, &rightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rightVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(Vector3), rightBoundary, GL_STATIC_DRAW);

	// Now buffer trackMesh data
	glGenBuffers(1, &trackVBO);
	glBindBuffer(GL_ARRAY_BUFFER, trackVBO);
	glBufferData(GL_ARRAY_BUFFER, 2 * numVertices * sizeof(Vector3), trackMesh, GL_STATIC_DRAW);

	// buffer texture data
	glGenBuffers(1, &bufferObject[TEXTURE_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[TEXTURE_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, 2 * numVertices * sizeof(Vector2), textureCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(TEXTURE_BUFFER, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(TEXTURE_BUFFER);

	glGenBuffers(1, &bufferObject[INDEX_BUFFER]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}


TrackSegment::~TrackSegment(void)
{	
	// Mesh destructor should handle deletion of indices/texture coords
	glDeleteBuffers(1, &leftVBO);
	glDeleteBuffers(1, &rightVBO);
	glDeleteBuffers(1, &trackVBO);
	delete[] leftBoundary;
	delete[] rightBoundary;
	delete[] trackMesh;
}

void TrackSegment::DrawTrackBoundaries() {
	type = GL_LINE_STRIP;

	// Draw left boundary
	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, leftVBO);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); //Specifies that in the currently bound array index VERTEX_BUFFER points to 3 Floats. 
	glEnableVertexAttribArray(VERTEX_BUFFER); // Enables VERTEX BUFFER to be an index in the VAO?
	glDrawArrays(type, 0, numVertices);
	
	// Draw right boundary
	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, rightVBO);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); //Specifies that in the currently bound array index VERTEX_BUFFER points to 3 Floats. 
	glEnableVertexAttribArray(VERTEX_BUFFER); // Enables VERTEX BUFFER to be an index in the VAO?
	glDrawArrays(type, 0, numVertices);
	
	glBindVertexArray(0); // unbind current VAO
}

void TrackSegment::Draw() {
	type = GL_TRIANGLES;

	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, trackVBO);
	glVertexAttribPointer(VERTEX_BUFFER, 3, GL_FLOAT, GL_FALSE, 0, 0); //Specifies that in the currently bound array index VERTEX_BUFFER points to 3 Floats. 
	glEnableVertexAttribArray(VERTEX_BUFFER); // Enables VERTEX BUFFER to be an index in the VAO?
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bumpTexture);

	glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0); 
	
	glBindVertexArray(0); // unbind current VAO
}

void TrackSegment::DrawSpline() {
	Spline::Draw();
}