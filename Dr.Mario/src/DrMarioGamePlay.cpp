#include "DrMarioGamePlay.h"
#include "GameMechanics.h"

GamePlayBackground::GamePlayBackground(Ember::rRenderer* renderer) {
	background.Initialize(game_background_path, renderer);

	bottle.Initialize(bottle_path, renderer);

	for (int i = 0; i < 3; i++) {
		std::string file_path = "res/textures/mario animation" + std::to_string(i + 1) + ".png";
		main_mario_swing_animation[i] = new Ember::Texture(file_path.c_str(), renderer);
	}

	mario_anim.Initialize(anim_speed);

	transitions.texture.Initialize(transition_path, renderer);
	transitions.spritesheet.Initialize(transitions.texture, 2, 1);

	mario_over.Initialize(mario_over_path, renderer);

	big_virus_timer[0].Initialize(300);
	big_virus_timer[1].Initialize(300);
	big_virus_timer[2].Initialize(300);
}

GamePlayBackground::~GamePlayBackground() {
	for (int i = 0; i < 3; i++)
		delete main_mario_swing_animation[i];
}

void GamePlayBackground::Initialize(int music) {
	switch (music) {
	case 0: {
		audio_music.Initialize("res/sounds/fever_music.mp3");
		audio_music.Play();
		break;
	}
	case 1: {
		audio_music.Initialize("res/sounds/chill_music.mp3");
		audio_music.Play();
		break;
	}
	case 2: {
		audio_music.Initialize("res/sounds/chill_music.mp3");
		audio_music.Disable();
	}
	}

}

void GamePlayBackground::DrawBackground(const Ember::IVec2& window_size) {
	background.Draw(Ember::Rect(0, 0, window_size.x, window_size.y));

	Ember::Rect bottle_position = { window_size.x / 2 - (bottle.GetTextureInfo().x * SCALE) / 2, window_size.y / 2 - (bottle.GetTextureInfo().y * SCALE) / 2, 240, 528 };
	bottle.Draw(bottle_position);
}

void GamePlayBackground::DrawLevelText(Ember::Font& font, int level) {
	font.ChangeFont(level, { 0, 0, 0, 255 });
	font.SetPosition(215 * SCALE, 145 * SCALE);
	font.Render();
}

void GamePlayBackground::DrawVirusCount(Ember::Font& font, int count) {
	font.ChangeFont(count, { 0, 0, 0, 255 });
	font.SetPosition(215 * SCALE, 190 * SCALE);
	font.Render();
}

void GamePlayBackground::DrawSpeed(Ember::Font& font, Speed speed) {
	font.ChangeFont(GetStringFromSpeed(speed), { 0, 0, 0, 255 });
	font.SetPosition(215 * SCALE, 170 * SCALE);
	font.Render();
}

bool GamePlayBackground::MarioAnimation() {
	bool complete = false;
	if (mario_anim.Update()) {
		if (mario_anim.GetFrame() == 4) {
			mario_anim_active = false;
		}
	}
	if (mario_anim_active) {
		if (mario_anim.GetFrame() == 2)
			main_mario_swing_animation[mario_anim.GetFrame() - 1]->Draw(Ember::Rect(mario_pos.x - (9 * SCALE), mario_pos.y,
				main_mario_swing_animation[mario_anim.GetFrame() - 1]->GetTextureInfo().x * SCALE, main_mario_swing_animation[mario_anim.GetFrame() - 1]->GetTextureInfo().y * SCALE));
		else
			main_mario_swing_animation[mario_anim.GetFrame() - 1]->Draw(Ember::Rect(mario_pos.x, mario_pos.y,
				main_mario_swing_animation[mario_anim.GetFrame() - 1]->GetTextureInfo().x * SCALE, main_mario_swing_animation[mario_anim.GetFrame() - 1]->GetTextureInfo().y * SCALE));
	}
	else {
		main_mario_swing_animation[0]->Draw(Ember::Rect(mario_pos.x, mario_pos.y,
			main_mario_swing_animation[0]->GetTextureInfo().x * SCALE, main_mario_swing_animation[0]->GetTextureInfo().y * SCALE));
	}

	return complete;
}

