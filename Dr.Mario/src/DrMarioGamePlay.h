#ifndef DR_MARIO_GAME_PLAY_H
#define DR_MARIO_GAME_PLAY_H

#include "Ember/Ember.h"
#include "SimpleAnim.h"
#include "DrMario.h"

#include "Pill.h"
#include "Virus.h"

/* draws background, big virus's, text, loosing, screen, pause, etc. */
class GamePlayBackground {
public:
	GamePlayBackground(Ember::rRenderer* renderer);
	~GamePlayBackground();

	void DrawBackground(const Ember::IVec2& window_size);

	void DrawLevelText(Ember::Font& font, int level);
	void DrawVirusCount(Ember::Font& font, int count);
	void DrawSpeed(Ember::Font& font, Speed speed);

	void Initialize(int music);

	void ResetMarioAnim();
	bool MarioAnimation();
	Ember::IVec2 GetMarioPosition() { return mario_pos; }

	void Quit(const Ember::IVec2& window_size);
	void NextLevel(const Ember::IVec2& window_size);

	bool IsReset() { return reset_once; }
	void Reset(bool r) { reset_once = r; }

	void MarioWhenGameOver();

	void BigVirusAnimBlue(PillColor color, TextureWithSpriteSheet& virus);
	void BigVirusAnimRed(PillColor color, TextureWithSpriteSheet& virus);
	void BigVirusAnimYellow(PillColor color, TextureWithSpriteSheet& virus);

	void DrawHighestScore(Ember::Font* font, int high_score);
	void DrawYourScore(Ember::Font* font, int score);

	Ember::AudioMusic GetMusic() { return audio_music; }
private:
	bool mario_anim_active = true;
	bool reset_once = false;

	const Ember::Rect inner_bottle = { 288, 192, GRID_SIZE * GRID_COLS, GRID_SIZE * GRID_COLS };
	Ember::IVec2 mario_pos = { 574, 210 };

	const int anim_speed = 100;
	SimpleAnim mario_anim;
	Ember::Texture* main_mario_swing_animation[3];

	Ember::Texture background;
	const std::string game_background_path = "res/textures/background.png";

	Ember::Texture bottle;
	const std::string bottle_path = "res/textures/bottle.png";

	Ember::AudioMusic audio_music;

	TextureWithSpriteSheet transitions;
	const std::string transition_path = "res/textures/transition.png";

	SimpleAnim big_virus_timer[3];

	Ember::Texture mario_over;
	const std::string mario_over_path = "res/textures/mario_over.png";
};

#endif // !DR_MARIO_GAME_PLAY_H
