#include "SimpleAnim.h"

SimpleAnim::SimpleAnim(int trigger_in_millis) {
	Initialize(trigger_in_millis);
}

SimpleAnim::SimpleAnim() 
	: trigger_point(1), current_point(0), current_frame(0) {
}

void SimpleAnim::Initialize(int trigger_in_millis) {
	current_point = 0;
	current_frame = 0;
	if (TriggerPointCleared(trigger_in_millis))
		trigger_point = trigger_in_millis;
}

bool SimpleAnim::Update() {
	if (SDL_GetTicks() - current_point >= trigger_point) {
		current_point = SDL_GetTicks();
		current_frame++;
		return true;
	}
	return false;
}

void SimpleAnim::ChangeTrigger(int trigger_in_millis) {
	if (TriggerPointCleared(trigger_in_millis))
		trigger_point = trigger_in_millis;
}

bool SimpleAnim::TriggerPointCleared(int trigger_in_millis) {
	return (trigger_in_millis > 0);
}