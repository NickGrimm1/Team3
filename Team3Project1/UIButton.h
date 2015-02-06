/**
<summary>Represents a basic UI button on screen

Author: Nick Grimm
Version: 0.0.5 07/02/2015.</summary>
*/
#pragma once
#include "ClickableEntity2D.h"
#include "DrawableTexture2D.h"
#include "DrawableText2D.h"
#include "SelectableEntity2D.h"
#include <vector>

using namespace std;

class UIButton : public T3Rectangle, public ClickableEntity2D, public SelectableEntity2D
{
public:
	UIButton(float x, float y, float width, float height, void (*actionOnSelect)(), void (*actionOnClick)(float, float), DrawableTexture2D* texture, DrawableText2D* text)
		: T3Rectangle(x, y, width, height), SelectableEntity2D(actionOnSelect), ClickableEntity2D(actionOnClick), texture(texture), text(text)
	{ }
	
	DrawableTexture2D* GetTexture();
	void SetTexture(DrawableTexture2D* value);
	DrawableText2D* GetText();
	void SetText(DrawableText2D* value);
	vector<DrawableEntity2D*> GetDrawables();
private:
	DrawableTexture2D* texture;
	DrawableText2D* text;
};