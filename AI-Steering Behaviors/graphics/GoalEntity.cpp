#include "GoalEntity.h"
#include <GLFW/glfw3.h>

GoalEntity::GoalEntity() :circle(vec3(7.0f, 1.0f, 7.0f)), start(time(0)), flock_started(false)
{
	
}


GoalEntity::~GoalEntity()
{
}


void GoalEntity::Update(float dt)
{

	if (MOVE_GOAL)
	{

		double seconds_since_start = difftime(time(0), start);
		if (seconds_since_start > 5 && !flock_started)
		{
			flock_started = true;
		}
		if (seconds_since_start > 6.5 && flock_started)
		{
			//Position.x = (rand() % 40) + (-20);
			//Position.y = (rand() % 5) + 3;
			//Position.z = (rand() % 40) + (-20);
			start = time(0);
		}
		else if (!flock_started)
		{
			Position.x = circle.x * sin(glfwGetTime()*0.5);
			Position.z = circle.z  * cosf(glfwGetTime()*0.5);
		}
	}
	
}

