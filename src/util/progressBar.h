#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H

class ProgressBar {

	int barWidth = 70;
public:
	ProgressBar(int barWidth);

	void updateProgressBar(float progress);
};

#endif
