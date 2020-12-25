#include "Virus.h"
#include "GameMechanics.h"

const int likely_hood_multiplier = 4;

void FillGridWithVirus(PillGrid** grid, VirusStack virus, int level, const Ember::IVec2& inner_bottle) {
	int chance_from_level = (level + 1) * likely_hood_multiplier;
	for (int i = 0; i < GRID_COLS; i++) {
		for (int j = 0; j < GRID_ROWS; j++) {
			int random_chance = rand() % 100;
			if (random_chance < chance_from_level && j > (GRID_ROWS / 2 ) - level / 4) {
				grid[i][j] = rand() % 3;
				virus.push_back(Virus({ (i * GRID_SIZE) + inner_bottle.x, (j * GRID_SIZE) + inner_bottle.y }, static_cast<PillColor>(grid[i][j])));
				virus.back().spritesheet_location = { 0, grid[i][j] };
				virus.back().value = &grid[i][j];
			}
		}
	}

	grid[0][GRID_ROWS / 2] = rand() % 3;
	virus.push_back(Virus({ (0 * GRID_SIZE) + inner_bottle.x, ((GRID_ROWS / 2) * GRID_SIZE) + inner_bottle.y }, static_cast<PillColor>(grid[0][GRID_ROWS / 2])));
	virus.back().spritesheet_location = { 0, grid[0][GRID_ROWS / 2] };
	virus.back().value = &grid[0][GRID_ROWS / 2];
}