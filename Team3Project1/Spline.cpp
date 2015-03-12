

#include "Spline.h"


Spline::Spline(const T3Vector3& a, const T3Vector3& b, const T3Vector3& c, unsigned int subdivisions)
{

	type = PrimitiveType::LINE_STRIP;
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
}


Spline::~Spline()
{

	delete[] ctrlPoints;

}
#if WINDOWS_BUILD
void Spline::Draw() { // Method assumes that the Shader in use has been bound

	type = PrimitiveType::LINE_STRIP;

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


}
#endif

#if PS3_BUILD
void Spline::Draw(Shader* s)
{
	type = PrimitiveType::LINE_STRIP;
	cellGcmAddressToOffset(&vertices->x, &vertexOffsets[VertexAttributes::POSITION]);
	cellGcmAddressToOffset(&vertices->nX, &vertexOffsets[VertexAttributes::NORMAL]);
	cellGcmAddressToOffset(&vertices->rgba, &vertexOffsets[VertexAttributes::COLOUR]);
	cellGcmAddressToOffset(&vertices->tX, &vertexOffsets[VertexAttributes::TEXCOORD]);
	cellGcmAddressToOffset(&vertices->tnX, &vertexOffsets[VertexAttributes::TANGENT]);
	short* temp = indices;
	indices = NULL;
	Mesh::Draw(s);
	indices = temp;
}
#endif