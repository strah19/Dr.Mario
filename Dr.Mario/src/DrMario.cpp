#include "Ember/Ember.h"
#include "DrMario.h"

#include "DrMarioHome.h"
#include "GameMechanics.h"
#include "DrMarioGamePlay.h"
#include "Options.h"

const std::string big_virus_file_path = "res/textures/big_virus_animations.png";
const Ember::IVec2 big_virus_size = { 6, 2 };

class DrMario : public Ember::Application {
public:
	DrMario() : Application("Dr.Mario", false, 768, 672), home_screen(renderer), game_screen(renderer), dr_mario_title_font(renderer, "res/drmarioFont.ttf", " ", 26, { 255, 255, 255, 255 }, 0, 0),
				game_play(renderer), dr_mario_font(renderer, "res/drmarioFont.ttf", " ", 18, { 255, 255, 255, 255 }, 0, 0), options(renderer), high_score_data("user_data.txt") {
		InitScreens();

		big_virus_info.texture.Initialize(big_virus_file_path, renderer);
		big_virus_info.spritesheet.Initialize(big_virus_info.texture, big_virus_size.x, big_virus_size.y);

		srand((unsigned int)time(NULL));

		std::stringstream ss;
		ss << high_score_data.ReadAll();
		ss >> high_score;
		high_score_data.EmptyFile();
	}

	void InitScreens() {
		screen = Ember::EventStack("MainGamePlay");

		screen.AddLayer("HomeScreen");
		screen.AddLayer("SettingsScreen");
		screen.AddLayer("OptionsMenu");
		screen.AddLayer("Pause");
		screen.SetCurrentLayer("HomeScreen");
	}

	~DrMario() {
		game_play.CleanUp();

		high_score_data.Write(high_score);
	}

	void OnUserUpdate() {
		window->Update();

		renderer->Clear(renderer_background);

		UpdateScreens();

		renderer->Show();
	}

	void UpdateScreens() {
		screen.DrawEventToLayer(EMBER_BIND_EVENT(OptionsMenuLoop), "OptionsMenu");
		screen.DrawEventToLayer(EMBER_BIND_EVENT(MainGameLoop), "MainGamePlay");
		screen.DrawEventToLayer(EMBER_BIND_EVENT(PauseLoop), "Pause");
		screen.DrawEventToLayer(EMBER_BIND_EVENT(HomeScreenLoop), "HomeScreen");
	}

	void HomeScreenLoop() {
		home_screen.DrawBackground({ window->Properties()->width, window->Properties()->height });

		home_screen.DrawMarioAnimation();
		home_screen.DrawBlueVirusAnimation(big_virus_info);
		home_screen.DrawTitleAnimation({ window->Properties()->width, window->Properties()->height });

		if (home_screen.Player1Button(dr_mario_title_font, events)) {
			screen.SetCurrentLayer("OptionsMenu");
		}
	}

	void PauseLoop() {
		int w, h;
		dr_mario_font.GetSize(&w, &h, "Pause");

		dr_mario_font.ChangeFont("Pause", { 255, 255, 255, 255 });
		dr_mario_font.SetPosition(window->Properties()->width / 2 - w/ 2, window->Properties()->height / 2 - h / 2);
		dr_mario_font.Render();
	}

	void OptionsMenuLoop() {
		options.DrawBackground({ window->Properties()->width, window->Properties()->height }, &dr_mario_title_font);
	}
	
