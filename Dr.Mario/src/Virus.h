#ifndef VIRUS_H
#define VIRUS_H

#include "Ember/Ember.h"

#include "DrMario.h"
#include "Pill.h"

struct Virus {
	Ember::IVec2 spritesheet_location = { 0, 0 };
	int* value = nullptr;

	PillColor color;
	bool dead = false;

	Ember::Rect position = { 0,0, GRID_SIZE, GRID_SIZE };

	Virus(const Ember::IVec2& position, PillColor color) {
		this->position.pos = position;

		this->color = color;
	}
};

void FillGridWithVirus(PillGrid** grid, VirusStack virus, int level, const Ember::IVec2& inner_bottle);

#endif // !VIRUS_H
