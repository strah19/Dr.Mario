#ifndef OPTIONS_H
#define OPTIONS_H

#include "Ember/Ember.h"
#include "DrMario.h"
#include "SimpleAnim.h"

class OptionsMenu {
public:
	OptionsMenu(Ember::rRenderer* renderer);

	void DrawBackground(const Ember::IVec2& window_size, Ember::Font* font);
	void MoveUp() { if (current_settings != 0) current_settings--; }
	void MoveDown() { if (current_settings < 2) current_settings++; }

	void MoveLeft() { 
		switch (current_settings) {
		case 0: {
			virus_level_controller--;
			IsVirusLevelRight(virus_level_controller);
			break;
		}
		case 1: {
			speed_controller--;
			IsSpeedAndMusicRight(speed_controller);
			break;
		}
		case 2: {
			music_type_controller--;
			IsSpeedAndMusicRight(music_type_controller);
			break;
		}
		}
	}
	void MoveRight() {
		switch (current_settings) {
		case 0: {
			virus_level_controller++;
			IsVirusLevelRight(virus_level_controller);
			break;
		}
		case 1: {
			speed_controller++;
			IsSpeedAndMusicRight(speed_controller);
			break;
		}
		case 2: {
			music_type_controller++;
			IsSpeedAndMusicRight(music_type_controller);
			break;
		}
		}
	}

	void IsVirusLevelRight(int& controller) {
		if (controller < 0)
			controller = 0;
		if (controller > 20)
			controller = 20;
	}

	void IsSpeedAndMusicRight(int& controller) {
		if (controller < 0)
			controller = 0;
		if (controller > 2)
			controller = 2;
	}

	int GetLevel() { return virus_level_controller; }
	Speed GetSpeed() {
		switch (speed_controller) {
		case 0: {
			return Speed::LOW;
			break;
		}
		case 1: {
			return Speed::MED;
			break;
		}
		case 2: {
			return Speed::HIGH;
			break;
		}
		}
	}
	int GetMusic() { return music_type_controller; }

	int GettCurrentSettings() { return current_settings; }
private:
	void UseActualSize(TextureWithSpriteSheet& obj, const Ember::IVec2& pos);

	void VirusLevel();
	void Speed();
	void Music();

	const std::string background_path = "res/textures/options_background.png";
	Ember::Texture background;

	const std::string selector_path = "res/textures/option_selectors.png";
	TextureWithSpriteSheet selectors;

	const std::string border_path = "res/textures/options_border.png";
	TextureWithSpriteSheet borders;

	const std::string virus_level_border_path = "res/textures/extra_options.png";
	Ember::Texture virus_level_border;

	int current_settings = 0;

	int virus_level_controller = 0;
	int speed_controller = 0;
	int music_type_controller = 0;

	Ember::IVec2 virus_level_pos = { 40 * SCALE, 52 * SCALE };
	Ember::IVec2 speed_pos = { 40 * SCALE, 104 * SCALE };
	Ember::IVec2 music_type_pos = { 40 * SCALE, 149 * SCALE };
};

#endif // !OPTIONS_H
