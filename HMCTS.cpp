#include "HMCTS.h"
#include <chrono>
#define SQRT2 1.4142
#define children_size 7


HMCTS::HMCTS(Puissance4 &ld, float U) : MCTS(ld, U) { temp = 0;}
HMCTS::HMCTS(Puissance4 &ld, float U, float tmp) : MCTS(ld, U) 
{
	temp = tmp;
}

int HMCTS::RunSimulation(int PlayerID, int count)
{
	uint64_t boardP1 = LD.Board[0];
	uint64_t boardP2 = LD.Board[1];
	int Round = LD.Round;
	int CurrentPlayer = LD.CurrentPlayer;
	for (int i = 0; i < count; ++i)
	{
		Select(root, PlayerID);
		LD.SetState(boardP1, boardP2, Round, CurrentPlayer);
	}
	return root->GetBest()->move;
}

int HMCTS::RunTimeSimulation(int PlayerID, double timeBank)
{
	uint64_t boardP1 = LD.Board[0];
	uint64_t boardP2 = LD.Board[1];
	int Round = LD.Round;
	int CurrentPlayer = LD.CurrentPlayer;

	auto start = std::chrono::high_resolution_clock::now();	
	std::chrono::duration<double, std::milli> elapsed =  std::chrono::high_resolution_clock::now() - start;
	int i=0;
	while (elapsed.count() < timeBank)
	{
		Select(root, PlayerID);
		LD.SetState(boardP1, boardP2, Round, CurrentPlayer);
		elapsed =  std::chrono::high_resolution_clock::now() - start;
		++i;
	}
	return root->GetBest()->move;
}

std::string HMCTS::PrintName()
{
	return "HMCTS";
}
void HMCTS::Select(Node* n, int PlayerID)
{
	int g = LD.GameFinished();
	if (g != -1)
	{
		int res = (g == PlayerID) ? 1 : (g == 3 - PlayerID) ? -1 : 0;
		n->Update(res);
	}
	else
	{
		int move = n->getFirstNotExplored(LD);
		if(move != -1) // There is an unxeplored move
		{	
			Expand(n, PlayerID, move);
		}
		else
		{
			Node* next = n->GetNextBest(PlayerID == LD.CurrentPlayer, UcbC);
			LD.PlayMove(next->move);
			Select(next, PlayerID);
		}
	}

}

void HMCTS::Expand(Node* n, int PlayerID, int move)
{
	auto next = n->addChild(n, move);
	LD.PlayMove(move);
	int res = Simulate(PlayerID);
	next->Update(res);

}

int HMCTS::Simulate(int PlayerID)
{
	int g = LD.GameFinished();
	if (g != -1) 
	{	
		return (g == PlayerID) ? 1 : (g == 3 - PlayerID) ? -1 : 0;
	}
	else
	{	
		int move;
		if (LD.Round > 25)
		{
			move = MoveFromProbas(PlayerID);
		}
		else
		{
			move = LD.GetRandomLegalMove();
		}
		LD.PlayMove(move);
		return Simulate(PlayerID);
	}
}

int HMCTS::MoveFromProbas(int PlayerID)
{
	double probas[children_size];
	double sum = 0.;
	for (int i = 0; i < children_size; ++i) // softmax
	{
		if (LD.IsLegal(i))
		{
			LD.PlayMove(i);
			probas[i] = std::exp(temp * LD.Evaluate(PlayerID));
			LD.UndoMove(i);
		}
		else
		{
			probas[i] = 0;
		}
		sum += probas[i];
	}
	for (int i = 0; i < children_size; ++i)
	{
		probas[i] /= sum;

	}
	double r = rand() / (RAND_MAX + 1.);
	int m = 0;
	while (r >= probas[m])
	{
		r -= probas[m];
		++m;
	}
	return m;
}
