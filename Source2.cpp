#include "Ember.h"

#define SCALE 3

enum class PillColor {
	RED = 0, BLUE = 1, YELLOW = 2, NONE = 3
};

enum class PillRenderingStyle {
	FULL_PILL, FRONT_PILL, BACK_PILL
};

class DrMario : public Ember::Application {
public:
	DrMario() : Application("Dr.Mario", false, 768, 672), screen("MainGamePlay"), background("res/background.png", renderer), bottle("res/bottle.png", renderer),
		main_theme_song("res/Dr Mario Fever.mp3"), p_texture("res/un_animated_pills.png", renderer), sprite_sheet_of_pills(p_texture, 3, 3),
		p_animation("res/pill_animations.png", renderer), sprite_sheet_of_pill_animation(p_animation, 2, 3),
		virus_animation("res/virus's.png", renderer), sprite_sheet_of_virus_animation(virus_animation, 3, 2) {
		screen.AddLayer("HomeScreen");
		screen.AddLayer("SettingsScreen");
		screen.AddLayer("QuitScreen");
		screen.SetCurrentLayer("MainGamePlay");

		srand(time(NULL));

		for (int i = 0; i < grid_cols; i++) {
			for (int j = 0; j < grid_rows; j++) {
				pill_grid[i][j] = -1;
				int random = rand() % 75;
				
				if (random > 65 && j > grid_cols / 2) {
				//	pill_grid[i][j] = rand() % 3;
					//virus.push_back(Virus({ (i * grid_size) + inner_bottle.x, (j * grid_size) + inner_bottle.y }, static_cast<PillColor>(pill_grid[i][j])));
				//	virus.back().sprite_sheet_location = { 0, pill_grid[i][j] };
					//virus.back().f = &pill_grid[i][j];
				//	std::cout << *virus.back().f << std::endl;
				}
				
			}
		}

		for (int i = 0; i < 3; i++) {
			std::string file_path = "res/mario animation" + std::to_string(i + 1) + ".png";
			mario_textures[i] = new Ember::Texture(file_path.c_str(), renderer);
		}

		pills.push_back(Pill(PillColor::RED, PillColor::RED, { mario_pos.x, inner_bottle.y }));
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
		for (int i = 0; i < 3; i++)
			delete mario_textures[i];
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
		screen.DrawEventToLayer(EMBER_BIND_EVENT(MainGameLoop), "HomeScreen");
	}

	void Quit() {

	}

	void Collision() {
		current_grid_spot = { (pills.back().position.x - inner_bottle.x) / grid_size, (pills.back().position.y - inner_bottle.y) / grid_size };
		pills.back().grid_loc = current_grid_spot;

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
			if (pills.back().position.y + pills.back().position.h + pills.back().position.w > inner_bottle.y + (grid_size * grid_rows))
				NextPill();
		}

		left_no = false;

		if (pills.back().position.x == inner_bottle.x)
			left_no = true;
		if (pill_grid[current_grid_spot.x - 1][current_grid_spot.y] != -1)
			if (pills.back().angle == 0.0f || pills.back().angle == 180.0f)
				left_no = true;
		if (pill_grid[current_grid_spot.x - 1][current_grid_spot.y] != -1 || pill_grid[current_grid_spot.x - 1][current_grid_spot.y + 1] != -1)
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
		pills.back().placed = true;
		if (pills.back().angle == 0.0f || pills.back().angle == 180.0f) {
			pills.back().f = &pill_grid[current_grid_spot.x][current_grid_spot.y];
			pills.back().s = &pill_grid[current_grid_spot.x + 1][current_grid_spot.y];
		}
		else {
			pills.back().f = &pill_grid[current_grid_spot.x][current_grid_spot.y];
			pills.back().s = &pill_grid[current_grid_spot.x][current_grid_spot.y + 1];
		}
		AddToGrid();
		ConfigWithEach();

