#include "Pill.h"

void Pill::UpdateGridLocation(const Ember::IVec2& inner_bottle_ref, PillGrid** grid) {
	grid_location = { ((position.x - inner_bottle_ref.x) / GRID_SIZE) / 9,((position.y - inner_bottle_ref.y) / GRID_SIZE) / 9 };
	first_value = &grid[grid_location.x][grid_location.y];

	if (PillIsUpright(*this))
		second_value = &grid[grid_location.x][grid_location.y + 1];
	else
		second_value = &grid[grid_location.x + 1][grid_location.y];
}

void PrintPillData(Pill& pill) {
	system("CLS");

	std::cout << "Pill Data" << std::endl;
	std::cout << "------------------------" << std::endl;
	std::cout << "Pill Angle: " << pill.angle << std::endl;
	std::cout << "Pill Position: " << pill.position.pos << std::endl;
	std::cout << "Pill Size: " << pill.position.size << std::endl;
	std::cout << "Pill Grid Location: " << pill.grid_location << std::endl;
	if (pill.first_value != nullptr)
		std::cout << "First Value: " << *pill.first_value << std::endl;
	if (pill.second_value != nullptr)
		std::cout << "First Value: " << *pill.second_value << std::endl;
	std::cout << "Pill Sprite Location: " << pill.spritesheet_location << std::endl;
	std::cout << "Left Color: " << static_cast<int>(pill.left_color) << std::endl;
	std::cout << "Right Color: " << static_cast<int>(pill.right_color) << std::endl;
	std::cout << "Pill State: " << static_cast<int>(pill.state) << std::endl;
}

bool PillIsUpright(Pill& pill) {
	return (pill.angle == 90.0f || pill.angle == 270.0f);
}

bool CheckForIntersect(Pill& pill, PillGrid** grid) {
	if (PillIsUpright(pill)) {
		if (grid[pill.grid_location.x][pill.grid_location.y + 2] != -1)
			return true;
	}
	else {
		if (grid[pill.grid_location.x][pill.grid_location.y + 1] != -1 || grid[pill.grid_location.x + 1][pill.grid_location.y + 1] != -1)
			return true;
	}
	return false;
}

bool LeftSide(Pill& pill, PillGrid** grid) {
	if (pill.grid_location.x != 0) {
		if (PillIsUpright(pill)) {
			if (grid[pill.grid_location.x - 1][pill.grid_location.y] != -1 || grid[pill.grid_location.x - 1][pill.grid_location.y + 1] != -1)
				return true;
		}
		else {
			if (grid[pill.grid_location.x - 1][pill.grid_location.y] != -1)
				return true;
		}
	}

	return (pill.grid_location.x == 0);
}

bool RightSide(Pill& pill, PillGrid** grid) {
	if (PillIsUpright(pill)) {
		if (pill.grid_location.x != 7) {
			if (grid[pill.grid_location.x + 1][pill.grid_location.y] != -1 || grid[pill.grid_location.x + 1][pill.grid_location.y + 1] != -1)
				return true;
		}
		return (pill.grid_location.x == 7);
	}
	else {
		if (pill.grid_location.x != 6) {
			if (grid[pill.grid_location.x + 2][pill.grid_location.y] != -1)
				return true;
		}
		return (pill.grid_location.x == 6);
	}
}

void CheckOverlap(Pill& pill, PillGrid** grid) {
	if (PillIsUpright(pill)) {
		if (*pill.first_value != -1 || *pill.second_value != -1) {
			pill.first_value = &grid[pill.grid_location.x][pill.grid_location.y - 1];
			pill.second_value = &grid[pill.grid_location.x][pill.grid_location.y - 2];

			pill.grid_location.y -= 1;
			pill.position.y -= GRID_SIZE;
		}
	}
	else {
		if (*pill.first_value != -1 || *pill.second_value != -1) {
			pill.first_value = &grid[pill.grid_location.x][pill.grid_location.y - 1];
			pill.second_value = &grid[pill.grid_location.x + 1][pill.grid_location.y - 1];

			pill.grid_location.y -= 1;
			pill.position.y -= GRID_SIZE;
		}
	}
}

