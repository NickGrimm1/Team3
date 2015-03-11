

#include "TrackSegment.h"


TrackSegment::TrackSegment(const T3Vector3& a, const T3Vector3& b, const T3Vector3& c, unsigned int subdivisions, float trackWidth) :
	Spline(a, b, c, subdivisions)
{
#if WINDOWS_BUILD
	// Now spline is built, generate mesh around it
	trackMesh = new Vertex[numVertices * 2]; // left boundary then right boundary
	T3Vector3 right;
	T3Vector3 up = T3Vector3(0,1,0);
	for (unsigned int i = 0; i < subdivisions; i++) {
		T3Vector3 direction = vertices[i+1].GetPosition() - vertices[i].GetPosition();
		direction.Normalise();
		right = T3Vector3::Cross(direction, up);
		right.Normalise();
		trackMesh[i].SetPosition(vertices[i].GetPosition() - right * trackWidth);
		trackMesh[i + subdivisions + 1].SetPosition(vertices[i].GetPosition() + right * trackWidth);
		trackMesh[i].SetTexCoord(T3Vector2(0, (float) i));
		trackMesh[i + subdivisions + 1].SetTexCoord(T3Vector2(1.0f, (float) i));
		trackMesh[i].SetNormal(up);
		trackMesh[i + subdivisions + 1].SetNormal(up);
	}
	// Set last vertices
	trackMesh[subdivisions].SetPosition(vertices[subdivisions].GetPosition() - right * trackWidth);
	trackMesh[(2 * numVertices) - 1].SetPosition(vertices[subdivisions].GetPosition() + right * trackWidth);
	trackMesh[subdivisions].SetTexCoord(T3Vector2(0, (float) subdivisions));
	trackMesh[(2 * numVertices) - 1].SetTexCoord(T3Vector2(1, (float) subdivisions));
	trackMesh[subdivisions].SetNormal(up);
	trackMesh[(2 * numVertices) - 1].SetNormal(up);

	// Calc midpoints







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
	glBufferData(GL_ARRAY_BUFFER, 2 * numVertices * sizeof(Vertex), trackMesh, GL_STATIC_DRAW);

	glGenBuffers(1, &bufferObject[1]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
#endif
}


TrackSegment::~TrackSegment(void)
{	
#if WINDOWS_BUILD
	// Mesh destructor should handle deletion of indices/texture coords
	glDeleteBuffers(1, &trackVBO);
	delete[] trackMesh;
#endif
}

void TrackSegment::Draw() {
#if WINDOWS_BUILD
	type = GL_TRIANGLES;

	glBindVertexArray(arrayObject); // Make mesh VAO currently bound object
	glBindBuffer(GL_ARRAY_BUFFER, trackVBO);

	// Set Vertex attributes
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

	glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0); 
	
	glBindVertexArray(0); // unbind current VAO
#endif
}

void TrackSegment::DrawSpline() {

	Spline::Draw();

}

T3Vector3 TrackSegment::GetTrackCentreLeft() const {

	if (segments % 2 == 0) {
		return trackMesh[segments / 2].GetPosition();
	}
	else {
		// Get midway point between central segment
		unsigned int pos = segments / 2;
		return (trackMesh[pos].GetPosition() + trackMesh[pos + 1].GetPosition()) / 2.0f;
	}

}

T3Vector3 TrackSegment::GetTrackCentreRight() const {
	
	if (segments % 2 == 0) {
		return trackMesh[segments + (segments / 2) + 1].GetPosition();
	}
	else {
		// Get midway point between central segment
		unsigned int pos = segments + 1 + (segments / 2);
		return (trackMesh[pos].GetPosition() + trackMesh[pos + 1].GetPosition()) / 2.0f;
	}
	

}
