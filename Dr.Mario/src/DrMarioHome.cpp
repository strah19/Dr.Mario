#include "DrMarioHome.h"

HomeScreen::HomeScreen(Ember::rRenderer* renderer) {
	tile_background.Initialize(home_screen_tile_background_path, renderer);
	home_screen_header.Initialize(home_screen_header_path, renderer);

	home_music.Initialize(home_screen_music_path);
	home_music.Play();
	home_music.Volume(home_screen_music_volume);

	home_screen_mario_animation.Initialize(animation_speeds);
	home_screen_blue_virus_animation.Initialize(animation_speeds);

	mario_info.texture.Initialize(home_screen_mario_path, renderer);
	mario_info.spritesheet.Initialize(mario_info.texture, 1, 2);

	huge_pill.Initialize(home_screen_pill_path, renderer);

	home_screen_title_animation.Initialize(animation_speeds);
	title_info.texture.Initialize(home_screen_title_animation_path, renderer);
	title_info.spritesheet.Initialize(title_info.texture, 2, 1);
}

void HomeScreen::DrawBackground(const Ember::IVec2& window_size) {
	tile_background.Draw(Ember::Rect(0, 0, window_size.x, window_size.y));
	huge_pill.Draw(Ember::Rect(window_size.x / 2 - (huge_pill.GetTextureInfo().x * SCALE / 2), 25, huge_pill.GetTextureInfo().x * SCALE, huge_pill.GetTextureInfo().y * SCALE));

	home_screen_header.Draw(Ember::Rect((window_size.x / 2) - (home_screen_header.GetTextureInfo().x * SCALE / 2), header_y_position, 
		home_screen_header.GetTextureInfo().x * SCALE, home_screen_header.GetTextureInfo().y * SCALE));
}

void HomeScreen::DrawBlueVirusAnimation(TextureWithSpriteSheet& big_virus_info) {
	if (home_screen_blue_virus_animation.Update()) {
		if (home_screen_blue_virus_animation.GetFrame() == 3)
			home_screen_blue_virus_animation.SetFrame(1);
	}

	big_virus_info.spritesheet.SelectSprite(home_screen_blue_virus_animation.GetFrame() - 1, 0);
	big_virus_info.texture.Draw(Ember::Rect(180 * SCALE, 167 * SCALE, 38 * SCALE, 24 * SCALE), big_virus_info.spritesheet.ReturnSourceRect());
}

void HomeScreen::DrawMarioAnimation() {
	if (home_screen_mario_animation.Update()) {
		if (home_screen_mario_animation.GetFrame() == 3)
			home_screen_mario_animation.SetFrame(1);
	}
	mario_info.spritesheet.SelectSprite(home_screen_mario_animation.GetFrame() - 1, 0);
	mario_info.texture.Draw(mario_position, mario_info.spritesheet.ReturnSourceRect());
}

void HomeScreen::DrawTitleAnimation(const Ember::IVec2& window_size) {
	if (home_screen_title_animation.Update()) {
		if (home_screen_title_animation.GetFrame() == 3)
			home_screen_title_animation.SetFrame(1);
	}

	title_info.spritesheet.SelectSprite(0, home_screen_title_animation.GetFrame() - 1);
	title_info.texture.Draw(Ember::Rect((window_size.x / 2) - (title_info.texture.GetTextureInfo().x * SCALE / 2), 75, 
		title_info.texture.GetTextureInfo().x * SCALE, (title_info.texture.GetTextureInfo().y * SCALE) / 2), title_info.spritesheet.ReturnSourceRect());
}

bool HomeScreen::Player1Button(Ember::Font& font, Ember::Events* events) {
	Ember::Button player_1_button(events, 71 * SCALE, 154 * SCALE, 375, 38);
	if (player_1_button.Hover()) {
		font.ChangeFont("1 PLAYER GAME", { 200, 200, 200, 255 });
		font.SetPosition(player_1_button.Position().x, player_1_button.Position().y);
		font.Render();
	}
	else {
		font.ChangeFont("1 PLAYER GAME", { 255, 255, 255, 255 });
		font.SetPosition(player_1_button.Position().x, player_1_button.Position().y);
		font.Render();
	}

	if (player_1_button.Click(Ember::ButtonIds::LeftMouseButton)) {
		return true;
	}

	return false;
}