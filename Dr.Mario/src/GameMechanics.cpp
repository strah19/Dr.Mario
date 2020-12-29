#include "GameMechanics.h"

GamePlay::GamePlay(Ember::rRenderer* renderer) {
	un_cut_pill.texture.Initialize(uncut_pills_path, renderer);
	small_virus_animation.texture.Initialize(small_virus_path, renderer);
	cut_pill_animation.texture.Initialize(cut_pill_path, renderer);

	un_cut_pill.spritesheet.Initialize(un_cut_pill.texture, 3, 3);

	small_virus_animation.spritesheet.Initialize(small_virus_animation.texture, 3, 2);
	small_v_anim.Initialize(anim_speed);

	cut_pill_animation.spritesheet.Initialize(cut_pill_animation.texture, 2, 3);

	grid = new PillGrid*[GRID_COLS];
	for (int i = 0; i < GRID_COLS; ++i)
		grid[i] = new PillGrid[GRID_ROWS];
	EmptyGrid();
}

void GamePlay::CleanUp() {
	for (int i = 0; i < GRID_COLS; ++i)
		delete[] grid[i];
	delete[] grid;
}

void GamePlay::Initialize(int starting_level, Speed speed) {
	virus.clear();
	pills.clear();
	EmptyGrid();
	FillGridWithVirus(grid, virus, starting_level, inner_bottle.pos);
	virus_count = virus.size();

	switch (speed) {
	case Speed::LOW: {
		pill_gravity = 350;
		break;
	}
	case Speed::MED: {
		pill_gravity = 300;
		break;
	}
	case Speed::HIGH: {
		pill_gravity = 250;
		break;
	}
	default:
		break;
	}
	pill_drop.Initialize(pill_gravity);
}

void GamePlay::EmptyGrid() {
	for (int i = 0; i < GRID_COLS; i++) {
		for (int j = 0; j < GRID_ROWS; j++) {
			grid[i][j] = -1;
		}
	}
}

void PrintGrid(PillGrid** grid) {
	system("CLS");

	for (int i = 0; i < GRID_ROWS; i++) {
		std::cout << std::endl;
		for (int j = 0; j < GRID_COLS; j++) 
			std::cout << grid[j][i];
	}
}

void GamePlay::LMovement() {
	if (GetActivity() && !stop_left_movement && !stop_gravity)
		pills.back().position.x -= GRID_SIZE;
}

void GamePlay::RMovement() {
	if (GetActivity() && !stop_right_movement && !stop_gravity)
		pills.back().position.x += GRID_SIZE;
}

void GamePlay::DownMovement() {
	if (GetActivity() && !stop_gravity) {
		pills.back().position.y += GRID_SIZE;
		wait_one_cycle_before_collision = -1;
	}
	else
		wait_one_cycle_before_collision = 0;
}

void GamePlay::Rotate() {
	if (GetActivity() && !stop_gravity) {
		if (pills.back().angle == 270.0f)
			pills.back().angle = 0.0f;
		else
			pills.back().angle += 90.0f;

		int w = pills.back().position.w;
		pills.back().position.w = pills.back().position.h;
		pills.back().position.h = w;

		if (pills.back().grid_location.x == GRID_COLS - 1)
			pills.back().position.x -= GRID_SIZE;

		if (pills.back().angle == 180.0f) {
			PillColor c = pills.back().left_color;
			pills.back().left_color = pills.back().right_color;
			pills.back().right_color = c;
			pills.back().position.y += GRID_SIZE;
		}
		if (pills.back().angle == 0.0f) {
			PillColor c = pills.back().left_color;
			pills.back().left_color = pills.back().right_color;
			pills.back().right_color = c;
			pills.back().position.y += GRID_SIZE;
		}
		else {
			pills.back().position.y -= GRID_SIZE;
		}
	}
}

