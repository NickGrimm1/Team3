#if PS3_BUILD
#include "../Team3Project1/Mesh.h"
#include <sysutil/sysutil_sysparam.h>	
#include <cell/gcm.h>
#include "../Team3Project1/Vertex.h"

namespace PrimitiveType
{
	enum Type
	{
		POINTS = CELL_GCM_PRIMITIVE_POINTS,
		LINES = CELL_GCM_PRIMITIVE_LINES,
		LINE_LOOP = CELL_GCM_PRIMITIVE_LINE_LOOP,
		LINE_STRIP = CELL_GCM_PRIMITIVE_LINE_STRIP,
		TRIANGLES = CELL_GCM_PRIMITIVE_TRIANGLES,
		TRIANGLE_STRIP = CELL_GCM_PRIMITIVE_TRIANGLE_STRIP,
		TRIANGLE_FAN = CELL_GCM_PRIMITIVE_TRIANGLE_FAN,
		QUADS = CELL_GCM_PRIMITIVE_QUADS,
		QUAD_STRIP = CELL_GCM_PRIMITIVE_QUAD_STRIP,
	};
}

void Mesh::BufferData()
{
	cellGcmAddressToOffset(this->vertices->x, this->vertexOffsets[VertexAttributes::POSITION]);
	cellGcmAddressToOffset(this->vertices->nX, this->vertexOffsets[VertexAttributes::NORMAL]);
	cellGcmAddressToOffset(this->vertices->rgba, this->vertexOffsets[VertexAttributes::COLOUR]);
	cellGcmAddressToOffset(this->vertices->tX, this->vertexOffsets[VertexAttributes::TEXCOORD]);
	cellGcmAddressToOffset(this->vertices->tnX, this->vertexOffsets[VertexAttributes::TANGENT]);
	if (this->indices)
		cellGcmAddressToOffset(this->indices, this->vertexOffsets[VertexAttributes::MAX]);
}

void Mesh::Draw(ShaderPart &vertex, ShaderPart &fragment)
{
	cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VertexAttributes::POSITION),
		0, 
		sizeof(Vertex), 
		3, 
		CELL_GCM_VERTEX_F, 
		CELL_GCM_LOCATION_LOCAL, 
		(uint32_t)vertexOffsets[VertexAttributes::POSITION]
	);

	if(vertexOffsets[VertexAttributes::NORMAL])
	{
		cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VertexAttributes::NORMAL),
			0, 
			sizeof(Vertex), 
			2, 
			CELL_GCM_VERTEX_F, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertexOffsets[VertexAttributes::NORMAL]
		);
	}

	if(vertexOffsets[VertexAttributes::COLOUR])	
	{
		cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VertexAttributes::COLOUR),
			0, 
			sizeof(Vertex), 
			4, 
			CELL_GCM_VERTEX_UB, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertexOffsets[VertexAttributes::COLOUR]
		);
	}

	if(vertexOffsets[VertexAttributes::TEXCOORD])
	{
		cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VertexAttributes::TEXCOORD),
			0, 
			sizeof(Vertex), 
			2, 
			CELL_GCM_VERTEX_F, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertexOffsets[VertexAttributes::TEXCOORD]
		);
	}

	if(vertexOffsets[VertexAttributes::TANGENT])
	{
		cellGcmSetVertexDataArray(vertex.GetAttributeIndex(VertexAttributes::TANGENT),
			0, 
			sizeof(Vertex), 
			2, 
			CELL_GCM_VERTEX_F, 
			CELL_GCM_LOCATION_LOCAL, 
			(uint32_t)vertexOffsets[VertexAttributes::TANGENT]
		);
	}

	if(vertexOffsets[VertexAttributes::MAX]) 
	{	//If we have indices

		cellGcmSetDrawIndexArray(type, numIndices, CELL_GCM_DRAW_INDEX_ARRAY_TYPE_16,
			CELL_GCM_LOCATION_LOCAL, vertexOffsets[VertexAttributes::MAX]);
	}
	else{
		//else just draw an ordered list of vertices
		cellGcmSetDrawArrays(type, 0, numVertices);
	}
}

void Mesh::GetMemory(unsigned int vertices, unsigned int indices)
{
	this->vertices = (Vertex*)GCMRenderer::localMemoryAlign(128, sizeof(Vertex) * vertices);
	if (indices)
		this->indices = (short*)GCMRenderer::localMemoryAlign(128, sizeof(short) * indices);
}
#endif