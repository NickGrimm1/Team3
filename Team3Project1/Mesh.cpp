#include "Mesh.h"
#include "../Main_PS3_PPU/Renderer.h"
#include "../Team3Project1/Renderer.h"

Mesh::Mesh()	
{
#if WINDOWS_BUILD
	glGenVertexArrays(1, &arrayObject);
#endif
#if PS3_BUILD
	for(int i = 0; i < VertexAttributes::MAX + 1; ++i)
		vertexOffsets[i] = 0;

	bumpTex = NULL;
	normTex = NULL;
#endif

	type = PrimitiveType::TRIANGLES;
	numVertices = 0;
	numIndices = 0;
	vertices = NULL;
	indices = NULL;

	transformCoords = true;


}

Mesh::~Mesh(void)	
{
#if WINDOWS_BUILD
	glDeleteVertexArrays(1, &arrayObject);	//Delete our VAO
	glDeleteBuffers(1, bufferObject);		//Delete our VBOs
	if (vertices)
		delete[]vertices;
	if (indices)
		delete[]indices;
#endif
}
#if WINDOWS_BUILD
void Mesh::Draw()	
{
	Renderer::count++;
	glBindVertexArray(arrayObject);
	if(numIndices > 0)
		glDrawElements(type, numIndices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(type, 0, numVertices);	//Draw the triangle!

	glBindVertexArray(0);
}
#endif
#if PS3_BUILD
void Mesh::Draw(Shader* shader)
{
	Renderer::count++;
	if(numVertices == 0)
		return;

	if(numVertices > 500)
	{
		bool a = true;
	}

	int shader_idx;

	shader_idx = shader->GetVertex()->GetAttributeIndex(VertexAttributes::POSITION);
	if (shader_idx > -1)
	{
		cellGcmSetVertexDataArray(shader_idx,
			0, 
			sizeof(Vertex), 
			3, 
			CELL_GCM_VERTEX_F, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertexOffsets[VertexAttributes::POSITION]
		);
	}

	if(vertexOffsets[VertexAttributes::NORMAL])
	{
		shader_idx = shader->GetVertex()->GetAttributeIndex(VertexAttributes::NORMAL);
		if (shader_idx > -1)
		{
			cellGcmSetVertexDataArray(shader_idx,
				0, 
				sizeof(Vertex), 
				3, 
				CELL_GCM_VERTEX_F, 
				CELL_GCM_LOCATION_LOCAL, 
				(uint32_t)vertexOffsets[VertexAttributes::NORMAL]
			);
		}
	}

	if(vertexOffsets[VertexAttributes::COLOUR])	
	{
		shader_idx = shader->GetVertex()->GetAttributeIndex(VertexAttributes::COLOUR);
		if (shader_idx > -1)
		{
			cellGcmSetVertexDataArray(shader_idx,
				0, 
				sizeof(Vertex), 
				4, 
				CELL_GCM_VERTEX_UB, 
				CELL_GCM_LOCATION_LOCAL, 
				(uint32_t)vertexOffsets[VertexAttributes::COLOUR]
			);
		}
	}

	if(vertexOffsets[VertexAttributes::TEXCOORD])
	{
		shader_idx = shader->GetVertex()->GetAttributeIndex(VertexAttributes::TEXCOORD);
		if (shader_idx > -1)
		{
			cellGcmSetVertexDataArray(shader_idx,
				0, 
				sizeof(Vertex), 
				2, 
				CELL_GCM_VERTEX_F, 
				CELL_GCM_LOCATION_LOCAL, 
				(uint32_t)vertexOffsets[VertexAttributes::TEXCOORD]
			);
		}
	}

	if(vertexOffsets[VertexAttributes::TANGENT])
	{
		shader_idx = shader->GetVertex()->GetAttributeIndex(VertexAttributes::TANGENT);
		if (shader_idx > -1)
		{
			cellGcmSetVertexDataArray(shader_idx,
				0, 
				sizeof(Vertex), 
				3, 
				CELL_GCM_VERTEX_F, 
				CELL_GCM_LOCATION_LOCAL, 
				(uint32_t)vertexOffsets[VertexAttributes::TANGENT]
			);
		}
	}

	if(vertexOffsets[VertexAttributes::MAX]) 
	{	//If we have indices

		cellGcmSetDrawIndexArray(type, numIndices, CELL_GCM_DRAW_INDEX_ARRAY_TYPE_16,
			CELL_GCM_LOCATION_LOCAL, vertexOffsets[VertexAttributes::MAX]);
	}
	else{
		//else just draw an ordered list of vertices
		//type = CELL_GCM_PRIMITIVE_TRIANGLES;
		//numVertices = 3;
		cellGcmSetDrawArrays(type, 0, numVertices);
	}
}
#endif

void	Mesh::BufferData()	
{
#if WINDOWS_BUILD
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices, GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
#endif
#if PS3_BUILD
	cellGcmAddressToOffset(&vertices->x, &vertexOffsets[VertexAttributes::POSITION]);
	cellGcmAddressToOffset(&vertices->nX, &vertexOffsets[VertexAttributes::NORMAL]);
	cellGcmAddressToOffset(&vertices->rgba, &vertexOffsets[VertexAttributes::COLOUR]);
	cellGcmAddressToOffset(&vertices->tX, &vertexOffsets[VertexAttributes::TEXCOORD]);
	cellGcmAddressToOffset(&vertices->tnX, &vertexOffsets[VertexAttributes::TANGENT]);
#endif
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

			T3Vector3 normal = T3Vector3::Cross((vertices[b].GetPosition() - vertices[a].GetPosition()), (vertices[c].GetPosition() - vertices[a].GetPosition())).Normal();

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
			T3Vector3 a = vertices[i].GetPosition();
			T3Vector3 b = vertices[i + 1].GetPosition();
			T3Vector3 c = vertices[i + 2].GetPosition();

			T3Vector3 normal = T3Vector3::Cross(b - a, c - a).Normal();

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

			vertices[a].SetTangent(tangent + vertices[a].GetTangent());
			vertices[b].SetTangent(tangent + vertices[b].GetTangent());
			vertices[c].SetTangent(tangent + vertices[c].GetTangent());
		}
	}
	else
	{
		for(unsigned int i = 0; i < numVertices; i += 3)
		{
			T3Vector3 tangent = GenerateTangent(vertices[i].GetPosition(), vertices[i + 1].GetPosition(), vertices[i + 2].GetPosition(), vertices[i].GetTexCoord(), vertices[i + 1].GetTexCoord(), vertices[i + 2].GetTexCoord());

			vertices[i].SetTangent(tangent + vertices[i].GetTangent());
			vertices[i + 1].SetTangent(tangent + vertices[i+1].GetTangent());
			vertices[i + 2].SetTangent(tangent + vertices[i+2].GetTangent());
		}
	}
	for(unsigned int i = 0; i < numVertices; ++i)
	{
		vertices[i].SetTangent(vertices[i].GetTangent().Normal());
	}
}

