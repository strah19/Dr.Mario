#ifndef GAME_MECHANICS_H
#define GAME_MECHANICS_H

#include "Ember/Ember.h"
#include "SimpleAnim.h"
#include "DrMario.h"

#include "Pill.h"
#include "Virus.h"

void PrintGrid(PillGrid** grid);
std::string GetStringFromSpeed(Speed speed);

class GamePlay {
public:
	GamePlay(Ember::rRenderer* renderer);

	void Initialize(int starting_level, Speed speed);
	void CleanUp();
	void Update();

	void LMovement();
	void RMovement();
	void DownMovement();

	void Rotate();

	int DrawVirus();
	void DrawPills();

	bool GetActivity() { return user_to_pill_mechanic_activity; }
	bool PillReady() { return ready_for_pill; }

	void NextPill(const Ember::IVec2& mario_pos);

	bool PillAnim();
	void ForeshadowPill(const Ember::IVec2& foreshadow_loc);

	void Freeze() {
		stop_gravity = true;
		between_pills = true;
		wait_one_cycle_before_collision = -2;
		recheck_between_pills = 1;

		stop_left_movement = true;
		stop_right_movement = true;
	}

	void ReStart() {
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

	bool DidLoose() {
		if(!pills.empty())
			if (pills.back().grid_location == Ember::IVec2({ 3, 0 }) || pills.back().grid_location == Ember::IVec2({ 4, 0 }))
				return true;
		return false;
	}

	void ResetScore() {
		num_of_dead_virus = 0;
		num_of_dead_pills = 0;
		num_of_dead_half = 0;
	}

	void CalculateScore(int& score);
	PillColor GetVirus() { return virus_dead_color; }
private:
	bool stop_left_movement = false;
	bool stop_right_movement = false;
	bool between_pills = false;
	bool stop_gravity = false;
	int recheck_between_pills = 0;
	int virus_count = 0;

	int num_of_dead_virus = 0;
	int num_of_dead_pills = 0;
	int num_of_dead_half = 0;

	PillGrid** grid;
	const Ember::Rect inner_bottle = { 288, 192, GRID_SIZE * GRID_COLS, GRID_SIZE * GRID_COLS };

	void EmptyGrid();
	void PushPillValuesToGrid();

	const std::string uncut_pills_path = "res/textures/un_animated_pills.png";
	const std::string small_virus_path = "res/textures/virus's.png";
	const std::string cut_pill_path = "res/textures/pill_animations.png";
	const int anim_speed = 150;

	TextureWithSpriteSheet un_cut_pill;

	SimpleAnim small_v_anim;
	TextureWithSpriteSheet small_virus_animation;

	TextureWithSpriteSheet cut_pill_animation;

	std::vector<Pill> pills;
	std::vector<Virus> virus;

	bool user_to_pill_mechanic_activity = false;
	bool ready_for_pill = true;
	bool anim_stage_1 = false;
	int wait_one_cycle_before_collision = 0;

	int pill_gravity = 0;
	SimpleAnim pill_drop;

	PillColor virus_dead_color = PillColor::None;

	PillColor foreshadow_c1 = PillColor::RED;
	PillColor foreshadow_c2 = PillColor::RED;
};


#endif // !GAME_MECHANICS_H