void GamePlay::Update() {
	if (pill_drop.Update()) {
		if (between_pills && recheck_between_pills > 0 ) {
			int pills_set = 0;
			for (int i = 0; i < pills.size(); i++) {
				if (pills[i].angle == 0.0f || pills[i].angle == 180.0f) {
					if (pills[i].state == PillState::LEFTSIDEGONE) {
						if (grid[pills[i].grid_location.x + 1][pills[i].grid_location.y + 1] == -1 && pills[i].grid_location.y < 15) {
							pills[i].first_value = &grid[pills[i].grid_location.x + 1][pills[i].grid_location.y + 1];
							pills[i].second_value = &grid[pills[i].grid_location.x + 1][pills[i].grid_location.y + 1];

							grid[pills[i].grid_location.x + 1][pills[i].grid_location.y] = -1;

							grid[pills[i].grid_location.x + 1][pills[i].grid_location.y + 1] = (int)pills[i].right_color;

							pills[i].position.y += GRID_SIZE;
							pills[i].grid_location.y += 1;
						}
						else
							pills_set++;
					}
					else if (pills[i].state == PillState::RIGHTSIDEGONE) {
						if (grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] == -1 && pills[i].grid_location.y < 15) {
							pills[i].first_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 1];
							pills[i].second_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 1];

							grid[pills[i].grid_location.x][pills[i].grid_location.y] = -1;

							grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] = (int)pills[i].left_color;

							pills[i].position.y += GRID_SIZE;
							pills[i].grid_location.y += 1;
						}
						else
							pills_set++;
					}
					else if (pills[i].state == PillState::UNCUT) {
						if (grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] == -1 && grid[pills[i].grid_location.x + 1][pills[i].grid_location.y + 1] == -1 && pills[i].grid_location.y < 15) {
							pills[i].first_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 1];
							pills[i].second_value = &grid[pills[i].grid_location.x + 1][pills[i].grid_location.y + 1];

							grid[pills[i].grid_location.x][pills[i].grid_location.y] = -1;
							grid[pills[i].grid_location.x + 1][pills[i].grid_location.y] = -1;

							grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] = (int)pills[i].left_color;
							grid[pills[i].grid_location.x + 1][pills[i].grid_location.y + 1] = (int)pills[i].right_color;

							pills[i].position.y += GRID_SIZE;
							pills[i].grid_location.y += 1;
						}
						else
							pills_set++;
					}
					else
						pills_set++;
				}
				else if (pills[i].angle == 90.0f || pills[i].angle == 270.0f) {
					if (pills[i].state == PillState::LEFTSIDEGONE) {
						if (grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] == -1 && pills[i].grid_location.y < 15) {
							pills[i].first_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 1];
							pills[i].second_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 1];

							grid[pills[i].grid_location.x][pills[i].grid_location.y] = -1;

							grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] = (int)pills[i].right_color;

							pills[i].position.y += GRID_SIZE;
							pills[i].grid_location.y += 1;
						}
						else
							pills_set++;
					}
					else if (pills[i].state == PillState::RIGHTSIDEGONE) {
						if (grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] == -1 && pills[i].grid_location.y < 15) {
							pills[i].first_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 1];
							pills[i].second_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 1];

							grid[pills[i].grid_location.x][pills[i].grid_location.y] = -1;

							grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] = (int)pills[i].left_color;

							pills[i].position.y += GRID_SIZE;
							pills[i].grid_location.y += 1;
						}
						else
							pills_set++;
					}
					else if (pills[i].state == PillState::UNCUT) {
						if (grid[pills[i].grid_location.x][pills[i].grid_location.y + 2] == -1 && pills[i].grid_location.y + 1 < 15) {
							pills[i].first_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 1];
							pills[i].second_value = &grid[pills[i].grid_location.x][pills[i].grid_location.y + 2];

							grid[pills[i].grid_location.x][pills[i].grid_location.y] = -1;
							grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] = -1;

							grid[pills[i].grid_location.x][pills[i].grid_location.y + 1] = (int)pills[i].left_color;
							grid[pills[i].grid_location.x][pills[i].grid_location.y + 2] = (int)pills[i].right_color;

							pills[i].position.y += GRID_SIZE;
							pills[i].grid_location.y += 1;
						}
						else
							pills_set++;
					}
					else
						pills_set++;
				}
				else
					pills_set++;
			}
			if (pills_set == pills.size()) {
				CheckForInARow(grid);
				if (recheck_between_pills == 2) {
					between_pills = false;
					user_to_pill_mechanic_activity = false;
					ready_for_pill = true;
					stop_gravity = false;
					wait_one_cycle_before_collision = 0;
					anim_stage_1 = false;

					stop_left_movement = false;
					stop_right_movement = false;
					recheck_between_pills = 0;
				}
				recheck_between_pills = 2;
			}
			else
				recheck_between_pills = 1;
		}
		else if (wait_one_cycle_before_collision == 0 && !stop_gravity) {
			pills.back().position.y += GRID_SIZE;
		}
	}

	pills.back().UpdateGridLocation(inner_bottle.pos, grid);

	stop_left_movement = LeftSide(pills.back(), grid);
	stop_right_movement = RightSide(pills.back(), grid);

	if (CheckForIntersect(pills.back(), grid) && !stop_gravity) {
		if (wait_one_cycle_before_collision == 25 || wait_one_cycle_before_collision == -1) {
			CheckOverlap(pills.back(), grid);
			PushPillValuesToGrid();
			pills.back().pill_set = true;

			CheckForInARow(grid);
			UpdatePillFromPointer(pills.back(), num_of_dead_half, num_of_dead_pills);

			stop_gravity = true;
			between_pills = true;
			wait_one_cycle_before_collision = -2;
			recheck_between_pills = 1;

			stop_left_movement = true;
			stop_right_movement = true;
			anim_stage_1 = true;
		}
		wait_one_cycle_before_collision++;
	}
	else
		wait_one_cycle_before_collision = 0;
}