T3Vector3 Mesh::GenerateTangent(const T3Vector3 &a,const T3Vector3 &b,const T3Vector3 &c,const T3Vector2 &ta,const T3Vector2 &tb,const T3Vector2 &tc)	
{
	T3Vector2 coord1  = tb - ta;
	T3Vector2 coord2  = tc - ta;

	T3Vector3 vertex1 = b - a;
	T3Vector3 vertex2 = c - a;

	T3Vector3 axis = T3Vector3(vertex1 * coord2.y - vertex2 * coord1.y);

	float factor = 1.0f / (coord1.x * coord2.y - coord2.x * coord1.y);

	return axis * factor;
}

void Mesh::AssignVertexMemory()
{
#if WINDOWS_BUILD
	vertices = new Vertex[numVertices];
#endif
#if PS3_BUILD
	vertices = (Vertex*)GCMRenderer::localMemoryAlign(128, sizeof(Vertex) * numVertices);
#endif
}

void Mesh::AssignIndexMemory(unsigned int indexCount)
{
#if WINDOWS_BUILD
	indices = new unsigned int[indexCount];
#endif
#if PS3_BUILD
	indices = (short*)GCMRenderer::localMemoryAlign(128, sizeof(short) * (indexCount));
#endif
}

Mesh* Mesh::GenerateTriangle()	
{
	Mesh*m = new Mesh();
	m->numVertices = 3;

	m->AssignVertexMemory();

	m->vertices[0] = Vertex(
		T3Vector3(0.0f, 0.5f,	0.0f),
		T3Vector3(),
		T3Vector4(1.0f, 0.0f, 0.0f, 1.0f),
		T3Vector2(0.5f, 0.0f));
	m->vertices[1] = Vertex(
		T3Vector3(0.5f, -0.5f, 0.0f),
		T3Vector3(),
		T3Vector4(0.0f, 1.0f, 0.0f, 1.0f),
		T3Vector2(1.0f, 1.0f));
	m->vertices[2] = Vertex(
		T3Vector3(-0.5f, -0.5f, 0.0f),
		T3Vector3(),
		T3Vector4(0.0f, 0.0f, 1.0f, 1.0f),
		T3Vector2(0.0f, 1.0f));

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

	m->AssignVertexMemory();

	m->vertices[0] = Vertex(
		T3Vector3(-1.0f, -1.0f, 0.0f), // bottom left
		T3Vector3(0.0f, 0.0f,1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(0.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(		// bottom right
		T3Vector3(1.0f, -1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(1.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(		// top left
		T3Vector3(-1.0f,1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(0.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(		// top right
		T3Vector3(1.0f, 1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(1.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuadAlt()	
{
	Mesh* m = new Mesh();

	m->numVertices = 4;

	m->type = PrimitiveType::TRIANGLE_STRIP;

	m->AssignVertexMemory();

	m->vertices[0] = Vertex(
		T3Vector3(0.0f, 0.0f, 0.0f),
		T3Vector3(0.0f, 0.0f, 1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(0.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(
		T3Vector3(1.0f, 0.0f, 0.0f),
		T3Vector3(0.0f, 0.0f, 1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(1.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(
		T3Vector3(0.0f, 1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(0.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(
		T3Vector3(1.0f,  1.0f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(1.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateQuadCentral()	
{
	Mesh* m = new Mesh();

	m->numVertices = 4;
	m->AssignVertexMemory();

	m->type = PrimitiveType::TRIANGLE_STRIP;

	m->AssignVertexMemory();

	m->vertices[0] = Vertex(
		T3Vector3(-0.5f, -0.5f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(0.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[1] = Vertex(
		T3Vector3(-0.5f, 0.5f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(0.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[2] = Vertex(
		T3Vector3(0.5f, -0.5f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(1.0f, 0.0f),
		T3Vector3(1.0f, 0.0f,0.0f));
	m->vertices[3] = Vertex(
		T3Vector3(0.5f, 0.5f, 0.0f),
		T3Vector3(0.0f, 0.0f,-1.0f),
		T3Vector4(1.0f, 1.0f,1.0f,1.0f),
		T3Vector2(1.0f, 1.0f),
		T3Vector3(1.0f, 0.0f,0.0f));

	m->BufferData();

	return m;
}

Mesh* Mesh::GenerateCone(unsigned int subdivs)
{
	Mesh* m = new Mesh();
	
	m->numVertices = subdivs * 2; // Need subdiv copies of the tip vertex to maintain normals
	m->AssignVertexMemory();
	
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

		m->vertices[i].SetTexCoord(T3Vector2((float) i * tex_inc, 0));
		m->vertices[i + subdivs].SetTexCoord(T3Vector2((float) i * tex_inc, 1));
	}	
	
	m->AssignIndexMemory(subdivs * 3 + 3);

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
	m->AssignVertexMemory();

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
		m->vertices[i].SetTexCoord(T3Vector2(i * tex_res, 0.0f));
		m->vertices[i + subdivs + 1].SetTexCoord(T3Vector2(i * tex_res, 1.0f));
		alpha += subdiv_angle;
	}

	// Setup index buffer
	m->AssignIndexMemory(subdivs * 6);
	
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
	m->AssignVertexMemory();

	m->vertices[0].SetPosition(T3Vector3(0.0f, 0.0f, 0.0f)); // Circle centre at origin
	m->vertices[0].SetColor(T3Vector4(0, 0, 0, 1.0f));
	m->vertices[0].SetTexCoord(T3Vector2(0.0f, 0.0f)); // Texel 0,0 at circle centre
	m->vertices[0].SetNormal(T3Vector3(0,1,0));
	m->vertices[0].SetTangent(T3Vector3(1,0,0));

	float subdiv_angle = 2 * PI / subdivs;
	float alpha = 0.0f;
	for (unsigned int i = 1; i <= subdivs + 1; ++i)
	{ // length is subdivs + 2
		float s = sin(alpha);
		float c = cos(alpha);
		m->vertices[i].SetPosition(T3Vector3(s, 0, c));
		m->vertices[i].SetColor(T3Vector4(0, 0, 0, 1.0f));
		m->vertices[i].SetNormal(T3Vector3(0,1,0)); // straight up
		m->vertices[i].SetTexCoord(T3Vector2(s, c)); // Make sure texture is set to repeat to handle other quadrants
		m->vertices[i].SetTangent(T3Vector3(1,0,0));
		alpha += subdiv_angle;
	}

	m->BufferData();

	return m;
}
