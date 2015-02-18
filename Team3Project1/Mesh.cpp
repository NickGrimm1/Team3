#include "Mesh.h"

Mesh::Mesh()	
{
#if WINDOWS_BUILD
	glGenVertexArrays(1, &arrayObject);
	type = GL_TRIANGLES;
#endif
	texture = NULL;
	bumpTexture	= NULL;
	numVertices = 0;
	numIndices = 0;
	vertices = NULL;
	indices = NULL;

	transformCoords = true;
}

Mesh::~Mesh(void)	
{
#if WINDOWS_BUILD
	glDeleteVertexArrays(1, &arrayObject);			//Delete our VAO
	glDeleteBuffers(1, bufferObject);		//Delete our VBOs
#endif

	delete[]vertices;
	delete[]indices;
}

void Mesh::Draw(bool update)	
{
#if WINDOWS_BUILD
	if(update) 
	{
		if (texture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->GetTextureName());
		}
		if (bumpTexture)
		{
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, bumpTexture->GetTextureName());
		}
	}

	glBindVertexArray(arrayObject);
	if(numIndices > 0)
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(type, 0, numVertices);	//Draw the triangle!

	glBindVertexArray(0);
#endif
}

void	Mesh::BufferData()	
{
	glBindVertexArray(arrayObject);

	// Buffer Vertices
	glGenBuffers(1, &bufferObject[0]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);

	// Set Vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Position
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3))); // Normal
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3))); // Color
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3) + sizeof(Vector4))); // TexCoord
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3) + sizeof(Vector3) + sizeof(Vector4) + sizeof(Vector2))); // Tangent

	// Buffer Indices
	if(indices) 
	{
		glGenBuffers(1, &bufferObject[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void	Mesh::GenerateNormals()	
{
	if(indices) 
	{
		for(unsigned int i = 0; i < numIndices; i += 3)
		{
			int a = indices[i];
			int b = indices[i + 1];
			int c = indices[i + 2];

			Vector3 normal = Vector3::Cross((vertices[b].position - vertices[a].position), (vertices[c].position - vertices[a].position));

			vertices[a].normal = normal;
			vertices[b].normal = normal;
			vertices[c].normal = normal;
		}
	}
	else
	{
		//It's just a list of triangles, so generate face normals
		for(unsigned int i = 0; i < numVertices; i += 3)
		{
			Vector3 &a = vertices[i].position;
			Vector3 &b = vertices[i + 1].position;
			Vector3 &c = vertices[i + 2].position;

			Vector3 normal = Vector3::Cross(b - a, c - a);

			vertices[i].normal = normal;
			vertices[i + 1].normal = normal;
			vertices[i + 2].normal = normal;
		}
	}

	for(GLuint i = 0; i < numVertices; ++i)
	{
		vertices[i].normal.Normalise();
	}
}

void Mesh::GenerateTangents()
{
	if(indices) 
	{
		for(unsigned int i = 0; i < numIndices; i += 3)
		{
			int a = indices[i];
			int b = indices[i + 1];
			int c = indices[i + 2];

			Vector3 tangent = GenerateTangent(vertices[a].position, vertices[b].position, vertices[c].position, vertices[a].texCoord, vertices[b].texCoord, vertices[c].texCoord);

			vertices[a].tangent = tangent;
			vertices[b].tangent = tangent;
			vertices[c].tangent = tangent;
		}
	}
	else
	{
		for(unsigned int i = 0; i < numVertices; i += 3)
		{
			Vector3 tangent = GenerateTangent(vertices[i].position, vertices[i + 1].position, vertices[i + 2].position, vertices[i].texCoord, vertices[i + 1].texCoord, vertices[i + 2].texCoord);

			vertices[i].tangent = tangent;
			vertices[i + 1].tangent = tangent;
			vertices[i + 2].tangent = tangent;
		}
	}
	for(GLuint i = 0; i < numVertices; ++i)
	{
		vertices[i].tangent.Normalise();
	}
}

Vector3 Mesh::GenerateTangent(const Vector3 &a,const Vector3 &b,const Vector3 &c,const Vector2 &ta,const Vector2 &tb,const Vector2 &tc)	
{
	Vector2 coord1  = tb - ta;
	Vector2 coord2  = tc - ta;

	Vector3 vertex1 = b - a;
	Vector3 vertex2 = c - a;

	Vector3 axis = Vector3(vertex1 * coord2.y - vertex2 * coord1.y);

	float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return axis * factor;
}


Mesh* Mesh::GenerateTriangle()	
{
	Mesh*m = new Mesh();
	m->numVertices = 3;

	m->vertices = new Vertex[m->numVertices];
	m->vertices[0] = Vertex(
		Vector3(0.0f, 0.5f,	0.0f),
		Vector3(),
		Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		Vector2(0.5f, 0.0f));
	m->vertices[1] = Vertex(
		Vector3(0.5f, -0.5f, 0.0f),
		Vector3(),
		Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector2(1.0f, 1.0f));
	m->vertices[2] = Vertex(
		Vector3(-0.5f, -0.5f, 0.0f),
		Vector3(),
		Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		Vector2(0.0f, 1.0f));

	m->GenerateNormals();
	m->GenerateTangents();
	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuad()	
{
	Mesh* m = new Mesh();

	m->numVertices = 4;

#if WINDOWS_BUILD
	m->type = GL_TRIANGLE_STRIP;
#endif

	m->vertices	= new Vertex[m->numVertices];

	m->vertices[0] = Vertex(
		Vector3(-1.0f, -1.0f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 1.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(
		Vector3(-1.0f,1.0f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 0.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(
		Vector3(1.0f, -1.0f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 1.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(
		Vector3(1.0f, 1.0f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 0.0f),
		Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuadAlt()	
{
	Mesh* m = new Mesh();

	m->numVertices = 4;

#if WINDOWS_BUILD
	m->type = GL_TRIANGLE_STRIP;
#endif

	m->vertices	= new Vertex[m->numVertices];

	m->vertices[0] = Vertex(
		Vector3(0.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 0.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(
		Vector3(0.0f, 1.0f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 1.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(
		Vector3(1.0f, 0.0f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 0.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(
		Vector3(1.0f,  1.0f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 1.0f),
		Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuadCentral()	
{
	Mesh* m = new Mesh();

	m->numVertices = 4;

#if WINDOWS_BUILD
	m->type = GL_TRIANGLE_STRIP;
#endif

	m->vertices	= new Vertex[m->numVertices];

	m->vertices[0] = Vertex(
		Vector3(-0.5f, -0.5f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 0.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(
		Vector3(-0.5f, 0.5f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 1.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(
		Vector3(0.5f, -0.5f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 0.0f),
		Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(
		Vector3(0.5f, 0.5f, 0.0f),
		Vector3(0.0f, 0.0f,-1.0f),
		Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 1.0f),
		Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateCone(unsigned int subdivs)
{
	Mesh* m = new Mesh();
	
	m->numVertices = subdivs * 2; // Need subdiv copies of the tip vertex to maintain normals

	m->vertices = new Vertex[m->numVertices];
	
	float subdiv_angle = 2 * PI / subdivs;
	float tex_inc = 1.0f / (float) subdivs;
	float alpha = 0.0f;
	for (unsigned int i = 0; i < subdivs; ++i) 
	{ // First set of vertices is the bottom circle
		float s = sin(alpha);
		float c = cos(alpha);
		m->vertices[i].position = Vector3(s, 1, c);
		m->vertices[i].color = Vector4(1, 1, 1, 1.0f);
		alpha += subdiv_angle;
	
		m->vertices[i + subdivs].position = Vector3(0, 0, 0);
		m->vertices[i + subdivs].color = Vector4(1, 1, 1, 1);

		m->vertices[i].texCoord = Vector2((float) i * tex_inc, 0);
		m->vertices[i + subdivs].texCoord = Vector2((float) i * tex_inc, 1);
	}	
	
	m->indices = new unsigned int[subdivs * 3 + 3];
	for (unsigned int i = 0; i < subdivs; ++i) 
	{
		// triangle 1 - base 0, base 1, top - counter clockwise order
		m->indices[m->numIndices++] = i;
		m->indices[m->numIndices++] = i + 1;
		m->indices[m->numIndices++] = i + subdivs;
	}
	
	m->indices[m->numIndices++] = subdivs - 1;
	m->indices[m->numIndices++] = 0;
	m->indices[m->numIndices++] = subdivs;

	m->GenerateNormals();
	m->GenerateTangents();

	m->BufferData(); // copy to graphics memory
	return m;
}

Mesh* Mesh::GenerateCylinder(unsigned int subdivs) 
{
	Mesh* m = new Mesh();

	m->numVertices = (subdivs + 1) * 2;
	m->vertices = new Vertex[m->numVertices];

	float subdiv_angle = 2 * PI / subdivs;
	float alpha = 0.0f;
	GLfloat tex_res = 1.0f / subdivs;
	for (unsigned int i = 0; i < subdivs + 1; ++i) {
		float s = sin(alpha);
		float c = cos(alpha);
		m->vertices[i].position = Vector3(s, 0, c);
		m->vertices[i + subdivs + 1].position = Vector3(s, 1.0f, c);
		m->vertices[i].color = Vector4(0, 0, 0, 1.0f);
		m->vertices[i + subdivs + 1].color = Vector4(0, 0, 0, 1.0f);
		m->vertices[i].normal = Vector3(s, 0, c);
		m->vertices[i + subdivs + 1].normal = Vector3(s, 0, c);
		m->vertices[i].normal.Normalise();
		m->vertices[i + subdivs + 1].normal.Normalise();
		m->vertices[i].tangent = Vector3(c, 0, s);
		m->vertices[i + subdivs + 1].tangent = Vector3(c, 0, s);
		m->vertices[i].tangent.Normalise();
		m->vertices[i + subdivs + 1].tangent.Normalise();
		m->vertices[i].texCoord = Vector2(i * tex_res, 0.0f);
		m->vertices[i + subdivs + 1].texCoord = Vector2(i * tex_res, 1.0f);
		alpha += subdiv_angle;
	}

	// Setup index buffer
	m->indices = new unsigned int[subdivs * 6];
	m->numIndices = 0;
	for (unsigned int i = 0; i < subdivs; ++i) {
		// triangle 1 - base 0, base 1, top 0 - counter clockwise order
		m->indices[m->numIndices++] = i;
		m->indices[m->numIndices++] = i + 1;
		m->indices[m->numIndices++] = i + subdivs + 1;
		// triangle 2 - top 0, base 1, top 1
		m->indices[m->numIndices++] = i + subdivs + 1;
		m->indices[m->numIndices++] = i + 1;
		m->indices[m->numIndices++] = i + subdivs + 2;
	}

	m->BufferData(); // copy to graphics memory

	return m;
}

Mesh* Mesh::GenerateCircle(unsigned int subdivs) 
{
	// Going to draw a unit circle at origin in xz plane
	
	Mesh* m = new Mesh();
	m->type = GL_TRIANGLE_FAN;
	m->numVertices = subdivs + 2; // Additional vertex at circle centre + repeat first vertex on circumference to close off circle

	m->vertices = new Vertex[m->numVertices];

	m->vertices[0].position = Vector3(0.0f, 0.0f, 0.0f); // Circle centre at origin
	m->vertices[0].color = Vector4(0, 0, 0, 1.0f);
	m->vertices[0].texCoord = Vector2(0.0f, 0.0f); // Texel 0,0 at circle centre
	m->vertices[0].normal = Vector3(0,1,0);

	float subdiv_angle = 2 * PI / subdivs;
	float alpha = 0.0f;
	for (unsigned int i = 1; i <= subdivs + 1; ++i)
	{ // length is subdivs + 2
		float s = sin(alpha);
		float c = cos(alpha);
		m->vertices[i].position = Vector3(s, 0, c);
		m->vertices[i].color = Vector4(0, 0, 0, 1.0f);
		m->vertices[i].normal = Vector3(0,1,0); // straight up
		m->vertices[i].texCoord = Vector2(s, c); // Make sure texture is set to repeat to handle other quadrants
		alpha += subdiv_angle;
	}

	m->GenerateTangents();

	m->BufferData(); // copy to graphics memory
	return m;
}