void GamePlay::CalculateScore(int& score) {
	score = (num_of_dead_half * 25) + (num_of_dead_pills * 50) + (num_of_dead_virus * 100);
}

void GamePlay::PushPillValuesToGrid() {
	if (pills.back().angle == 0.0f || pills.back().angle == 180.0f) {
		grid[pills.back().grid_location.x][pills.back().grid_location.y] = static_cast<int>(pills.back().left_color);
		grid[pills.back().grid_location.x + 1][pills.back().grid_location.y] = static_cast<int>(pills.back().right_color);
	}
	else {
		grid[pills.back().grid_location.x][pills.back().grid_location.y] = static_cast<int>(pills.back().left_color);
		grid[pills.back().grid_location.x][pills.back().grid_location.y + 1] = static_cast<int>(pills.back().right_color);
	}
}

void GamePlay::NextPill(const Ember::IVec2& mario_pos) {
	AddPill(pills, { mario_pos.x, inner_bottle.y }, foreshadow_c1, foreshadow_c2);
	ready_for_pill = false;
	wait_one_cycle_before_collision = 0;
	virus_dead_color = PillColor::None;
}

int GamePlay::DrawVirus() {
	if (small_v_anim.Update()) {
		if (small_v_anim.GetFrame() == 3)
			small_v_anim.SetFrame(1);
	}

	for (auto& v : virus) {
		if (v.value != nullptr) {
			if (*v.value != -1 && !v.dead) {
				small_virus_animation.spritesheet.SelectSprite(v.spritesheet_location.x + small_v_anim.GetFrame() - 1, v.spritesheet_location.y);
				small_virus_animation.texture.Draw(v.position, small_virus_animation.spritesheet.ReturnSourceRect());
			}
			else if (*v.value == -1 && !v.dead) {
				virus_dead_color = v.color;
				v.dead = true;
				v.value = nullptr;
				num_of_dead_virus++;
				virus_count--;
			}
		}
	}

	return virus_count;
}

