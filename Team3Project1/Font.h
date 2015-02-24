/**
<summary>Defines a font asset.

Author: Nick Grimm
Version: 0.0.1 05/02/2015.</summary>
*/
#pragma once
#include "Texture.h"

class Font
{
public:
	Font(unsigned int xCount, unsigned int yCount, Texture* texure = NULL) 
		: texture(texture), xCount(xCount), yCount(yCount)
	{ }
	~Font() { }
	Texture* GetTexture() { return texture; }
	void SetTexture(Texture* value) { texture = value; }
	unsigned int GetXCount() const { return xCount; }
	unsigned int GetYCount() const { return yCount; }
private:
	Texture* texture;
	unsigned int xCount;
	unsigned int yCount;
};