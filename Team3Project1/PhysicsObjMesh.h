#pragma once

/**
<summary>PhysicsObjMesh
Saves a copy of the vertices list for use in GJK

Author: Derek Kelly
Version: 0.0.1 10/03/2015</summary>
*/

#include "../Framework/OBJMesh.h"

class PhysicsObjMesh : public OBJMesh {
public:
	PhysicsObjMesh(std::string filename) : OBJMesh() {
		LoadOBJMesh(filename); 

		vertexCount = inputVertices.size();
		physicsVertices = new Vertex[vertexCount];
		for (unsigned int i = 0; i < vertexCount; i++) {
			physicsVertices[i].SetPosition(inputVertices[i]);
		}

		
		BuildOBJMesh();
	};

	virtual ~PhysicsObjMesh() {
		delete[] physicsVertices;
	}

	virtual Vertex* GetVertices() const {return physicsVertices;}
	virtual unsigned int GetNumVertices() const {return vertexCount;}

protected:
	Vertex* physicsVertices;
	unsigned int vertexCount;
};