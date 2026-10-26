#include "graphics/animation.h"

#include <cmath>

Animation::Animation(bool running) {
	this->running = running;
	if (running)
		finished = false;
}

void Animation::updateAnimation(float currentTime) {
	if (!running)
		return;

	animationStep = (currentTime - animationStartTime) / duration;
	animationStep = sqrtf(animationStep);
	if (animationStep > 1.0f) {
		running = false;
		finished = true;
	}
}

void Animation::doAnimation(int newTarget, float animationDuration, int axis, bool inverseMove, float startTime) {
	if (running)
		return;

	targetSide = newTarget;
	this->axis = axis;
	this->inverseMove = inverseMove;
	animationStartTime = startTime;
	duration = animationDuration;
	running = true;
	finished = false;
	animationStep = 0.0f;
}
