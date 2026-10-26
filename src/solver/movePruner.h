#ifndef MOVE_PRUNER_H
#define MOVE_PRUNER_H

class MovePruner {
public:
	MovePruner();

	bool pruneMove(int move, int lastMove);
	bool pruneSolutionDerived(int move, bool solutionDerived);
};

#endif // !MOVE_PRUNER_H