Ember::IVec2 CalculateSpriteLocation(PillColor c1, PillColor c2) {
	Ember::IVec2 sprite_loc = { 0, 0 };
	if (c1 == PillColor::RED) {
		if (c2 == PillColor::RED)
			sprite_loc = { 0, 0 };
		else if (c2 == PillColor::BLUE)
			sprite_loc = { 0, 1 };
		else if (c2 == PillColor::YELLOW)
			sprite_loc = { 0, 2 };
	}
	else if (c1 == PillColor::BLUE) {
		if (c2 == PillColor::RED)
			sprite_loc = { 1, 1 };
		else if (c2 == PillColor::BLUE)
			sprite_loc = { 1, 0 };
		else if (c2 == PillColor::YELLOW)
			sprite_loc = { 1, 2 };
	}
	if (c1 == PillColor::YELLOW) {
		if (c2 == PillColor::RED)
			sprite_loc = { 2, 1 };
		else if (c2 == PillColor::BLUE)
			sprite_loc = { 2, 2 };
		else if (c2 == PillColor::YELLOW)
			sprite_loc = { 2, 0 };
	}

	return sprite_loc;
}

void CheckForInARow(PillGrid** grid) {
	for (int i = 0; i < GRID_COLS; i++) {
		for (int j = 0; j < GRID_ROWS; j++) {
			GoThroughOnePosition({ i, j }, grid, Direction::Left, 5);
			GoThroughOnePosition({ i, j }, grid, Direction::Right, 5);
			GoThroughOnePosition({ i, j }, grid, Direction::Up, 5);
			GoThroughOnePosition({ i, j }, grid, Direction::Down, 5);
		}
	}

}

void GoThroughOnePosition(const Ember::IVec2& pos, PillGrid** grid, Direction direction, int range) {
	int left_counter = 0;
	int right_counter = 0;
	int down_counter = 0;
	int up_counter = 0;
	for (int i = 0; i < range; i++) {
		if (direction == Direction::Left && pos.x - i > 0) {
			if (grid[pos.x - i][pos.y] == grid[pos.x][pos.y]) {
				left_counter++;
			}
			else
				break;
		}
		else if (direction == Direction::Right && pos.x + i < GRID_COLS) {
			if (grid[pos.x + i][pos.y] == grid[pos.x][pos.y]) {
				right_counter++;
			}
			else
				break;
		}
		else if (direction == Direction::Up && pos.y - i > 0) {
			if (grid[pos.x][pos.y - i] == grid[pos.x][pos.y]) {
				up_counter++;
			}
			else
				break;
		}
		else if (direction == Direction::Down && pos.y && pos.y + i < GRID_ROWS) {
			if (grid[pos.x][pos.y + i] == grid[pos.x][pos.y]) {
				down_counter++;
			}
			else
				break;
		}
	}

	if (direction == Direction::Left) {
		if (left_counter >= 4) {
			for (int j = 0; j < left_counter; j++) {
				grid[pos.x - j][pos.y] = -1;
			}
		}
	}
	else if (direction == Direction::Right) {
		if (right_counter >= 4) {
			for (int j = 0; j < right_counter; j++) {
				grid[pos.x + j][pos.y] = -1;
			}
		}
	}
	else if (direction == Direction::Up) {
		if (up_counter >= 4) {
			for (int j = 0; j < up_counter; j++) {
				grid[pos.x][pos.y - j] = -1;
			}
		}
	}
	else if (direction == Direction::Down) {
		if (down_counter >= 4) {
			for (int j = 0; j < down_counter; j++) {
				grid[pos.x][pos.y + j] = -1;
			}
		}
	}
}

void AddPill(PillStack pills, const Ember::IVec2& pos, PillColor c1, PillColor c2) {
	pills.push_back(Pill(pos, c1, c2));
	pills.back().spritesheet_location = CalculateSpriteLocation(c1, c2);
	PillColor c = pills.back().left_color;
	pills.back().left_color = pills.back().right_color;
	pills.back().right_color = c;
}

void UpdatePillFromPointer(Pill& pill, int& half, int& whole) {
	if (pill.state == PillState::UNCUT) {
		if (*pill.first_value == -1 && *pill.second_value != -1) {
			pill.first_value = pill.second_value;
			pill.state = PillState::LEFTSIDEGONE;
			half++;
		}
		else if (*pill.first_value != -1 && *pill.second_value == -1) {
			pill.second_value = pill.first_value;
			pill.state = PillState::RIGHTSIDEGONE;
			half++;
		}
	}
	if (*pill.first_value == -1 && *pill.second_value == -1 && pill.state == PillState::UNCUT) {
		whole++;
		pill.state = PillState::DEADPILL;
	}
	if (*pill.first_value == -1 && *pill.second_value == -1 && pill.state != PillState::DEADPILL) {
		pill.state = PillState::DEADPILL;
	}
}