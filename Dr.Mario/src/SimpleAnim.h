#ifndef SIMPLE_ANIM_H
#define SIMPLE_ANIM_H

#include "Ember/Ember.h"

class SimpleAnim {
public:
	SimpleAnim(int trigger_in_millis);
	SimpleAnim();

	void Initialize(int trigger_in_millis);

	bool Update();
	void ChangeTrigger(int trigger_in_millis);

	void SetFrame(int frame) { current_frame = frame; }
	int GetFrame() const { return current_frame; }
private:
	int trigger_point;
	int current_point;
	int current_frame;

	bool TriggerPointCleared(int trigger_in_millis);
};

struct TextureWithSpriteSheet {
	Ember::Texture texture;
	Ember::SpriteSheet spritesheet;
};

#endif // !SIMPLE_ANIM_H
