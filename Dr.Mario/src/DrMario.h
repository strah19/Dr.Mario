#ifndef DR_MARIO_H
#define DR_MARIO_H

struct Pill;
struct Virus;

#define SCALE 3

#define GRID_SIZE 8 * 3

#define GRID_COLS 8
#define GRID_ROWS 16

using PillGrid = int;

using PillStack = std::vector<Pill>&;
using VirusStack = std::vector<Virus>&;

enum class Speed {
	LOW, MED, HIGH
};

#endif // !DR_MARIO_H
