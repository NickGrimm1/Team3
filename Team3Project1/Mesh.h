#pragma once
#if WINDOWS_BUILD
#include "../Framework/OGLRenderer.h"
#endif
#include "Vertex.h"
#include "Texture.h"

class Mesh
{
public:
	friend class MD5Mesh;
	Mesh();
	virtual ~Mesh();

	//Sets the Mesh's diffuse map. Takes an OpenGL texture 'name'
	void SetTexture(Texture* tex)	{texture = tex;}
	//Gets the Mesh's diffuse map. Returns an OpenGL texture 'name'
	Texture* GetTexture()	{return texture;}
	//Sets the Mesh's bump map. Takes an OpenGL texture 'name'
	void SetBumpMap(Texture* tex)	{bumpTexture = tex;}
	//Gets the Mesh's bump map. Returns an OpenGL texture 'name'
	Texture* GetBumpMap()	{return bumpTexture;}
	void SetColour(Vector4* colour) 
	{
		for (unsigned int i = 0; i < numVertices; i++)
			vertices[i].color = colour[i];
	} 
	bool TransformsTexCoords() { return transformCoords;}

	virtual void Draw(bool update = true);

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

	int GetNumVertices() {return NumVertices;}
	Vector3* GetVertices() {return Vertices; }

protected:
	//Buffers all VBO data into graphics memory. Required before drawing!
	void BufferData();
	//Generates normals for all facets. Assumes geometry type is GL_TRIANGLES...
	void GenerateNormals();
	//Generates tangents for all facets. Assumes geometry type is GL_TRIANGLES...
	void GenerateTangents();
	//Helper function for GenerateTangents
	Vector3 GenerateTangent(const Vector3 &a,const Vector3 &b,const Vector3 &c,const Vector2 &ta,const Vector2 &tb,const Vector2 &tc);
	
	//Pointer to vertex data
	Vertex*		vertices;
	//Pointer to vertex position attribute data (badly named...?)
	Vector3*	Vertices;
	//Pointer to vertex indices attribute data
	unsigned int*	indices;
	//Number of vertices for this mesh
	unsigned int numVertices;
	//Number of indices for this mesh
	unsigned int numIndices;

	//Number of vertices for this mesh
	GLuint	NumVertices;
	bool transformCoords;
	// OpenGL texture name for the diffuse map
	Texture* texture;
	//OpenGL texture name for the bump map
	Texture* bumpTexture;

#if WINDOWS_BUILD
	//VAO for this mesh
	GLuint arrayObject;
	//VBOs for this mesh
	GLuint bufferObject[2];
	//Primitive type for this mesh (GL_TRIANGLES...etc)
	GLuint type;
#endif
	
};