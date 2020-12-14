#include "Ember.h"

#define SCALE 3

enum class PillColor {
	RED = 0, BLUE = 1, YELLOW = 2
};

class DrMario : public Ember::Application {
public:
	DrMario() : Ember::Application("Dr.Mario", false, 768, 672), screen("MainGamePlay") { 
		InitScreens();

		srand((unsigned int) time(NULL));
	}

	~DrMario() {

	}

	void OnUserUpdate() {
		window->Update();

		renderer->Clear({ 200, 200, 200, 255 });
		UpdateScreens();
		renderer->Show();
	}

	void UpdateScreens() {
		screen.DrawEventToLayer(EMBER_BIND_EVENT(Quit), "QuitScreen");
		screen.DrawEventToLayer(EMBER_BIND_EVENT(MainGameLoop), "MainGamePlay");
		screen.DrawEventToLayer(EMBER_BIND_EVENT(HomeScreen), "HomeScreen");
		screen.DrawEventToLayer(EMBER_BIND_EVENT(HomeScreen), "Pause");
	}

	void InitScreens() {
		screen.AddLayer("HomeScreen");
		screen.AddLayer("SettingsScreen");
		screen.AddLayer("QuitScreen");
		screen.AddLayer("Pause");

		screen.SetCurrentLayer("MainGamePlay");
	}

	void MainGameLoop() {

	}

	void Pause() {

	}

	void HomeScreen() {

	}

	void Quit() {

	}
private:
	Ember::EventStack screen;

	Ember::Texture background;
	Ember::Texture bottle;
};

int main(int argc, char** argv)
{
	DrMario mario;
	mario.Run();

	return 0;
}