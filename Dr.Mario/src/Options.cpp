#include "Options.h"

OptionsMenu::OptionsMenu(Ember::rRenderer* renderer) {
	background.Initialize(background_path, renderer);

	selectors.texture.Initialize(selector_path, renderer);
	selectors.spritesheet.Initialize(selectors.texture, 2, 1);

	borders.texture.Initialize(border_path, renderer);
	borders.spritesheet.Initialize(borders.texture, 3, 3);

	virus_level_border.Initialize(virus_level_border_path, renderer);
}

void OptionsMenu::UseActualSize(TextureWithSpriteSheet& obj, const Ember::IVec2& pos) {
	obj.texture.Draw(Ember::Rect(pos.x, pos.y, obj.texture.GetTextureInfo().x, obj.texture.GetTextureInfo().y), obj.spritesheet.ReturnSourceRect());
}

void OptionsMenu::DrawBackground(const Ember::IVec2& window_size, Ember::Font* font) {
	background.Draw(Ember::Rect(0, 0, window_size.x, window_size.y));

	selectors.spritesheet.SelectSprite(0, 0);
	selectors.texture.Draw(Ember::Rect(87 * SCALE + (virus_level_controller * 12), 78 * SCALE, selectors.texture.GetTextureInfo().x * SCALE,
		selectors.texture.GetTextureInfo().y), selectors.spritesheet.ReturnSourceRect());

	font->ChangeFont(virus_level_controller, { 255, 255, 255, 255 });
	font->SetPosition(185 * SCALE, 72 * SCALE);
	font->Render();

	selectors.spritesheet.SelectSprite(0, 1);
	selectors.texture.Draw(Ember::Rect(89 * SCALE + (speed_controller * 120), 127 * SCALE, selectors.texture.GetTextureInfo().x * SCALE,
		selectors.texture.GetTextureInfo().y), selectors.spritesheet.ReturnSourceRect());

	if (music_type_controller == 0 || music_type_controller == 1) {
		borders.spritesheet.SelectSprite(2, 1);
		UseActualSize(borders, { 58 * SCALE + (music_type_controller * 170), 173 * SCALE });
	}
	else {
		borders.spritesheet.SelectSprite(2, 2);
		UseActualSize(borders, { 58 * SCALE + (music_type_controller * 170), 171 * SCALE });
	}

	virus_level_border.Draw(Ember::Rect(179 * SCALE, 69 * SCALE, virus_level_border.GetTextureInfo().x * SCALE, virus_level_border.GetTextureInfo().y * SCALE));

	switch (current_settings) {
	case 0: {
		VirusLevel();
		break;
	}
	case 1: {
		Speed();
		break;
	}
	case 2: {
		Music();
		break;
	}
	}
}


void OptionsMenu::VirusLevel() {
	borders.spritesheet.SelectSprite(0, 1);
	UseActualSize(borders, virus_level_pos);
	borders.spritesheet.SelectSprite(1, 2);
	UseActualSize(borders, { speed_pos.x + 12, speed_pos.y + 6 });
	borders.spritesheet.SelectSprite(1, 1);
	UseActualSize(borders, { music_type_pos.x, music_type_pos.y - 5 });

}

void OptionsMenu::Speed() {
	borders.spritesheet.SelectSprite(0, 2);
	UseActualSize(borders, speed_pos);
	borders.spritesheet.SelectSprite(1, 1);
	UseActualSize(borders, { music_type_pos.x, music_type_pos.y - 5 });
	borders.spritesheet.SelectSprite(1, 1);
	UseActualSize(borders, { virus_level_pos.x + 7, virus_level_pos.y - 5 });
}

void OptionsMenu::Music() {
	borders.spritesheet.SelectSprite(0, 1);
	UseActualSize(borders, music_type_pos);
	borders.spritesheet.SelectSprite(1, 2);
	UseActualSize(borders, { speed_pos.x + 12, speed_pos.y + 6 });
	borders.spritesheet.SelectSprite(1, 1);
	UseActualSize(borders, { virus_level_pos.x + 7, virus_level_pos.y - 5 });
}