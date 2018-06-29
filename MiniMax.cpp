#include <iostream>
#include "MiniMax.h"

int MiniMax::MiniMinMax(Puissance4 &LD, int PlayerID)
{
	int BestMove = 0;
	int BestScore = INT_MIN;
	for (int i = 0; i < 7; ++i)
	{
		if (LD.IsLegal(i))
		{
			LD.PlayMove(i);
			int s = LD.Evaluate(PlayerID);
			LD.UndoMove(i);
			if (s > BestScore)
			{
				BestMove = i;
				BestScore = s;
			}
		}
	}
	return BestMove;
}
