#ifndef SIMPLIFY_SOLUTION_H
#define SIMPLIFY_SOLUTION_H

#include <vector>

class SolutionSimplifier {
public:
	void updateSolution(std::vector<int>& solution, int index, int move, int lastMove);
	void simplifySolution(std::vector<int>& solution);
	int sameMoveReductionDiff(int move1, int move2);
	int reductionDifference(int move1, int move2);
};

#endif
