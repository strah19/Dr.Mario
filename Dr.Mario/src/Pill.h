#ifndef PILL_H
#define PILL_H

#include "Ember/Ember.h"
#include "DrMario.h"

enum class PillColor {
	RED = 0, BLUE = 1, YELLOW = 2, None = 3
};

enum class PillState {
	UNCUT = 0, LEFTSIDEGONE = 1, RIGHTSIDEGONE = 2, DEADPILL = 3
};

struct Pill {
	PillColor left_color = PillColor::RED;
	PillColor right_color = PillColor::RED;

	Ember::IVec2 spritesheet_location = { 0, 0 };
	Ember::IVec2 grid_location = { 0, 0 };

	bool pill_set = false;

	int* first_value = nullptr;
	int* second_value = nullptr;

	Ember::Rect position = { 0, 0, SCALE * 15, GRID_SIZE - SCALE };
	PillState state = PillState::UNCUT;
	float angle = 0;

	Pill(const Ember::IVec2& position, PillColor right_color, PillColor left_color) {
		this->position.pos = position;

		this->left_color = left_color;
		this->right_color = right_color;
	}
	
	void UpdateGridLocation(const Ember::IVec2& inner_bottle_ref, PillGrid** grid);
};

bool PillIsUpright(Pill& pill);
bool CheckForIntersect(Pill& pill, PillGrid** grid);
bool LeftSide(Pill& pill, PillGrid** grid);
bool RightSide(Pill& pill, PillGrid** grid);

enum class Direction {
	Left, Right, Up, Down
};

void CheckForInARow(PillGrid** grid);
void PrintPillData(Pill& pill);

Ember::IVec2 CalculateSpriteLocation(PillColor c1, PillColor c2);
void AddPill(PillStack pills, const Ember::IVec2& pos, PillColor c1, PillColor c2);
void CheckOverlap(Pill& pill, PillGrid** grid);
void UpdatePillFromPointer(Pill& pill, int& half, int& whole);
void GoThroughOnePosition(const Ember::IVec2& pos, PillGrid** grid, Direction direction, int range);

#endif // !PILL_H
