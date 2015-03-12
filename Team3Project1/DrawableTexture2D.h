/**
Version History:
0.0.5 05/02/2015
0.0.6 06/02/2015
	Added Type.
0.0.7 12/02/2015
	Actually implemented getters & setters.
*/

/**
<summary>Represents a drawable image.

Author: Nick Grimm
Version: 0.0.7 12/02/2015.</summary>
*/
#pragma once
#include "DrawableEntity2D.h"
#include "Texture.h"

class DrawableTexture2D : public DrawableEntity2D
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x position of the entity.</param>
	<param name='y'>The y position of the entity.</param>
	<param name='depth>The depth of the entity on screen.</param>
	<param name='width'>The width of the entity.</param>
	<param name='height'>The height of the entity.</param>
	<param name='texture'>The texture.</param>
	<param name='rotation'>The rotation in degrees from up (clockwise). Default is 0.</param>
	<param name='origin'>The origin around which this object will rotate, relative to the object's draw area. Default is the center (0.5, 0.5).</param>
	<param name='blendColor'>The colour. Default is White (1,1,1,1).</param>	
	*/
	DrawableTexture2D(float x, float y, int depth, float width, float height, Texture* texture, float rotation = 0, T3Vector2 origin = T3Vector2(0.5f, 0.5f), T3Vector4 blendColor = T3Vector4(1.0f, 1.0f, 1.0f ,1.0f), bool transparent = true)
		: DrawableEntity2D(x, y, depth, width, height, DrawableType::Texture, rotation, origin, blendColor, transparent), 
		texture(texture)
	{ }

	/**
	<summary>Gets the texture.</summary>
	*/
	Texture* GetTexture() const { return texture; }
	/**
	<summary>Sets the texture.</summary>
	<param name='value'>The texture.</param>
	*/
	void SetTexture(Texture* value) { texture = value; }

	friend std::ostream& operator<<(std::ostream& o, const DrawableTexture2D& v)
	{
		//DrawableEntity2D::operator<<(o, v);
		o << "DrawableTexture2D:" << std::endl;
		o << "  texture: " << v.texture << std::endl;
		return o;
	}
protected:
	Texture* texture;
};