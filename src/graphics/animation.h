#ifndef ANIMATION_H
#define ANIMATION_H

class Animation {
public:
	bool running = false;
	bool finished = true;
	bool inverseMove = false;
	int targetSide = 0;
	int axis = 0;
	float animationStartTime = 0.0f;
	float duration = 0.0f;
	float animationStep = 0.1f;

	Animation(bool running);

	void updateAnimation(float currentTime);
	void doAnimation(int newTarget, float animationDuration, int axis, bool inverseMove, float startTime);
};

#endif
