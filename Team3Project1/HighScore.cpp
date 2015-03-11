#include "HighScore.h"
#include "GameStateManager.h"
#include <sstream>

HighScore::HighScore(unsigned int score)
{
	name = "AAA";
	currentPos = 0;
	highScore = score;
	lastDown = (float) timer.GetMS();
	lastUp = (float) timer.GetMS();
	lastLeft = (float) timer.GetMS();
	lastRight = (float) timer.GetMS();
}


HighScore::~HighScore(void)
{
	
}

void HighScore::LoadContent() {
#if WINDOWS_BUILD
	scoreboard = new Scoreboard();
#endif
	Font* font = GameStateManager::Assets()->LoadFont(this, "quadrats", 16, 16);
	Texture* scoreBoardTex = GameStateManager::Assets()->LoadTexture(this, "score_board", 0);
	DrawableTexture2D* scoreBoardBG = new DrawableTexture2D(0.2f, 0.1f, 5, 0.6f, 0.8f, scoreBoardTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);
	AddDrawable(scoreBoardBG);

	ostringstream convert;
	convert << highScore;
	string score = convert.str();

	AddDrawable(new DrawableText2D(0.25f, 0.2f, 6, 0.5f, 0.1f, "NEW HIGH SCORE!!!", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false));
	AddDrawable(new DrawableText2D(0.3f, 0.35f, 6, 0.4f, 0.1f, score, font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false));
	AddDrawable(new DrawableText2D(0.3f, 0.7f, 7, 0.4f, 0.1f, "SUBMIT", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false));

	for (unsigned int i = 0; i < 3; i++) {
		nameEntities[i] = new DrawableText2D(0.35f + (i * 0.1f), 0.5f, 7, 0.1f, 0.1f, "A", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);
		AddDrawable(nameEntities[i]);
		smallCursor[i] = new DrawableText2D(0.35f + (i * 0.1f), 0.55f, 6, 0.1f, 0.1f, "_", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);
	}
	
	bigCursor = new DrawableText2D(0.3f, 0.75f, 6, 0.45f, 0.1f, "_", font, 0, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);

	AddDrawable(smallCursor[0]);
	cursor = smallCursor[0];

}

void HighScore::Update() {
	if (currentPos < 3 && cursor != smallCursor[currentPos]) {
			RemoveDrawable(cursor, false);
			AddDrawable(smallCursor[currentPos]);
			cursor = smallCursor[currentPos];
	}
	else if (currentPos == 3 && cursor != bigCursor) {
			RemoveDrawable(cursor, false);
			AddDrawable(bigCursor);
			cursor = bigCursor;
	}
}

void HighScore::UnloadContent() {
#if WINDOWS_BUILD
	delete scoreboard;
#endif
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
			cout << "key pressed" << endl;
			currentPos++;
		}
		else {
			// Submit score
			SubmitScore();
		}
	}

	if (type == GamepadEvents::BUTTON_PRESS && button == GamepadEvents::INPUT_START) { // submit whereeversame
		SubmitScore();
}

	if (type == GamepadEvents::BUTTON_PRESS && button == GamepadEvents::INPUT_CIRCLE_B) { // moveback
		if (currentPos > 0) {
			currentPos--;
		}
	}
}

void HighScore::GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {
	if (analogueControl == GamepadEvents::LEFT_STICK) {
		// Vertical movement if absolute value if vertical displacement and greater than horizontal displacement
		if (abs(amount.y) > abs(amount.x)) {
			if (amount.y > 0.8f && timer.GetMS() > lastUp + 250) { // really make user displace stick, prevent letter changing every frame
				name[currentPos] = name[currentPos] + 1;
				if (name[currentPos] > 'Z') { // change current letter on vertical movement
					name[currentPos] = '0';
				}
				else if (name[currentPos] > '9' && name[currentPos] < 'A') { // change current letter on vertical movement
					name[currentPos] = 'A';
				}
				ostringstream convert;
				convert << name[currentPos];
				string letter = convert.str();
				nameEntities[currentPos]->SetText(letter);
				lastUp = (float) timer.GetMS();
			}
			else if (amount.y < -0.8f && timer.GetMS() > lastDown + 250) {
				name[currentPos]--;
				if (name[currentPos] < '0') {
					name[currentPos] = 'Z';
				}
				else if (name[currentPos] > '9' && name[currentPos] < 'A') {
					name[currentPos] = '9';
				}
				ostringstream convert;
				convert << name[currentPos];
				string letter = convert.str();
				nameEntities[currentPos]->SetText(letter);
				lastDown = (float) timer.GetMS();
			}
		}
		else {// Horizontal displacement - change letter position
			if (amount.x > 0.8f && timer.GetMS() > lastRight + 250) {
				currentPos = (currentPos + 1) % 4;
				lastRight = (float) timer.GetMS();
			}
			else if (amount.x < -0.8f && timer.GetMS() > lastLeft + 250) {
				currentPos--;
				if (currentPos < 0) currentPos = 3;
				lastLeft = (float) timer.GetMS();
			}
		}
	}
}
