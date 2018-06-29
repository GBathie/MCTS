#include "NoTreeGMCTS.h"
#include "MiniMax.h"
#include <chrono>


NoTreeGMCTS::NoTreeGMCTS(Puissance4 &ld, float U) : MCTS(ld, U) { }


int NoTreeGMCTS::RunSimulation(int PlayerID, int count)
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

int NoTreeGMCTS::RunTimeSimulation(int PlayerID, double timeBank)
{
	uint64_t boardP1 = LD.Board[0];
	uint64_t boardP2 = LD.Board[1];
	int Round = LD.Round;
	int CurrentPlayer = LD.CurrentPlayer;

	auto start = std::chrono::high_resolution_clock::now();	
	std::chrono::duration<double, std::milli> elapsed =  std::chrono::high_resolution_clock::now() - start;
	//int i=0;
	while (elapsed.count() < timeBank)
	{
		Select(root, PlayerID);
		LD.SetState(boardP1, boardP2, Round, CurrentPlayer);
		elapsed =  std::chrono::high_resolution_clock::now() - start;
		//++i;
	}
	// std::cout << i << " i ";
	return root->GetBest()->move;
}

std::string NoTreeGMCTS::PrintName()
{
	return "NoTreeGMCTS";
}

void NoTreeGMCTS::Select(Node* n, int PlayerID)
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


void NoTreeGMCTS::Expand(Node* n, int PlayerID, int move)
{
	auto next = n->addChild(n, move);
	LD.PlayMove(move);
	int res = Simulate(PlayerID);
	next->Update(res);

}

int NoTreeGMCTS::Simulate(int PlayerID)
{
	int g = LD.GameFinished();
	if (g != -1) 
	{
		return (g == PlayerID) ? 1 : (g == 3 - PlayerID) ? -1 : 0;
	}
	else
	{	
		int move = MiniMax::MiniMinMax(LD, PlayerID);
		LD.PlayMove(move);
		return Simulate(PlayerID);
	}
}