void GamePlayBackground::BigVirusAnimBlue(PillColor color, TextureWithSpriteSheet& virus) {
	if (big_virus_timer[0].Update()) {
		if (big_virus_timer[0].GetFrame() == 3)
			big_virus_timer[0].SetFrame(1);
	}

	if (color == PillColor::BLUE) {
		big_virus_timer[0].ChangeTrigger(50);
		virus.spritesheet.SelectSprite(big_virus_timer[0].GetFrame() - 1, 3);
	}
	else {
		big_virus_timer[0].ChangeTrigger(300);
		virus.spritesheet.SelectSprite(big_virus_timer[0].GetFrame() - 1, 0);
	}
	virus.texture.Draw(Ember::Rect(13 * SCALE, 155 * SCALE, 28 * SCALE, 21 * SCALE), virus.spritesheet.ReturnSourceRect());
}

void GamePlayBackground::BigVirusAnimYellow(PillColor color, TextureWithSpriteSheet& virus) {
	if (big_virus_timer[1].Update()) {
		if (big_virus_timer[1].GetFrame() == 3)
			big_virus_timer[1].SetFrame(1);
	}

	if (color == PillColor::YELLOW) {
		big_virus_timer[1].ChangeTrigger(50);
		virus.spritesheet.SelectSprite(big_virus_timer[1].GetFrame() - 1, 5);
	}
	else {
		big_virus_timer[1].ChangeTrigger(300);
		virus.spritesheet.SelectSprite(big_virus_timer[1].GetFrame() - 1, 2);
	}
	virus.texture.Draw(Ember::Rect(30 * SCALE, 130 * SCALE, 28 * SCALE, 21 * SCALE), virus.spritesheet.ReturnSourceRect());
}

void GamePlayBackground::BigVirusAnimRed(PillColor color, TextureWithSpriteSheet& virus) {
	if (big_virus_timer[2].Update()) {
		if (big_virus_timer[2].GetFrame() == 3)
			big_virus_timer[2].SetFrame(1);
	}

	if (color == PillColor::RED) {
		big_virus_timer[2].ChangeTrigger(50);
		virus.spritesheet.SelectSprite(big_virus_timer[2].GetFrame() - 1, 4);
	}
	else {
		big_virus_timer[2].ChangeTrigger(300);
		virus.spritesheet.SelectSprite(big_virus_timer[2].GetFrame() - 1, 1);
	}
	virus.texture.Draw(Ember::Rect(41 * SCALE, 155 * SCALE, 28 * SCALE, 21 * SCALE), virus.spritesheet.ReturnSourceRect());
}

void GamePlayBackground::MarioWhenGameOver() {
	mario_over.Draw(Ember::Rect(mario_pos.x - 5 * SCALE, mario_pos.y,
		mario_over.GetTextureInfo().x * SCALE, mario_over.GetTextureInfo().y * SCALE));
}

void GamePlayBackground::ResetMarioAnim() {
	mario_anim.SetFrame(1);
	mario_anim_active = true;
}

std::string GetStringFromSpeed(Speed speed) {
	std::string speed_text = "";

	switch (speed) {
	case Speed::LOW: {
		speed_text = "LOW";
		break;
	}
	case Speed::MED: {
		speed_text = "MED";
		break;
	}
	case Speed::HIGH: {
		speed_text = "HI";
		break;
	}
	default:
		break;
	}

	return speed_text;
}

void GamePlayBackground::Quit(const Ember::IVec2& window_size) {
	transitions.spritesheet.SelectSprite(0, 1);
	transitions.texture.Draw(Ember::Rect(window_size.x / 2 - (transitions.texture.GetTextureInfo().x * SCALE) / 2, window_size.y / 2 - (transitions.texture.GetTextureInfo().y) / 2,
		transitions.texture.GetTextureInfo().x * SCALE, transitions.texture.GetTextureInfo().y), transitions.spritesheet.ReturnSourceRect());
}

void GamePlayBackground::NextLevel(const Ember::IVec2& window_size) {
	transitions.spritesheet.SelectSprite(0, 0);
	transitions.texture.Draw(Ember::Rect(window_size.x / 2 - (transitions.texture.GetTextureInfo().x * SCALE) / 2, window_size.y / 2 - (transitions.texture.GetTextureInfo().y) / 2,
		transitions.texture.GetTextureInfo().x * SCALE, transitions.texture.GetTextureInfo().y), transitions.spritesheet.ReturnSourceRect());
}

void GamePlayBackground::DrawHighestScore(Ember::Font* font, int high_score) {
	font->ChangeFont(high_score, { 0, 0, 0, 255 });
	font->SetPosition(20 * SCALE, 58 * SCALE);
	font->Render();
}

void GamePlayBackground::DrawYourScore(Ember::Font* font, int score) {
	font->ChangeFont(score, { 0, 0, 0, 255 });
	font->SetPosition(20 * SCALE, 82 * SCALE);
	font->Render();
}