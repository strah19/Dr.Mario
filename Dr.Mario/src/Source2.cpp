#include "Ember.h"

#define SCALE 3
#define NUM_COLORS 6

enum class PillColor {
	RED = 0, BLUE = 1, YELLOW = 2
};

enum class PillArangment {
	REDRED = 0, YELLOWYELLOW = 1, BLUEBLUE = 2, BLUERED = 3, YELLOWRED = 4, YELLOWBLUE = 5
};

class DrMario : public Ember::Application {
public:
	DrMario() : Application("Dr.Mario", false, 768, 672), screen("MainGamePlay"), background("res/background.png", renderer), bottle("res/bottle.png", renderer),
		main_theme_song("res/Dr Mario Fever.mp3") {
		screen.AddLayer("HomeScreen");
		screen.AddLayer("SettingsScreen");
		screen.SetCurrentLayer("MainGamePlay");

		InitPillTextures();
		pills.push_back(Pill(PillColor::RED, PillColor::RED, inner_bottle.pos, PillArangment::REDRED));
		srand(time(NULL));

		for (int i = 0; i < grid_cols; i++)
			for (int j = 0; j < grid_rows; j++)
				pill_grid[i][j] = -1;

		PrintGrid();
	}

	void PrintGrid() {
		system("CLS");

		for (int i = 0; i < grid_rows; i++) {
			std::cout << std::endl;
			for (int j = 0; j < grid_cols; j++)
				std::cout << pill_grid[j][i];
		}
	}

	~DrMario() {
		for (int i = 0; i < NUM_COLORS; i++) 
			delete pill_textures[i];
	}

	void InitPillTextures() {
		for (int i = 0; i < NUM_COLORS; i++) {
			std::string file_path = "res/pill";
			file_path.append(std::to_string(i) + ".png");
			pill_textures[i] = new Ember::Texture(file_path.c_str(), renderer);
		}
	}

	void OnUserUpdate() {	
		window->Update();

		renderer->Clear({ 200, 200, 200, 255 });

		UpdateScreens();

		renderer->Show();
	}

	void UpdateScreens() {
		screen.DrawEventToLayer(EMBER_BIND_EVENT(MainGameLoop), "MainGamePlay");
		screen.DrawEventToLayer(EMBER_BIND_EVENT(MainGameLoop), "HomeScreen");
	}

	void Collision() {
		current_grid_spot = { (pills.back().position.x - inner_bottle.x) / grid_size, (pills.back().position.y - inner_bottle.y) / grid_size };

		if (pills.back().angle == 0.0f || pills.back().angle == 180.0f) {
			if (pill_grid[current_grid_spot.x][current_grid_spot.y + 1] != -1 || pill_grid[current_grid_spot.x + 1][current_grid_spot.y + 1] != -1) {
				NextPill();
			}
			if (pills.back().position.y + (pills.back().position.h * 2) > inner_bottle.y + (grid_size * grid_rows))
				NextPill();
		}
		else {
			if (pill_grid[current_grid_spot.x][current_grid_spot.y + 2] != -1) {
				NextPill();
			}	
			if (pills.back().position.y + pills.back().position.h  + pills.back().position.w > inner_bottle.y + (grid_size * grid_rows))
				NextPill();
		}

		left_no = false;

		if (pills.back().position.x == inner_bottle.x)
			left_no = true;
		if(pill_grid[current_grid_spot.x - 1][current_grid_spot.y] != -1)
			if (pills.back().angle == 0.0f || pills.back().angle == 180.0f)
				left_no = true;
		if(pill_grid[current_grid_spot.x - 1][current_grid_spot.y] != -1 || pill_grid[current_grid_spot.x - 1][current_grid_spot.y - 1] != -1)
			if (pills.back().angle == 90.0f || pills.back().angle == 270.0f)
				left_no = true;

		right_no = false;

		if (current_grid_spot.x == grid_cols - 2 || pill_grid[current_grid_spot.x + 2][current_grid_spot.y] != -1) {
			if (pills.back().angle == 0.0f || pills.back().angle == 180.0f) 
				right_no = true;
		}

		if (current_grid_spot.x == grid_cols - 1 || pill_grid[current_grid_spot.x + 1][current_grid_spot.y] != -1 || pill_grid[current_grid_spot.x + 1][current_grid_spot.y + 1] != -1) 
			if (pills.back().angle == 90.0f || pills.back().angle == 270.0f)
				right_no = true;
	}

	void NextPill() {
		AddToGrid();
		PrintGrid();

		PillArangment ara= static_cast<PillArangment>(rand() % 6);
		switch((int) ara) {
		case 0: 
			pills.push_back(Pill(PillColor::RED, PillColor::RED, { inner_bottle.x + grid_size, inner_bottle.y }, PillArangment::REDRED));
		break;
		case 1: 
			pills.push_back(Pill(PillColor::YELLOW, PillColor::YELLOW, { inner_bottle.x + grid_size, inner_bottle.y }, PillArangment::YELLOWYELLOW));
		break;
		case 2: 
			pills.push_back(Pill(PillColor::BLUE, PillColor::BLUE, { inner_bottle.x + grid_size, inner_bottle.y }, PillArangment::BLUEBLUE));
		break;
		case 3: 
			pills.push_back(Pill(PillColor::BLUE, PillColor::RED, { inner_bottle.x + grid_size, inner_bottle.y }, PillArangment::BLUERED));
		break;
		case 4: 
			pills.push_back(Pill(PillColor::YELLOW, PillColor::RED, { inner_bottle.x + grid_size, inner_bottle.y }, PillArangment::YELLOWRED));
	    break;
		case 5: 
			pills.push_back(Pill(PillColor::YELLOW, PillColor::BLUE, { inner_bottle.x + grid_size, inner_bottle.y }, PillArangment::YELLOWBLUE));
		}

		ready_for_next_pill = false;
		std::cout << std::endl << "Current Colors: " << static_cast<int>(pills.back().left_color) << " " << static_cast<int>(pills.back().right_color) << std::endl;
	}

