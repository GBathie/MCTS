#ifndef NOTREEGMCTS_H_
#define	NOTREEGMCTS_H_

#include "MCTS.h"

// Greedy Simulation with no tree building
class NoTreeGMCTS : public MCTS
{
public:
	NoTreeGMCTS(Puissance4 &ld, float U);
	int RunSimulation(int PlayerID, int count);
	int RunTimeSimulation(int PlayerID, double timeBank) override;
	std::string PrintName() override;
protected:	
	void Select(Node* n, int PlayerID);
	void Expand(Node* n, int PlayerID, int move);
	int Simulate(int PlayerID);
};

#endif // NOTREEGMCTS_H_