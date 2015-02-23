#pragma once
#if WINDOWS_BUILD
#include "../Framework/OGLRenderer.h"
#endif
#if PS3_BUILD
#include <sysutil/sysutil_sysparam.h>	
#include <cell/gcm.h>
#endif
#include "Vertex.h"
#include "Texture.h"


namespace PrimitiveType
{
	enum Type
	{
#if WINDOWS_BUILD
		POINTS = GL_POINTS,
		LINES = GL_LINES,
		LINE_LOOP = GL_LINE_LOOP,
		LINE_STRIP = GL_LINE_STRIP,
		TRIANGLES = GL_TRIANGLES,
		TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
		TRIANGLE_FAN = GL_TRIANGLE_FAN,
		QUADS = GL_QUADS,
		QUAD_STRIP = GL_QUAD_STRIP,
#endif
#if PS3_BUILD
		POINTS = CELL_GCM_PRIMITIVE_POINTS,
		LINES = CELL_GCM_PRIMITIVE_LINES,
		LINE_LOOP = CELL_GCM_PRIMITIVE_LINE_LOOP,
		LINE_STRIP = CELL_GCM_PRIMITIVE_LINE_STRIP,
		TRIANGLES = CELL_GCM_PRIMITIVE_TRIANGLES,
		TRIANGLE_STRIP = CELL_GCM_PRIMITIVE_TRIANGLE_STRIP,
		TRIANGLE_FAN = CELL_GCM_PRIMITIVE_TRIANGLE_FAN,
		QUADS = CELL_GCM_PRIMITIVE_QUADS,
		QUAD_STRIP = CELL_GCM_PRIMITIVE_QUAD_STRIP,
#endif
	};
}

class Mesh
{
public:
	friend class MD5Mesh;
	Mesh();
	virtual ~Mesh();

	void SetColour(Vector4* colour) 
	{
		for (unsigned int i = 0; i < numVertices; i++)
			vertices[i].SetColor(colour[i]);
			
	} 
	bool TransformsTexCoords() { return transformCoords;}

#if WINDOWS_BUILD
	virtual void Draw();
#endif
#if PS3_BUILD
	virtual void Mesh::Draw(ShaderPart &vertex, ShaderPart &fragment);
#endif

	//Generates a single triangle, with RGB colours
	static Mesh* GenerateTriangle();
	//Generates a single white quad, going from -1 to 1 on the x and y axis.
	static Mesh* GenerateQuad();
	//Generates a single white quad, going from 0 to 1 on the x and y axis.
	static Mesh* GenerateQuadAlt();
	//Generates a single white quad, going from -0.5 to 0.5 on the x and y axis.
	static Mesh* Mesh::GenerateQuadCentral();
	//Generates a coloured quad, going from -1 to 1 on the x and z axis, with adjustable texture coords.
	static Mesh* GenerateQuadTexCoordCol(Vector2 scale, Vector2 texCoord, Vector4 colour); //NX 01/11/2012
	// Generates Circle/Cone/Cylinder meshes with subdivs segments to approximate curvature
	static Mesh* GenerateCircle(unsigned int subdivs);
	static Mesh* GenerateCone(unsigned int subdivs);
	static Mesh* GenerateCylinder(unsigned int subdivs);

protected:
	//Generates normals for all facets. Assumes geometry type is GL_TRIANGLES...
	void GenerateNormals();
	//Generates tangents for all facets. Assumes geometry type is GL_TRIANGLES...
	void GenerateTangents();
	//Helper function for GenerateTangents
	Vector3 GenerateTangent(const Vector3 &a,const Vector3 &b,const Vector3 &c,const Vector2 &ta,const Vector2 &tb,const Vector2 &tc);
	
	// Pointer to vertex data
	Vertex*	vertices;
	// Number of vertices for this mesh
	unsigned int numVertices;
	// Number of indices for this mesh
	unsigned int numIndices;
	bool transformCoords;

	// Primitive type for this mesh (GL_TRIANGLES...etc)
	unsigned int type;
	
#if WINDOWS_BUILD
	// Pointer to vertex indices attribute data
	unsigned int* indices;
	//Buffers all VBO data into graphics memory. Required before drawing!
	void BufferData();
	//VAO for this mesh
	GLuint arrayObject;
	//VBOs for this mesh
	GLuint bufferObject[2];
#endif
#if PS3_BUILD
	// Offsets for each of the vertex attributes
	unsigned int vertexOffsets[VertexAttributes::MAX];
	// Pointer to vertex indices attribute data
	short* indices;
#endif
};