#include "MCTS.h"

#include <chrono>

#define SQRT2 1.4142
#define children_size 7


MCTS::MCTS(Puissance4 &ld, float U) : LD(ld)
{
	UcbC = U;
	randGen =  std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
	root = new Node(0);
}

MCTS::~MCTS()
{
	delete root;
}

void MCTS::ResetRoot()
{	
	delete root;
	root = new Node(0);
}

int MCTS::RunSimulation(int PlayerID, int count)
{
	uint64_t boardP1 = LD.Board[0], boardP2 = LD.Board[1];
	int Round = LD.Round;
	int CurrentPlayer = LD.CurrentPlayer;
	for (int i = 0; i < count; ++i)
	{
		Select(root, PlayerID);
		LD.SetState(boardP1, boardP2, Round, CurrentPlayer);
	}
	return root->GetBest()->move;
}

int MCTS::RunTimeSimulation(int PlayerID, double timeBank)
{
	uint64_t boardP1 = LD.Board[0];
	uint64_t boardP2 = LD.Board[1];
	int Round = LD.Round;
	int CurrentPlayer = LD.CurrentPlayer;

	auto start = std::chrono::high_resolution_clock::now();	
	std::chrono::duration<double, std::milli> elapsed =  std::chrono::high_resolution_clock::now() - start;
	int k = 0;
	while (elapsed.count() < timeBank)
	{
		Select(root, PlayerID);
		LD.SetState(boardP1, boardP2, Round, CurrentPlayer);
		elapsed =  std::chrono::high_resolution_clock::now() - start;
		++k;
	}
	std::cout << k << " k ";
	return root->GetBest()->move;
}

std::string MCTS::PrintName()
{
	return "MCTS";
}

void MCTS::DebugTree()
{		
	std::cerr << root->ToString() << std::endl;
	for (int i = 0; i < children_size; ++i)
	{
		if (root->children[i])
		{
			std::cerr << "\t" << root->children[i]->ToString() << std::endl;			
			for (int j = 0; j < children_size; ++j)
			{
				if (root->children[i]->children[j])
					std::cerr << "\t\t" << root->children[i]->children[j]->ToString() << std::endl;
			}
		}
	}
}

void MCTS::Select(Node* n, int PlayerID)
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

void MCTS::Expand(Node* n, int PlayerID, int move)
{
	auto next = n->addChild(n, move);
	LD.PlayMove(move);
	int res = Simulate(PlayerID);
	next->Update(res);

}

int MCTS::Simulate(int PlayerID)
{
	int g = LD.GameFinished();
	if (g != -1) 
	{	
		return (g == PlayerID) ? 1 : (g == 3 - PlayerID) ? -1 : 0;
	}
	else
	{	
		int move = LD.GetRandomLegalMove();
		LD.PlayMove(move);
		return Simulate(PlayerID);
	}
}