	void AddToGrid() {
		if (pills.back().angle == 0.0f || pills.back().angle == 180.0f) {
			pill_grid[current_grid_spot.x][current_grid_spot.y] = static_cast<int>(pills.back().left_color);
			pill_grid[current_grid_spot.x + 1][current_grid_spot.y] = static_cast<int>(pills.back().right_color);
		}
		else {
			pill_grid[current_grid_spot.x][current_grid_spot.y] = static_cast<int>(pills.back().left_color);
			pill_grid[current_grid_spot.x][current_grid_spot.y + 1] = static_cast<int>(pills.back().right_color);

		}
	}

	void MainGameLoop() {
		if (SDL_GetTicks() - pill_gravity >= pill_gravity_timing) {
			pill_gravity = SDL_GetTicks();
			pills.back().position.y += grid_size;
		}

		Collision();

		background.Draw(Ember::Rect({ 0, 0, window->Properties()->width, window->Properties()->height }));
		bottle.Draw(bottle_position);

		for (auto& pill : pills) {
			if (pill.angle == 90.0f || pill.angle == 270.0f) {
				pill_textures[static_cast<int>(pill.a)]->Draw(Ember::Rect(pill.position.x - (grid_size / 2) + 3, pill.position.y + grid_size / 2, pill.position.h, pill.position.w), pill.angle);

			}
			else {
				pill_textures[static_cast<int>(pill.a)]->Draw(pill.position, pill.angle);
			}
			renderer->Border(pill.position, { 255, 255, 255, 255 });
		}
	}

	void Rotate() {
		if (pills.back().angle == 270.0f)
			pills.back().angle = 0.0f;
		else 
			pills.back().angle += 90.0f;

		int w = pills.back().position.w;
		pills.back().position.w = pills.back().position.h;
		pills.back().position.h = w;

		if (current_grid_spot.x == grid_cols - 1)
			pills.back().position.x -= grid_size;

		if (pills.back().angle == 180.0f) {
			PillColor c = pills.back().left_color;
			pills.back().left_color = pills.back().right_color;
			pills.back().right_color = c;
		}
		if (pills.back().angle == 0.0f) {
			PillColor c = pills.back().left_color;
			pills.back().left_color = pills.back().right_color;
			pills.back().right_color = c;
		}
	}

	void HomeScreenLoop() {

	}

	bool KeyboardEvents(Ember::KeyboardEvents& keyboard) {
		if (keyboard.scancode == SDL_SCANCODE_ESCAPE) 
			window->Quit();
		else if (keyboard.scancode == SDL_SCANCODE_SPACE) 
			Rotate();
		if (!ready_for_next_pill) {
			if (keyboard.scancode == SDL_SCANCODE_DOWN) 
				pills.back().position.y += grid_size;
			if (keyboard.scancode == SDL_SCANCODE_LEFT && !left_no) 
				pills.back().position.x -= grid_size;
			if (keyboard.scancode == SDL_SCANCODE_RIGHT && !right_no) 
				pills.back().position.x += grid_size;
		}

		return false;
	}

	void UserDefEvent(Ember::Event& event) {
		Ember::EventDispatcher dispath(&event);

		dispath.Dispatch<Ember::KeyboardEvents>(EMBER_BIND_EVENT(KeyboardEvents));
	}
private:
	Ember::EventStack screen;
	Ember::Texture background;

	Ember::Texture* pill_textures[NUM_COLORS];
	Ember::Texture bottle;

	Ember::Rect bottle_position = { window->Properties()->width / 2 - (bottle.GetTextureInfo().x * SCALE) / 2, window->Properties()->height / 2 - (bottle.GetTextureInfo().y * SCALE) / 2, 240, 528 };
	Ember::IVec2 current_grid_spot = { 0, 0 };
	static const int grid_cols = 8;
	static const int grid_rows = 16;
	static const int grid_size = 8 * SCALE;
	const Ember::Rect inner_bottle = { 288, 192, grid_size * grid_cols, grid_size * grid_rows };

	Ember::AudioMusic main_theme_song;

	int pill_gravity = 0;
	const int pill_gravity_timing = 250;

	bool ready_for_next_pill = false;
	bool left_no = false;
	bool right_no = false;

	struct Pill {
		PillColor left_color;
		PillColor right_color;
		PillArangment a;

		Ember::Rect position;
		float angle = 0;
		Pill(PillColor color1, PillColor color2, const Ember::IVec2& pos, PillArangment a) : left_color(color1), right_color(color2), position(pos.x, pos.y, SCALE * 15, grid_size), a(a) { }
	};

	std::vector<Pill> pills;
	int pill_grid[grid_cols][grid_rows];
};

int main(int argc, char** argv)
{
	DrMario mario;
	mario.Run();

	return 0;
}