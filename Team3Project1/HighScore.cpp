#include "HighScore.h"
#include "GameStateManager.h"
#include <sstream>

HighScore::HighScore(unsigned int score)
{
	name = "AAA";
	currentPos = 0;
	highScore = score;
	lastDown = timer.GetMS();
	lastUp = timer.GetMS();
	lastLeft = timer.GetMS();
	lastRight = timer.GetMS();
}


HighScore::~HighScore(void)
{
	
}

void HighScore::LoadContent() {
	scoreboard = new Scoreboard();
	Font* font = GameStateManager::Assets()->LoadFont(this, TEXTUREDIR"quadrats.tga", 16, 16);
	Texture* scoreBoardTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"score_board.png", 0);
	DrawableTexture2D* scoreBoardBG = new DrawableTexture2D(0.2f, 0.1f, 5, 0.6f, 0.8f, scoreBoardTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);
	AddDrawable(scoreBoardBG);

	ostringstream convert;
	convert << highScore;
	string score = convert.str();

	AddDrawable(new DrawableText2D(0.25f, 0.2f, 6, 0.5f, 0.1f, "NEW HIGH SCORE!!!", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1)));
	AddDrawable(new DrawableText2D(0.3f, 0.35f, 6, 0.4f, 0.1f, score, font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1)));
	AddDrawable(new DrawableText2D(0.3f, 0.7f, 7, 0.4f, 0.1f, "SUBMIT", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1)));

	for (unsigned int i = 0; i < 3; i++) {
		nameEntities[i] = new DrawableText2D(0.35f + (i * 0.1f), 0.5f, 7, 0.1f, 0.1f, "A", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1));
		AddDrawable(nameEntities[i]);
		smallCursor[i] = new DrawableText2D(0.35f + (i * 0.1f), 0.55f, 6, 0.1f, 0.1f, "_", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,0,1,1));
	}
	
	bigCursor = new DrawableText2D(0.3f, 0.75f, 6, 0.45f, 0.1f, "_", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,0,1,1));

	AddDrawable(smallCursor[0]);
	cursor = smallCursor[0];

}

void HighScore::Update() {
	if (currentPos < 3 && cursor != smallCursor[currentPos]) {
			RemoveDrawable(cursor);
			AddDrawable(smallCursor[currentPos]);
			cursor = smallCursor[currentPos];
	}
	else if (currentPos == 3 && cursor != bigCursor) {
			RemoveDrawable(cursor);
			AddDrawable(bigCursor);
			cursor = bigCursor;
	}
}

void HighScore::UnloadContent() {
	delete scoreboard;
}

bool HighScore::SubmitScore() {
#if WINDOWS_BUILD
	return false;
#endif

#ifdef PS3_BUILD
	return false;
#endif
}

#if WINDOWS_BUILD
void HighScore::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {

	if (type == KeyboardEvents::KEY_PRESS && key == KeyboardEvents::KEYBOARD_RETURN) {
		if (currentPos < 3) {
			currentPos++;
		}
		else {
			// Submit score
			SubmitScore();
		}
	}

	if (type == KeyboardEvents::KEY_PRESS && key >= KeyboardEvents::KEYBOARD_1 && key <= KeyboardEvents::KEYBOARD_Z && currentPos < 3) {
		name[currentPos] = char(key);
		ostringstream convert;
		convert << name[currentPos];
		string letter = convert.str();
		nameEntities[currentPos]->SetText(letter);
		currentPos++;
	}
}

#endif

void HighScore::GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {
	if (type == GamepadEvents::BUTTON_PRESS && button == GamepadEvents::INPUT_CROSS_A) { // same action as return on keyboard
		if (currentPos < 3) {
			currentPos++;
		}
		else {
			// Submit score
			SubmitScore();
		}
	}
}

void HighScore::GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {
	if (analogueControl == GamepadEvents::AnalogueControl::LEFT_STICK) {
		// Vertical movement if absolute value if vertical displacement and greater than horizontal displacement
		if (abs(amount.y) > abs(amount.x)) {
			if (amount.y > 0.5f && timer.GetMS() > lastUp + 0.25f) { // really make user displace stick, prevent letter changing every frame
				name[currentPos] = (name[currentPos] + 1);
				if (name[currentPos] > 'Z') { // change current letter on vertical movement
					name[currentPos] = '1';
				}
				lastUp = timer.GetMS();
			}
			else if (amount.y < -0.5f && timer.GetMS() > lastDown + 0.25f) {
				name[currentPos]--;
				if (name[currentPos] < '1') {
					name[currentPos] = 'Z';
				}
				lastDown = timer.GetMS();
			}
		}
		else {// Horizontal displacement - change letter position
			if (amount.x > 0.5f && timer.GetMS() > lastRight + 0.25f) {
				currentPos = (currentPos + 1) % 4;
				lastRight = timer.GetMS();
			}
			else if (amount.x < 0.5f && timer.GetMS() > lastLeft + 0.25f) {
				currentPos--;
				if (currentPos < 0) currentPos = 3;
				lastLeft = timer.GetMS();
			}
		}
	}
}
