#ifndef DR_MARIO_HOME_H
#define DR_MARIO_HOME_H

#include "Ember/Ember.h"
#include "SimpleAnim.h"
#include "DrMario.h"

class HomeScreen {
public:
	HomeScreen(Ember::rRenderer* renderer);
	void DrawBackground(const Ember::IVec2& window_size);

	void DrawBlueVirusAnimation(TextureWithSpriteSheet& big_virus_info);
	void DrawMarioAnimation();
	void DrawTitleAnimation(const Ember::IVec2& window_size);

	bool Player1Button(Ember::Font& font, Ember::Events* events);

	void StopMusic() { home_music.Disable(); }
private:
	Ember::AudioMusic home_music;

	SimpleAnim home_screen_mario_animation;
	TextureWithSpriteSheet mario_info;

	SimpleAnim home_screen_blue_virus_animation;

	SimpleAnim home_screen_title_animation;
	TextureWithSpriteSheet title_info;

	Ember::Texture tile_background;
	Ember::Texture home_screen_header;
	Ember::Texture huge_pill;

	const int header_y_position = 450;
	const std::string home_screen_tile_background_path = "res/textures/green_tile_grid.png";
	const std::string home_screen_header_path = "res/textures/home_screen_header.png";
	const std::string home_screen_mario_path = "res/textures/title_mario_animations.png";
	const std::string home_screen_pill_path = "res/textures/huge_pill.png";
	const std::string home_screen_title_animation_path = "res/textures/title_animation.png";

	const std::string home_screen_music_path = "res/sounds/title_music.mp3";

	const int animation_speeds = 200;
	const int home_screen_music_volume = 64;

	Ember::Rect mario_position = { 40 * SCALE, 157 * SCALE, 36 * SCALE, 38 * SCALE };
};

#endif // !DR_MARIO_HOME_H
