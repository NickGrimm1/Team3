#include "Mesh.h"

Mesh::Mesh()	
{
#if WINDOWS_BUILD
	glGenVertexArrays(1, &arrayObject);
#endif
#if PS3_BUILD
	for(int i = 0; i < VertexAttributes::MAX + 1; ++i)
		vertexOffsets[i] = 0;
#endif

	type = PrimitiveType::TRIANGLES;
	numVertices = 0;
	numIndices = 0;
	vertices = NULL;
	indices = NULL;

	transformCoords = true;
}
#if WINDOWS_BUILD
Mesh::~Mesh(void)	
{
	glDeleteVertexArrays(1, &arrayObject);			//Delete our VAO
	glDeleteBuffers(1, bufferObject);		//Delete our VBOs
	if (vertices)
		delete[]vertices;
	if (indices)
		delete[]indices;
}

void Mesh::Draw(bool update)	
{
	glBindVertexArray(arrayObject);
	if(numIndices > 0)
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(type, 0, numVertices);	//Draw the triangle!

	glBindVertexArray(0);
}

void	Mesh::BufferData()	
{
	glBindVertexArray(arrayObject);

	// Buffer Vertices
	glGenBuffers(1, &bufferObject[0]);
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject[0]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	// Set Vertex attributes
	glVertexAttribPointer(VertexAttributes::POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0); // Position
	glVertexAttribPointer(VertexAttributes::NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3)); // Normal
	glVertexAttribPointer(VertexAttributes::COLOUR, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 6)); // Color
	glVertexAttribPointer(VertexAttributes::TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 10)); // TexCoord
	glVertexAttribPointer(VertexAttributes::TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 12)); // Tangent

	// Buffer Indices
	if(indices) 
	{
		glGenBuffers(1, &bufferObject[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(short), indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

void Mesh::GetMemory(unsigned int vertices, unsigned int indices)
{
	this->vertices = new Vertex[numVertices];
	if (indices)
		this->indices = new short[numIndices];
}
#endif

void	Mesh::GenerateNormals()	
{
	if(indices) 
	{
		for(unsigned int i = 0; i < numIndices; i += 3)
		{
			int a = indices[i];
			int b = indices[i + 1];
			int c = indices[i + 2];

			T3Vector3 normal = T3Vector3::Cross((vertices[b].GetPosition() - vertices[a].GetPosition()), (vertices[c].GetPosition() - vertices[a].GetPosition()));

			vertices[a].SetNormal(normal);
			vertices[b].SetNormal(normal);
			vertices[c].SetNormal(normal);
		}
	}
	else
	{
		//It's just a list of triangles, so generate face normals
		for(unsigned int i = 0; i < numVertices; i += 3)
		{
			T3Vector3 &a = vertices[i].GetPosition();
			T3Vector3 &b = vertices[i + 1].GetPosition();
			T3Vector3 &c = vertices[i + 2].GetPosition();

			T3Vector3 normal = T3Vector3::Cross(b - a, c - a);

			vertices[i].SetNormal(normal);
			vertices[i + 1].SetNormal(normal);
			vertices[i + 2].SetNormal(normal);
		}
	}

	for(unsigned int i = 0; i < numVertices; ++i)
	{
		vertices[i].GetNormal().Normalise();
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

			T3Vector3 tangent = GenerateTangent(vertices[a].GetPosition(), vertices[b].GetPosition(), vertices[c].GetPosition(), vertices[a].GetTexCoord(), vertices[b].GetTexCoord(), vertices[c].GetTexCoord());

			vertices[a].SetTangent(tangent);
			vertices[b].SetTangent(tangent);
			vertices[c].SetTangent(tangent);
		}
	}
	else
	{
		for(unsigned int i = 0; i < numVertices; i += 3)
		{
			T3Vector3 tangent = GenerateTangent(vertices[i].GetPosition(), vertices[i + 1].GetPosition(), vertices[i + 2].GetPosition(), vertices[i].GetTexCoord(), vertices[i + 1].GetTexCoord(), vertices[i + 2].GetTexCoord());

			vertices[i].SetTangent(tangent);
			vertices[i + 1].SetTangent(tangent);
			vertices[i + 2].SetTangent(tangent);
		}
	}
	for(unsigned int i = 0; i < numVertices; ++i)
	{
		vertices[i].SetTangent(vertices[i].GetTangent().Normal());
	}
}

T3Vector3 Mesh::GenerateTangent(const T3Vector3 &a,const T3Vector3 &b,const T3Vector3 &c,const Vector2 &ta,const Vector2 &tb,const Vector2 &tc)	
{
	Vector2 coord1  = tb - ta;
	Vector2 coord2  = tc - ta;

	T3Vector3 vertex1 = b - a;
	T3Vector3 vertex2 = c - a;

	T3Vector3 axis = T3Vector3(vertex1 * coord2.y - vertex2 * coord1.y);

	float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return axis * factor;
}


Mesh* Mesh::GenerateTriangle()	
{
	Mesh*m = new Mesh();
	m->numVertices = 3;

	m->GetMemory(m->numVertices, 0);

	m->vertices[0] = Vertex(
		T3Vector3(0.0f, 0.5f,	0.0f),
		T3Vector3(),
		T3Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		Vector2(0.5f, 0.0f));
	m->vertices[1] = Vertex(
		T3Vector3(0.5f, -0.5f, 0.0f),
		T3Vector3(),
		T3Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		Vector2(1.0f, 1.0f));
	m->vertices[2] = Vertex(
		T3Vector3(-0.5f, -0.5f, 0.0f),
		T3Vector3(),
		T3Vector4(0.0f, 0.0f, 1.0f, 1.0f),
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

	m->type = PrimitiveType::TRIANGLE_STRIP;

	m->GetMemory(m->numVertices, 0);

	m->vertices[0] = Vertex(
		T3Vector3(-1.0f, -1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(
		T3Vector3(1.0f, -1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(
		T3Vector3(-1.0f,1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(
		T3Vector3(1.0f, 1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuadAlt()	
{
	Mesh* m = new Mesh();

	m->numVertices = 4;

	m->type = PrimitiveType::TRIANGLE_STRIP;

	m->GetMemory(m->numVertices, 0);

	m->vertices[0] = Vertex(
		T3Vector3(0.0f, 0.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(
		T3Vector3(0.0f, 1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(
		T3Vector3(1.0f, 0.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(
		T3Vector3(1.0f,  1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuadCentral()	
{
	Mesh* m = new Mesh();

	m->numVertices = 4;

	m->type = PrimitiveType::TRIANGLE_STRIP;

	m->GetMemory(m->numVertices, 0);

	m->vertices[0] = Vertex(
		T3Vector3(-0.5f, -0.5f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(
		T3Vector3(-0.5f, 0.5f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(0.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(
		T3Vector3(0.5f, -0.5f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(
		T3Vector3(0.5f, 0.5f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		Vector2(1.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateCone(unsigned int subdivs)
{
	Mesh* m = new Mesh();
	
	m->numVertices = subdivs * 2; // Need subdiv copies of the tip vertex to maintain normals

	m->GetMemory(m->numVertices, subdivs * 3 + 3);
	
	float subdiv_angle = 2 * PI / subdivs;
	float tex_inc = 1.0f / (float) subdivs;
	float alpha = 0.0f;
	for (unsigned int i = 0; i < subdivs; ++i) 
	{ // First set of vertices is the bottom circle
		float s = sin(alpha);
		float c = cos(alpha);
		m->vertices[i].SetPosition(T3Vector3(s, 1, c));
		m->vertices[i].SetColor(T3Vector4(1, 1, 1, 1.0f));
		alpha += subdiv_angle;
	
		m->vertices[i + subdivs].SetPosition(T3Vector3(0, 0, 0));
		m->vertices[i + subdivs].SetColor(T3Vector4(1, 1, 1, 1));

		m->vertices[i].SetTexCoord(Vector2((float) i * tex_inc, 0));
		m->vertices[i + subdivs].SetTexCoord(Vector2((float) i * tex_inc, 1));
	}	

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


	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateCylinder(unsigned int subdivs) 
{
	Mesh* m = new Mesh();

	m->numVertices = (subdivs + 1) * 2;
	m->GetMemory(m->numVertices, subdivs * 6);

	float subdiv_angle = 2 * PI / subdivs;
	float alpha = 0.0f;
	float tex_res = 1.0f / subdivs;
	for (unsigned int i = 0; i < subdivs + 1; ++i) {
		float s = sin(alpha);
		float c = cos(alpha);
		m->vertices[i].SetPosition(T3Vector3(s, 0, c));
		m->vertices[i + subdivs + 1].SetPosition(T3Vector3(s, 1.0f, c));
		m->vertices[i].SetColor(T3Vector4(0, 0, 0, 1.0f));
		m->vertices[i + subdivs + 1].SetColor(T3Vector4(0, 0, 0, 1.0f));
		m->vertices[i].SetNormal(T3Vector3(s, 0, c).Normal());
		m->vertices[i + subdivs + 1].SetNormal(T3Vector3(s, 0, c).Normal());
		m->vertices[i].SetTangent(T3Vector3(c, 0, s).Normal());
		m->vertices[i + subdivs + 1].SetTangent(T3Vector3(c, 0, s).Normal());
		m->vertices[i].SetTexCoord(Vector2(i * tex_res, 0.0f));
		m->vertices[i + subdivs + 1].SetTexCoord(Vector2(i * tex_res, 1.0f));
		alpha += subdiv_angle;
	}
	
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

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateCircle(unsigned int subdivs) 
{
	// Going to draw a unit circle at origin in xz plane
	
	Mesh* m = new Mesh();

	m->type = PrimitiveType::TRIANGLE_FAN;

	m->numVertices = subdivs + 2; // Additional vertex at circle centre + repeat first vertex on circumference to close off circle

	m->GetMemory(m->numVertices, 0);

	m->vertices[0].SetPosition(T3Vector3(0.0f, 0.0f, 0.0f)); // Circle centre at origin
	m->vertices[0].SetColor(T3Vector4(0, 0, 0, 1.0f));
	m->vertices[0].SetTexCoord(Vector2(0.0f, 0.0f)); // Texel 0,0 at circle centre
	m->vertices[0].SetNormal(T3Vector3(0,1,0));

	float subdiv_angle = 2 * PI / subdivs;
	float alpha = 0.0f;
	for (unsigned int i = 1; i <= subdivs + 1; ++i)
	{ // length is subdivs + 2
		float s = sin(alpha);
		float c = cos(alpha);
		m->vertices[i].SetPosition(T3Vector3(s, 0, c));
		m->vertices[i].SetColor(T3Vector4(0, 0, 0, 1.0f));
		m->vertices[i].SetNormal(T3Vector3(0,1,0)); // straight up
		m->vertices[i].SetTexCoord(Vector2(s, c)); // Make sure texture is set to repeat to handle other quadrants
		alpha += subdiv_angle;
	}

	m->GenerateTangents();

	m->BufferData();

	return m;
}