	void MainGameLoop() {
		if (game_play.GetActivity()) 
			game_play.Update();
		
		if (game_play.PillReady()) {
			if (!game_play.DidLoose()) {
				game_play.NextPill(game_screen.GetMarioPosition());
				game_screen.Reset(false);
			}
			else {
				lost = true;
			}
		}
		game_screen.DrawBackground({ window->Properties()->width, window->Properties()->height });

		game_screen.DrawSpeed(dr_mario_font, options.GetSpeed());
		game_screen.DrawLevelText(dr_mario_font, level);
		virus_count = game_play.DrawVirus();
		game_screen.DrawVirusCount(dr_mario_font, virus_count);

		if (!game_play.GetActivity() && !lost) {
			game_play.PillAnim();
			if (!game_screen.IsReset()) {
					game_screen.ResetMarioAnim();
					game_screen.Reset(true);
			}
		}
		game_play.DrawPills();
		game_screen.BigVirusAnimRed(game_play.GetVirus(), big_virus_info);
		game_screen.BigVirusAnimBlue(game_play.GetVirus(), big_virus_info);
		game_screen.BigVirusAnimYellow (game_play.GetVirus(), big_virus_info);

		if(!lost)
			game_screen.MarioAnimation();

		if (lost) {
			game_play.Freeze();
			game_screen.Quit({ window->Properties()->width, window->Properties()->height });
			game_screen.MarioWhenGameOver();
		}
		if (virus_count == 0) {
			game_play.Freeze();
			game_screen.NextLevel({ window->Properties()->width, window->Properties()->height });
		}

		game_play.CalculateScore(score);
		game_screen.DrawHighestScore(&dr_mario_font, high_score);
		game_screen.DrawYourScore(&dr_mario_font, score);

		if (game_play.GetActivity()) 
			game_play.ForeshadowPill(game_screen.GetMarioPosition());
	}

	bool KeyboardEvents(Ember::KeyboardEvents& keyboard) {
		if (keyboard.scancode == SDL_SCANCODE_ESCAPE) {
			if (screen.GetLayer() == "OptionsMenu") 
				screen.SetCurrentLayer("HomeScreen");
			else
				window->Quit();
		}
		if (keyboard.scancode == SDL_SCANCODE_LEFT) {
			game_play.LMovement();
			if (screen.GetLayer() == "OptionsMenu")
				options.MoveLeft();
		}
		if (keyboard.scancode == SDL_SCANCODE_RIGHT) {
			game_play.RMovement();
			if (screen.GetLayer() == "OptionsMenu")
				options.MoveRight();
		}
		if (keyboard.scancode == SDL_SCANCODE_DOWN) {
			game_play.DownMovement();
			if (screen.GetLayer() == "OptionsMenu")
				options.MoveDown();
		}
		if (keyboard.scancode == SDL_SCANCODE_UP) {
			if (screen.GetLayer() == "OptionsMenu")
				options.MoveUp();
		}
		if (keyboard.scancode == SDL_SCANCODE_SPACE)
			game_play.Rotate();
		if (keyboard.scancode == SDL_SCANCODE_RETURN) {
			if (screen.GetLayer() == "OptionsMenu" && options.GettCurrentSettings() == 2) {
				screen.SetCurrentLayer("MainGamePlay");
				game_screen.Initialize(options.GetMusic());
				level = options.GetLevel();
				game_play.Initialize(level, options.GetSpeed());
			}
			if (screen.GetLayer() == "MainGamePlay" && virus_count == 0) {
				if (score > high_score)
					high_score = score;
				level++;
				game_play.Initialize(level, options.GetSpeed());
				game_play.ReStart();
			}
			if (screen.GetLayer() == "MainGamePlay" && lost) {
				game_play.Initialize(level, options.GetSpeed());
				game_play.ReStart();
				game_play.ResetScore();
				lost = false;
			}
		}
		if (keyboard.scancode == SDL_SCANCODE_P) {
			if (screen.GetLayer() == "MainGamePlay" || screen.GetLayer() == "Pause") {
				if (screen.GetLayer() == "MainGamePlay") {
					screen.SetCurrentLayer("Pause");
				}
				else if (screen.GetLayer() == "Pause") {
					screen.SetCurrentLayer("MainGamePlay");
				}
			}
		}
		return false;
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispath(&event);

		dispath.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_EVENT(KeyboardEvents));
	}
private:
	Ember::EventStack screen;

	Ember::Color renderer_background = { 0, 0, 0, 255 };

	int virus_count = 1;
	int level = 0;
	bool lost = false;

	TextureWithSpriteSheet big_virus_info;
	Ember::Font dr_mario_title_font;
	Ember::Font dr_mario_font;

	HomeScreen home_screen;
	GamePlayBackground game_screen;
	GamePlay game_play;
	Ember::File high_score_data;
	int high_score = 0;
	int score = 0;

	OptionsMenu options;
};

int main(int argc, char** argv) 
{
	DrMario mario; 
	mario.Run();

	return 0;
}