void GamePlay::DrawPills() {
	for (auto& pill : pills) {
		un_cut_pill.spritesheet.SelectSprite(pill.spritesheet_location.x, pill.spritesheet_location.y);
		if (!pill.pill_set) {
			if (pill.angle == 90.0f || pill.angle == 270.0f) {
				un_cut_pill.texture.Draw(Ember::Rect(pill.position.x - (GRID_SIZE / 2), pill.position.y + GRID_SIZE / 2, pill.position.h, pill.position.w), un_cut_pill.spritesheet.ReturnSourceRect(),
					SDL_FLIP_NONE, pill.angle);
			}
			else {
				un_cut_pill.texture.Draw(pill.position, un_cut_pill.spritesheet.ReturnSourceRect(), SDL_FLIP_NONE, pill.angle);
			}
		}
		else if (pill.pill_set) {
			UpdatePillFromPointer(pill, num_of_dead_half, num_of_dead_pills);

			if (pill.state == PillState::UNCUT) {
				if (*pill.first_value == -1 && *pill.second_value == -1 && pill.state != PillState::DEADPILL) {
					pill.state = PillState::DEADPILL;
				}
				if (pill.angle == 90.0f || pill.angle == 270.0f) {
					un_cut_pill.texture.Draw(Ember::Rect(pill.position.x - (GRID_SIZE / 2), pill.position.y + GRID_SIZE / 2, pill.position.h, pill.position.w), un_cut_pill.spritesheet.ReturnSourceRect(),
						SDL_FLIP_NONE, pill.angle);
				}
				else {
					un_cut_pill.texture.Draw(pill.position, un_cut_pill.spritesheet.ReturnSourceRect(), SDL_FLIP_NONE, pill.angle);
				}
			}
			else if (pill.state == PillState::LEFTSIDEGONE) {
				cut_pill_animation.spritesheet.SelectSprite(static_cast<int>(pill.right_color), 0);
				if (*pill.first_value == -1 && *pill.second_value == -1 && pill.state != PillState::DEADPILL) {
					pill.state = PillState::DEADPILL;
				}
				if (pill.angle == 0.0f || pill.angle == 180.0f)
					cut_pill_animation.texture.Draw(Ember::Rect(pill.position.x + GRID_SIZE, pill.position.y, GRID_SIZE - 3, GRID_SIZE - 3), cut_pill_animation.spritesheet.ReturnSourceRect());
				else
					cut_pill_animation.texture.Draw(Ember::Rect(pill.position.x, pill.position.y + GRID_SIZE, GRID_SIZE - 3, GRID_SIZE - 3), cut_pill_animation.spritesheet.ReturnSourceRect());
			}
			else if (pill.state == PillState::RIGHTSIDEGONE && pill.state != PillState::DEADPILL) {
				if (*pill.first_value == -1 && *pill.second_value == -1) {
					pill.state = PillState::DEADPILL;
				}
				else {
					cut_pill_animation.spritesheet.SelectSprite((int)pill.left_color, 0);
					cut_pill_animation.texture.Draw(Ember::Rect(pill.position.x, pill.position.y, GRID_SIZE - 3, GRID_SIZE - 3), cut_pill_animation.spritesheet.ReturnSourceRect());
				}
			}
		}
	}
}

bool GamePlay::PillAnim() {
	if (pills.back().position.x == inner_bottle.x + (inner_bottle.w / 2) - (pills.back().position.w / 2)) {
		anim_stage_1 = true;
	}
	if (!anim_stage_1) {
		pills.back().position.y -= 3;
		pills.back().position.x -= 4;
	}

	if (anim_stage_1) {
		pills.back().position.y += 3;
		pills.back().angle += 17;
	}

	if (pills.back().position.y == inner_bottle.y - GRID_SIZE && anim_stage_1) {
		user_to_pill_mechanic_activity = true;
		pills.back().angle = 0;

		foreshadow_c1 = static_cast<PillColor>(rand() % 3);
		foreshadow_c2 = static_cast<PillColor>(rand() % 3);

		return true;
	}

	return false;
}

void GamePlay::ForeshadowPill(const Ember::IVec2& foreshadow_loc) {
	Ember::IVec2 sprite_loc = CalculateSpriteLocation(foreshadow_c1, foreshadow_c2);
	un_cut_pill.spritesheet.SelectSprite(sprite_loc.x, sprite_loc.y);
	un_cut_pill.texture.Draw(Ember::Rect(foreshadow_loc.x, foreshadow_loc.y - GRID_SIZE, SCALE * 15, GRID_SIZE - SCALE), un_cut_pill.spritesheet.ReturnSourceRect(), SDL_FLIP_NONE);
}