		PillColor c1 = static_cast<PillColor>(rand() % 3);
		PillColor c2 = static_cast<PillColor>(rand() % 3);
		Ember::IVec2 s = { 0, 0 };
		if (cstart1 == PillColor::RED) {
			if (cstart2 == PillColor::RED)
				s = { 0, 0 };
			else if (cstart2 == PillColor::BLUE)
				s = { 0, 1 };
			else if (cstart2 == PillColor::YELLOW)
				s = { 0, 2 };
		}
		else if (cstart1 == PillColor::BLUE) {
			if (cstart2 == PillColor::RED)
				s = { 1, 1 };
			else if (cstart2 == PillColor::BLUE)
				s = { 1, 0 };
			else if (cstart2 == PillColor::YELLOW)
				s = { 1, 2 };
		}
		if (cstart1 == PillColor::YELLOW) {
			if (cstart2 == PillColor::RED)
				s = { 2, 1 };
			else if (cstart2 == PillColor::BLUE)
				s = { 2, 2 };
			else if (cstart2 == PillColor::YELLOW)
				s = { 2, 0 };
		}

		pills.push_back(Pill(cstart1, cstart2, { mario_pos.x, inner_bottle.y }));
		pills.back().sprite_sheet_location = s;
		ready_for_next_pill = false;
		pill_ready = false;
		updating_all_pills_new_pos = true;
	}

	enum class Direction {
		Left, Right, Up, Down
	};

	void ConfigWithEach() {
		PrintGrid();
		if (pills.back().angle == 0.0f || pills.back().angle == 180.0f) {
			int left = InARow(current_grid_spot, pill_grid[current_grid_spot.x][current_grid_spot.y], 4, Direction::Left, 0, 8, 0, 16);
			int right = InARow({ current_grid_spot.x + 1, current_grid_spot.y }, pill_grid[current_grid_spot.x + 1][current_grid_spot.y], 4, Direction::Right, 0, 8, 0, 16);
			int left_c = InARow(current_grid_spot, pill_grid[current_grid_spot.x][current_grid_spot.y], 4, Direction::Down, 0, 8, 0, 16);
			int right_c = InARow({ current_grid_spot.x + 1, current_grid_spot.y }, pill_grid[current_grid_spot.x + 1][current_grid_spot.y], 4, Direction::Down, 0, 8, 0, 16);
			int left_up = InARow(current_grid_spot, pill_grid[current_grid_spot.x][current_grid_spot.y], 4, Direction::Up, 0, 8, 0, 16);
			int right_up = InARow({ current_grid_spot.x + 1, current_grid_spot.y }, pill_grid[current_grid_spot.x + 1][current_grid_spot.y], 4, Direction::Up, 0, 8, 0, 16);

			if (pill_grid[current_grid_spot.x][current_grid_spot.y] == pill_grid[current_grid_spot.x + 1][current_grid_spot.y]) {
				if (FinalCheck(left + right)) {
					for (int i = 0; i < left + right; i++) {
						pill_grid[current_grid_spot.x - left + i + 1][current_grid_spot.y] = -1; 
					}
				}
			}
			else {
				if (FinalCheck(left)) {
					for (int i = 0; i < left; i++) {
						pill_grid[current_grid_spot.x - left + i + 1][current_grid_spot.y] = -1;
					}
				}
				if(FinalCheck(right)) {
					for (int i = 0; i < right; i++) {
						pill_grid[current_grid_spot.x + i + 1][current_grid_spot.y] = -1;
					}
				}
			}

			if (FinalCheck(left_c + left_up - 1)) {
				for (int i = 0; i < left_c + left_up; i++) {
					pill_grid[current_grid_spot.x][current_grid_spot.y - left_up + i] = -1;
				}
			}

			if (FinalCheck(right_c + right_up - 1)) {
				for (int i = 0; i < right_c + right_up; i++) {
					pill_grid[current_grid_spot.x + 1][current_grid_spot.y - right_up + i] = -1;
				}
			}
		}
		if (pills.back().angle == 270.0f || pills.back().angle == 90.0f) {
			int left_top = InARow(current_grid_spot, pill_grid[current_grid_spot.x][current_grid_spot.y], 4, Direction::Left, 0, 8, 0, 16);
			int right_top = InARow({ current_grid_spot.x, current_grid_spot.y }, pill_grid[current_grid_spot.x][current_grid_spot.y], 4, Direction::Right, 0, 8, 0, 16);
			int left_bot = InARow({ current_grid_spot.x, current_grid_spot.y + 1 }, pill_grid[current_grid_spot.x][current_grid_spot.y + 1], 4, Direction::Left, 0, 8, 0, 16);
			int right_bot = InARow({ current_grid_spot.x, current_grid_spot.y + 1 }, pill_grid[current_grid_spot.x][current_grid_spot.y + 1], 4, Direction::Right, 0, 8, 0, 16);
			int up = InARow(current_grid_spot, pill_grid[current_grid_spot.x][current_grid_spot.y], 4, Direction::Up, 0, 8, 0, 16);
			int down = InARow({ current_grid_spot.x, current_grid_spot.y + 1 }, pill_grid[current_grid_spot.x][current_grid_spot.y + 1], 4, Direction::Down, 0, 8, 0, 16);

			if (FinalCheck(left_bot + right_bot - 1)) {
				for (int i = 0; i < left_bot + right_bot; i++) {
					pill_grid[current_grid_spot.x - left_bot + i][current_grid_spot.y + 1] = -1;
				}
			}
			if (FinalCheck(left_top + right_top - 1)) {
				for (int i = 0; i < left_top + right_top; i++) {
					pill_grid[current_grid_spot.x - left_top + i][current_grid_spot.y] = -1;
				}
			}

			if (pill_grid[current_grid_spot.x][current_grid_spot.y] == pill_grid[current_grid_spot.x][current_grid_spot.y + 1])
				if (FinalCheck(up + down)) {
					for (int i = 0; i < up + down; i++) {
						pill_grid[current_grid_spot.x][current_grid_spot.y - up + i + 1] = -1;
					}
				}
			if (pill_grid[current_grid_spot.x][current_grid_spot.y] != pill_grid[current_grid_spot.x][current_grid_spot.y + 1]) {
				std::cout << down << std::endl;
				if (FinalCheck(up)) {
					for (int i = 0; i < up; i++) {
						pill_grid[current_grid_spot.x][current_grid_spot.y - i] = -1;
					}
				}
				if (FinalCheck(down)) {
					for (int i = 0; i <= down; i++) {
						pill_grid[current_grid_spot.x][current_grid_spot.y + i + 1] = -1;
					}
				}
			}
		}
	}

	bool FinalCheck(int i) {
		return (i >= 4);
	}

	int InARow(const Ember::IVec2& pos, int color, int range, Direction direction, int lborder, int rborder, int uborder, int dborder) {
		int in_row = 0;
		for (int i = 0; i < range; i++) {
			if (direction == Direction::Left && pos.x - i > lborder - 1) {
				if (pill_grid[pos.x - i][pos.y] == color)
					in_row++;
				else
					break;
			}
			else if (direction == Direction::Right && pos.x + i < rborder) {
				if (pill_grid[pos.x + i][pos.y] == color)
					in_row++;
				else
					break;
			}
			else if (direction == Direction::Up && pos.y - i > uborder) {
				if (pill_grid[pos.x][pos.y - i] == color)
					in_row++;
				else
					break;
			}
			else if (direction == Direction::Down && pos.y && pos.y + i <= dborder) {
				if (pill_grid[pos.x][pos.y + i] == color)
					in_row++;
				else
					break;
			}
		} 

		return in_row;
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
		static bool stage_1 = false;
		if (SDL_GetTicks() - pill_gravity >= pill_gravity_timing) {
			pill_gravity = SDL_GetTicks();
			if (pill_ready) {
				pills.back().position.y += grid_size;
			}

			if (updating_all_pills_new_pos) {
				int s = 0;
				for (int i = 0; i < pills.size() - 1; i++) {
					if (*pills[i].f == -1 && *pills[i].s != -1) {
						if (pill_grid[pills[i].grid_loc.x + 1][pills[i].grid_loc.y + 1] == -1 && pills[i].grid_loc.y < 15) {						 
							pills[i].f = &pill_grid[pills[i].grid_loc.x][pills[i].grid_loc.y + 1];
							pills[i].s = &pill_grid[pills[i].grid_loc.x + 1][pills[i].grid_loc.y + 1];
							
							pill_grid[pills[i].grid_loc.x + 1][pills[i].grid_loc.y] = -1;
							pill_grid[pills[i].grid_loc.x + 1][pills[i].grid_loc.y + 1] = (int)pills[i].right_color;

							pills[i].position.y += grid_size;
							pills[i].grid_loc.y += 1;

						}
						else
							s++;
					} 
					else if (*pills[i].f != -1 && *pills[i].s == -1) {
						if (pill_grid[pills[i].grid_loc.x][pills[i].grid_loc.y + 1] == -1 && pills[i].grid_loc.y < 15) {
							pills[i].f = &pill_grid[pills[i].grid_loc.x][pills[i].grid_loc.y + 1];
							pills[i].s = &pill_grid[pills[i].grid_loc.x + 1][pills[i].grid_loc.y + 1];

							pill_grid[pills[i].grid_loc.x][pills[i].grid_loc.y] = -1;
							pill_grid[pills[i].grid_loc.x][pills[i].grid_loc.y + 1] = (int)pills[i].left_color;

							pills[i].position.y += grid_size;
							pills[i].grid_loc.y += 1;

						}
						else
							s++;
					}
					else
						s++;
				}
				if (s == pills.size() - 1) {
					updating_all_pills_new_pos = false;
				}
			}
		}
		if (pill_ready) {
			Collision();
			stage_1 = false;
			current_frame = 0;
			stop_mario = false;
		}
		if(!updating_all_pills_new_pos && !pill_ready) {
			if (SDL_GetTicks() - mario_timing >= 100 && !stop_mario) {
				current_frame++;
				mario_timing = SDL_GetTicks();
			 if (current_frame == 3) {
				stop_mario = true;
				current_frame = 0;
			}
			}

			if (pills.back().position.x == inner_bottle.x + (inner_bottle.w / 2) - (pills.back().position.w / 2)) {
				stage_1 = true;
			}
			if(!stage_1) {
				pills.back().position.y -= 3;
				pills.back().position.x -= 4;
			}

			if (stage_1) {
				pills.back().position.y += 3;
				pills.back().angle += 17;
			}

			if (pills.back().position.y == inner_bottle.y - grid_size && stage_1) {
				pill_ready = true;
				pills.back().angle = 0;

				cstart1 = static_cast<PillColor>(rand() % 3);
				cstart2 = static_cast<PillColor>(rand() % 3);


				if (pill_grid[4][0] != -1) {
					screen.SetCurrentLayer("QuitScreen");
				}
			}
		}
		background.Draw(Ember::Rect({ 0, 0, window->Properties()->width, window->Properties()->height }));
		bottle.Draw(bottle_position);

		for (auto& pill : pills) {
			sprite_sheet_of_pills.SelectSprite(pill.sprite_sheet_location.x, pill.sprite_sheet_location.y);
			if (!pill.placed) {
				if (pill.angle == 90.0f || pill.angle == 270.0f) {
					p_texture.Draw(Ember::Rect(pill.position.x - (grid_size / 2), pill.position.y + grid_size / 2, pill.position.h, pill.position.w), sprite_sheet_of_pills.ReturnSourceRect(),
						SDL_FLIP_NONE, pill.angle);
				}
				else {
					p_texture.Draw(pill.position, sprite_sheet_of_pills.ReturnSourceRect(), SDL_FLIP_NONE, pill.angle);
				}
			}
			else {
				if (*pill.f != -1 && *pill.s != -1 && pill.cut == 0) {
					if (pill.angle == 90.0f || pill.angle == 270.0f) {
						p_texture.Draw(Ember::Rect(pill.position.x - (grid_size / 2), pill.position.y + grid_size / 2, pill.position.h, pill.position.w), sprite_sheet_of_pills.ReturnSourceRect(),
							SDL_FLIP_NONE, pill.angle);
					}
					else {
						p_texture.Draw(pill.position, sprite_sheet_of_pills.ReturnSourceRect(), SDL_FLIP_NONE, pill.angle);
					}
				}
				else if ((*pill.f == -1 && *pill.s != -1) || pill.cut == 1) {
					if (*pill.f == -1 && *pill.s == -1) {
						pill.cut = 3;
					}
					else {
						pill.cut = 1;
						pill.left_color = PillColor::NONE;
						sprite_sheet_of_pill_animation.SelectSprite(static_cast<int>(pill.right_color), 0);
						if (pill.angle == 0.0f || pill.angle == 180.0f)
							p_animation.Draw(Ember::Rect(pill.position.x + grid_size, pill.position.y, grid_size - 3, grid_size - 3), sprite_sheet_of_pill_animation.ReturnSourceRect());
						else
							p_animation.Draw(Ember::Rect(pill.position.x, pill.position.y + grid_size, grid_size - 3, grid_size - 3), sprite_sheet_of_pill_animation.ReturnSourceRect());
					}
				}
				else if ((*pill.f != -1 && *pill.s == -1) || pill.cut == 2) {
					if (*pill.f == -1 && *pill.s == -1) {
						pill.cut = 3;
					}
					else {
						pill.cut = 2;
						pill.right_color = PillColor::NONE;
						sprite_sheet_of_pill_animation.SelectSprite((int)pill.left_color, 0);
						p_animation.Draw(Ember::Rect(pill.position.x, pill.position.y, grid_size - 3, grid_size - 3), sprite_sheet_of_pill_animation.ReturnSourceRect());
					}
				}
			}
		}

		if (virus_animation_delay % 7 == 0) {
			virus_animation_delay = 0;
			virus_frame++;
			if (virus_frame == 2)
				virus_frame = 0;
		}
		for (auto& v : virus) {
			if (*v.f != -1 && !v.dead) {
				sprite_sheet_of_virus_animation.SelectSprite(v.sprite_sheet_location.x + virus_frame, v.sprite_sheet_location.y);
				virus_animation.Draw(v.position, sprite_sheet_of_virus_animation.ReturnSourceRect());
			}
			if (*v.f == -1)
				v.dead = true;
		}

		if(current_frame == 1)
			mario_textures[current_frame]->Draw(Ember::Rect(mario_pos.x - (9 * SCALE), mario_pos.y, mario_textures[current_frame]->GetTextureInfo().x * SCALE, mario_textures[current_frame]->GetTextureInfo().y * SCALE));
		else
			mario_textures[current_frame]->Draw(Ember::Rect(mario_pos.x, mario_pos.y, mario_textures[current_frame]->GetTextureInfo().x * SCALE, mario_textures[current_frame]->GetTextureInfo().y * SCALE));

		if (pill_ready) {
			Ember::IVec2 s = { 0, 0 };
			if (cstart1 == PillColor::RED) {
				if (cstart2 == PillColor::RED)
					s = { 0, 0 };
				else if (cstart2 == PillColor::BLUE)
					s = { 0, 1 };
				else if (cstart2 == PillColor::YELLOW)
					s = { 0, 2 };
			}
			else if (cstart1 == PillColor::BLUE) {
				if (cstart2 == PillColor::RED)
					s = { 1, 1 };
				else if (cstart2 == PillColor::BLUE)
					s = { 1, 0 };
				else if (cstart2 == PillColor::YELLOW)
					s = { 1, 2 };
			}
			if (cstart1 == PillColor::YELLOW) {
				if (cstart2 == PillColor::RED)
					s = { 2, 1 };
				else if (cstart2 == PillColor::BLUE)
					s = { 2, 2 };
				else if (cstart2 == PillColor::YELLOW)
					s = { 2, 0 };
			}
			sprite_sheet_of_pills.SelectSprite(s.x, s.y);
			p_texture.Draw(Ember::Rect(mario_pos.x, inner_bottle.y, SCALE * 15, grid_size - SCALE), sprite_sheet_of_pills.ReturnSourceRect());
		}

		virus_animation_delay++;
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
		else if (keyboard.scancode == SDL_SCANCODE_SPACE && pill_ready) 
			Rotate();
		if (!ready_for_next_pill && pill_ready) {
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
	Ember::Texture bottle;
	
	Ember::Rect bottle_position = { window->Properties()->width / 2 - (bottle.GetTextureInfo().x * SCALE) / 2, window->Properties()->height / 2 - (bottle.GetTextureInfo().y * SCALE) / 2, 240, 528 };
	Ember::IVec2 current_grid_spot = { 0, 0 };
	static const int grid_cols = 8;
	static const int grid_rows = 16;
	static const int grid_size = 8 * SCALE;
	const Ember::Rect inner_bottle = { 288, 192, grid_size * grid_cols, grid_size * grid_rows };

	Ember::Texture* mario_textures[3];

	Ember::AudioMusic main_theme_song;

	int pill_gravity = 0;
	const int pill_gravity_timing = 300;

	bool ready_for_next_pill = false;
	bool left_no = false;
	bool right_no = false;
	bool pill_ready = false;

	PillColor cstart1;
	PillColor cstart2;

	int mario_timing = 0;
	int current_frame = 0;
	bool stop_mario = false;
	bool updating_all_pills_new_pos = false;

	int virus_animation_delay = 0;
	int virus_frame = 0;
	struct Pill {
		PillColor left_color;
		PillColor right_color;

		Ember::IVec2 sprite_sheet_location = { 0, 0 };
		int* f = nullptr;
		int* s = nullptr;
		bool placed = false;
		PillRenderingStyle ren = PillRenderingStyle::FULL_PILL;
		Ember::IVec2 grid_loc = { 0, 0 };
		int cut = 0;
		Ember::Rect position;
		float angle = 0;
		Pill(PillColor color1, PillColor color2, const Ember::IVec2& pos) : left_color(color1), right_color(color2), position(pos.x, pos.y, SCALE * 15, grid_size - SCALE) { }
	};

	struct Virus {
		Ember::IVec2 sprite_sheet_location = { 0, 0 };
		PillColor color;
		Ember::Rect position;
		int* f = nullptr;
		bool dead = false;
		Virus(const Ember::IVec2& pos, PillColor color) : position(pos.x, pos.y, grid_size, grid_size), color(color) { }
	};

	Ember::Texture p_texture;
	Ember::SpriteSheet sprite_sheet_of_pills;

	Ember::Texture p_animation;
	Ember::SpriteSheet sprite_sheet_of_pill_animation;

	Ember::Texture virus_animation;
	Ember::SpriteSheet sprite_sheet_of_virus_animation;
	Ember::IVec2 mario_pos = { 574, 210 };
	std::vector<Pill> pills; 
	std::vector<Virus> virus;
	int pill_grid[grid_cols][grid_rows];
};

int main(int argc, char** argv) 
{
	DrMario mario; 
	mario.Run();

	return 0;
}