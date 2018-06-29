#ifndef HMCTS_H_
#define	HMCTS_H_

#include "MCTS.h"

class HMCTS : public MCTS
{
public:
	HMCTS(Puissance4 &ld, float U);
	HMCTS(Puissance4 &ld, float U, float temp);
	int RunSimulation(int PlayerID, int count);
	int RunTimeSimulation(int PlayerID, double timeBank) override;
	std::string PrintName() override;
protected:	
	float temp;
	void Select(Node* n, int PlayerID);
	void Expand(Node* n, int PlayerID, int move);
	int Simulate(int PlayerID);
	int MoveFromProbas(int PlayerID);
};

#endif // HMCTS_H_
