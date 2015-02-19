/******************************************************************************
Class:TextMesh
Implements:Mesh
Author:Rich Davison
Description:A simple extension to the Mesh class, which will build up a mesh
that represents a string of text, rendered using a texture containing a 
bitmap font. Once the TextMesh has been generated, it can be used to render
a line of text on screen - either for multiple frames by keeping its pointer
like any other mesh, or by simply creating and deleting TextMeshes as text
is required.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   This is nyantext!!
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "Font.h"
#include "Mesh.h"
#include <vector>

class TextMesh : public Mesh {
public:
	TextMesh(const std::string &text, const Font &font);
	virtual ~TextMesh(void) {}
protected:
	const Font& font;
};

