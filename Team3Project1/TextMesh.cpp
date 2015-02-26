#include "TextMesh.h"

/*
Our really basic TextMesh is going to make one big line of quads, each
containing a character of text, generated from the passed in string, and
the font struct.

Each character of text will be kept in a quad 1 unit big one each axis, 
and each quad is right next to its neighbours, making it easy to work out
how large a textmesh will be when scaled etc
*/
TextMesh::TextMesh(const std::string &text, const Font &font) :  font(font)	{

	//Each quad will be 4 points, drawn using a triangle strip
	//just like the GenerateQuad function from early on!
	type = PrimitiveType::TRIANGLE_STRIP;

	//Each character has 4 vertices...
	numVertices = text.length()*4;

	vertices = new Vertex[numVertices];

	unsigned int xCount = font.GetXCount();
	unsigned int yCount = font.GetYCount();

	//Now to work out how much of the texture each character
	//of the font takes up. Remember, texture coordinates
	//are 'normalised', so a bitmap font texture of 16 by 16
	//will have characters that are 1 / 16.0 in size etc...
	float texelWidth  = 1.0f / (float) xCount;
	float texelHeight = 1.0f / (float) yCount;

	/*
	Now to generate the vertex attributes for each character
	of text. Vertex position is 'easy' - it's just i to i+1
	on the width, and 0 to -1 on the height. It's -1 so that
	if we draw orthographic text at a screen position of
	(0,0) (top LEFT of the screen intuitively), the text
	'hangs down' from the top left, and is visible.
	*/
	for(unsigned int i = 0; i < text.length(); ++i) {
		unsigned int c = (unsigned int) text[i];

		//We can use the modulus operator to wrap around the 
		//position of the character, using the cell counts
		//of the font! so if we have a cell count of 16
		//by 16, character 0 will be at the top left, 
		//character 15 will be at the far right, and 16
		//will be on the left, down a row, and so on.

		float x = (float) (c % xCount);
		float y = (float) ((c / xCount) % yCount);

		//Now we can simply use our worked out font character sizes
		//to generate the correct texture coordinates for each glyph...

		vertices[(i*4)  ] = Vertex(
			T3Vector3((float) i, 0, 0),
			T3Vector3(0,0,0),
			T3Vector4(1,1,1,1),
			T3Vector2(x*texelWidth, (y)*texelHeight),
			T3Vector3(0,0,0));

		vertices[(i*4)+1] = Vertex(
			T3Vector3((float) i, -1, 0),
			T3Vector3(0,0,0),
			T3Vector4(1,1,1,1),
			T3Vector2(x*texelWidth, (y+1) * texelHeight),
			T3Vector3(0,0,0));
	
		vertices[(i*4)+2] = Vertex(
			T3Vector3((float) i+1, 0,0),
			T3Vector3(0,0,0),
			T3Vector4(1,1,1,1),
			T3Vector2((x+1)*texelWidth , (y)*texelHeight),
			T3Vector3(0,0,0));
		
		vertices[(i*4)+3] = Vertex(
			T3Vector3((float) i+1, -1, 0), 
			T3Vector3(0,0,0),
			T3Vector4(1,1,1,1),
			T3Vector2((x+1)*texelWidth , (y+1) * texelHeight),
			T3Vector3(0,0,0));
	}

	//Lastly, we buffer the data, just like a 'normal' mesh!
#if WINDOWS_BUILD
	BufferData();
#endif
}