#include "MCTS.h"
#include "HMCTS.h"
#include "NoTreeGMCTS.h"
#include <string>
#include <iostream>

#include <chrono>

namespace Session
{

	void PlayNGames(int n, int s1, int s2, float u1, float u2, Puissance4 &p)
	{
		std::cout << "Playing " << n <<" games : s =" << s1 << ", " << s2  << ", u = "<< u1 << ", " << u2 << std::endl;
		int score = 0, m = 0, draws = 0;
		MCTS Mcts = MCTS(p, u1);
		HMCTS HMcts = HMCTS(p, u2);
		for (int i = 0; i < n; ++i)
		{	
			std::cout << "Game " << i << std::endl;
			p.SetState(0,0,1,1);
			int g = p.GameFinished();
			while(g == -1)
			{
				Mcts.ResetRoot();
				m = Mcts.RunSimulation(1, s1);
				p.PlayMove(m);
				

				g = p.GameFinished();
				if (g != -1)
				{ 
					break;
				}

				HMcts.ResetRoot();
				m = HMcts.RunSimulation(2, s2);
				p.PlayMove(m);
				g = p.GameFinished();
			}
			if (g == 0) draws++;
			if (g == 1) score++;
		}
		std::cout << "Summary over " << n << " games :" << std::endl;
		std::cout << "------------------------" << std::endl;
		std::cout << "P1 (" << s1 <<", " << u1 << ") wins " << score  << "   " << ((float)score)/(float)n *100<< "%"<< std::endl;
		std::cout << "P2 (" << s2 <<", " << u2 << ") wins " << n - score - draws  << "   " << ((float)n - score - draws)/(float)n *100<< "%"<< std::endl;
		std::cout << "Draws : " << draws << std::endl;
	}
	// Polymorphism !
	void PlayNGamesTime(int n, double t, MCTS &playerOne, MCTS &playerTwo, Puissance4 &p)
	{
		playerOne.PrintName();
		playerTwo.PrintName();
		std::cout << "Playing " << n <<" games : t =" << t << std::endl;
		int score = 0, m = 0, draws = 0;
		for (int i = 0; i < n; ++i)
		{	
			std::cout << "Game " << i << "a";
			p.SetState(0,0,1,1);
			int g = p.GameFinished();
			while(g == -1)
			{
				playerOne.ResetRoot();
				m = playerOne.RunTimeSimulation(1, t);
				p.PlayMove(m);
				

				g = p.GameFinished();
				if (g != -1)
				{ 
					break;
				}

				playerTwo.ResetRoot();
				m = playerTwo.RunTimeSimulation(2, t);
				p.PlayMove(m);
				g = p.GameFinished();
			}
			if (g == 0) draws++;
			if (g == 1) score++;

			std::cout << ", Game " << i  << "b ";
			p.SetState(0,0,1,1);
			g = p.GameFinished();
			while(g == -1)
			{
				playerTwo.ResetRoot();
				m = playerTwo.RunTimeSimulation(1, t);
				p.PlayMove(m);

				g = p.GameFinished();
				if (g != -1)
				{ 
					break;
				}


				playerOne.ResetRoot();
				m = playerOne.RunTimeSimulation(2, t);
				p.PlayMove(m);

				g = p.GameFinished();
			}
			if (g == 0) draws++;
			if (g == 2) score++;
			std::cout << std::endl;


		}
		n = 2 * n;
		std::cout << "Summary over " << n << " games, time = " <<t << " :" << std::endl;
		std::cout << "-----------------------------------" << std::endl;
		std::cout << "P1 (" << playerOne.PrintName() << ") wins " << score  << "   " << ((float)score)/(float)n *100<< "%"<< std::endl;
		std::cout << "P2 (" << playerTwo.PrintName() << ") wins " << n - score - draws  << "   " << ((float)n - score - draws)/(float)n *100<< "%"<< std::endl;
		std::cout << "Draws : " << draws << std::endl;
	}


	void RunParallel()
	{	
		Puissance4 LD = Puissance4(1);
		int a;
		double t;
		std::cin >> a;
		std::cin >> t;
		//float u1 =1.4142f, u2 = 1.4142f;
		float u1, u2;
		std::cin >> u1 >> u2;
		//std::cerr << "u1 = " << u1 << ", u2 = " << u2 << std::endl;
		std::cout << "u2 = " << u2 << std::endl;
		int c;
		std::cin >> c;
		switch (c)
		{
			case 2:		
				{	
					MCTS Mcts = MCTS(LD, u1);
					NoTreeGMCTS NTGMcts = NoTreeGMCTS(LD, u2);
					PlayNGamesTime(a, t, Mcts, NTGMcts, LD);
				}
				break;
			case 3:
				{
					MCTS Mcts = MCTS(LD, u1);
					HMCTS HMcts = HMCTS(LD, 1.4142f, u2);
					PlayNGamesTime(a,t,Mcts, HMcts, LD);
				}
			default:
				std::cout << "Unkown Operation" << std::endl;
				break;
		}
	}

}
