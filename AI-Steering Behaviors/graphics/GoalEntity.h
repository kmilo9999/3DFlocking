#pragma once
#include "Entity.h"
#include <time.h>

#define MOVE_GOAL true

class GoalEntity: public Entity
{

public:
	GoalEntity();
	~GoalEntity();
	void Update(float dt) override;


private:
	vec3 circle;
	time_t start;
	bool flock_started;
};

