#ifndef MCTS_H_
#define	MCTS_H_

#include "Node.h"
#include <random>
#include <string>

class MCTS
{
public:
	MCTS(Puissance4 &ld, float U);
	~MCTS();
	void ResetRoot();
	int RunSimulation(int PlayerID, int count);
	virtual int RunTimeSimulation(int PlayerID, double timeBank);
	virtual std::string PrintName();
	void DebugTree();

	Puissance4 &LD;
protected:
	std::mt19937 randGen;
	Node* root;
	float UcbC;
	void Select(Node* n, int PlayerID);
	void Expand(Node* n, int PlayerID, int move);
	int Simulate(int PlayerID);
};

#endif // MCTS_H_