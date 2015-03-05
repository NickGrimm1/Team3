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

class UIButton : public ClickableEntity2D
{
public:

	UIButton(float x, float y, float width, float height, void (GameScreen2D::*actionOnSelect)(void), void (GameScreen2D::*actionOnClick)(float, float), 
		DrawableTexture2D* texture, DrawableText2D* text, void (GameScreen2D::*actionOnUnselect)(), GameScreen2D* arg)
		: ClickableEntity2D(x, y, width, height, actionOnSelect, actionOnClick, actionOnUnselect, arg) {
		drawables.push_back(texture);
		drawables.push_back(text);
	}

	DrawableTexture2D* GetTexture() { return (DrawableTexture2D*)drawables.at(0); }
	void SetTexture(DrawableTexture2D* value) { drawables.at(0) = value; }

	DrawableText2D* GetText() { return (DrawableText2D*)drawables.at(1); }
	void SetText(DrawableText2D* value) { drawables.at(1) = value; }

};