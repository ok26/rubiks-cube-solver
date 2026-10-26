#include "generator/kociembaTables.h"
#include "generator/tableVerifier.h"
#include "solver/kociemba/moveController.h"

#include <cstring>
#include <iostream>
#include <memory>

int main(int argc, char** argv) {
	bool phase1 = argc == 1, phase2 = argc == 1, verify = false;
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "phase1"))
			phase1 = true;
		else if (!strcmp(argv[i], "phase2"))
			phase2 = true;
		else if (!strcmp(argv[i], "verify"))
			verify = true;
		else {
			std::cout << "Usage: generate-tables [phase1] [phase2] [verify]\n"
				<< "Writes the pruning tables to resources/data/ (both phases if no argument is given)\n";
			return 1;
		}
	}

	auto moveController = std::make_unique<MoveController>();
	moveController->initializeTables();

	if (phase2)
		generatePhase2Tables(*moveController);
	if (phase1)
		generatePhase1Tables(*moveController);
	if (verify)
		return verifyTables(*moveController) ? 0 : 1;
	return 0;
}
