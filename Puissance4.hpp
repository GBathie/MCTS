#ifndef PUISSANCE4_HPP_
#define	PUISSANCE4_HPP_

#include <iostream>
#include <limits.h>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <bitset> 
#include <chrono>
#include <random>
#define L 6
#define Col 7
#define EVAL_THREE 10000
#define EVAL_TWO 100
#define P4MASK 0b0111111011111101111110111111011111101111110111111ULL


class Puissance4
{
// New version of Puissance4 that doesnt need unplaying
public:
	int CurrentPlayer; // Value is 1 or 2
	int Round;
	uint64_t Board [2];
	std::mt19937 randGen;

	Puissance4(int botID) : CurrentPlayer(botID), Round(botID)
	{
		Board[0] = 0;
		Board[1] = 0;
		randGen =  std::mt19937(
				std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::high_resolution_clock::now().time_since_epoch()).count());
	}

		
	void DebugBoard()
	{
		for (int i = L - 1; i >= 0; --i)
		{
			for (int j = 0; j < Col; ++j)
			{
				if (Board[0] & ((uint64_t)1 << (i + (L+1) * j)))
				{
					std::cerr << "1";
				}
				else if (Board[1] & ((uint64_t)1 << (i + (L+1) * j)))
				{
					std::cerr << "2";
				}
				else
				{
					std::cerr << "0";
				}
			}
			std::cerr << std::endl;
		}
    	std::cerr << "CurrentPlayer = " << CurrentPlayer << std::endl;
    	std::cerr << "1 = " << std::bitset<64>(Board[0])  << std::endl;
    	std::cerr << "2 = " << std::bitset<64>(Board[1])  << std::endl;
    	std::cerr << "Round : " << Round;
    	std::cerr << "GameFinished = " << GameFinished() << std::endl;
	}

	void SetState(uint64_t b1, uint64_t b2, int r, int cp)
	{
		Board[0] = b1;
		Board[1] = b2;
		Round = r;
		CurrentPlayer = cp;
	}

    inline bool HasWon(uint64_t board)
   	{
    	uint64_t y = board & (board >> 6);
        if (y & (y >> 2 * 6))     // check \ diagonal
        	return true;
		y = board & (board >> 7);
        if (y & (y >> 2 * 7))     // check horizontal
        	return true;
		y = board & (board >> 8);
        if (y & (y >> 2 * 8))     // check / diagonal
        	return true;
        y = board & (board >> 1);
        if (y & (y >> 2))         // check vertical
			return true;
        return false;
    }

	inline int GameFinished()
	{
		
		if (HasWon(Board[2 - CurrentPlayer]))
		{
			return 3 - CurrentPlayer;
		}
		else if (Round > 42)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}

	inline int GetCurrentPlayer()
	{
		return CurrentPlayer;
	}

	inline int GetRandomLegalMove() // is biased
	{	
		int res = randGen() % Col;
		while (!IsLegal(res)) 
		{
			res = (res + 1) % Col;
		}
		return res;
	}

	inline bool IsLegal(int move)
	{
		return !((Board[0] | Board[1])&((uint64_t)1 << (5 + move * (L + 1))));
	}

	inline void PlayMove(int move)
	{
		uint64_t totalBoard = Board[0] | Board[1];
		int i = 0;
		while (totalBoard & ((uint64_t)1 << (i + (L + 1) * move)))
		{
			++i;
		}
		Board[CurrentPlayer - 1] |= ((uint64_t)1 << (i + (L + 1) * move));
		++Round;
		SwitchPlayers();
	}

	inline void UndoMove(int Move)
	{
		SwitchPlayers();
		--Round;
		int i = 5;
		while (!(Board[CurrentPlayer - 1] & ((uint64_t)1 << (i + (L + 1) * Move))))
		{
			--i;
		}	
		Board[CurrentPlayer - 1] &= ~((uint64_t)1 << (i + (L + 1) * Move));
	}

	inline int Evaluate(int PlayerID)
	{
		int g = HasWon(Board[PlayerID - 1]);
		if (g == PlayerID) 
		{
			return INT_MAX - 1;
		}
		if (g == 3 - PlayerID) 	
		{
			return INT_MIN + 1;
		}
		if (g == 0) return 0;
		return Meval(PlayerID) - Meval(3 - PlayerID);
	}


private:
	
	inline void SwitchPlayers()
	{
		CurrentPlayer = 3 - CurrentPlayer;
	}


	inline int numberOfThree(int PlayerID)
	{

		uint64_t b1 = Board[PlayerID];
		uint64_t b2 = ~Board[1 - PlayerID] & P4MASK;

    	//std::cerr << "b = " << std::bitset<64>(b2)  << std::endl;
		int res = 0;

		uint64_t b = b1 & (b1 >> 6) & (b1 >> 12) & (b2 >> 18);
		b |= b1 & (b1 >> 6) & (b1 >> 18) & (b2 >> 12);
		b |= b1 & (b1 >> 12) & (b1 >> 18) & (b2 >> 6);
		b |= (b1 >> 6) & (b1 >> 12) & (b1 >> 18) & b2;
		res += __builtin_popcountll(b);
    	//std::cerr << "b = " << std::bitset<64>(b)  << ", res : " << res << std::endl;

    	// |
		b  = b1 & (b1 >> 1) & (b1 >> 2) & (b2 >> 3);
		b |= b1 & (b1 >> 1) & (b1 >> 3) & (b2 >> 2);
		b |= b1 & (b1 >> 2) & (b1 >> 3) & (b2 >> 1);
		b |= (b1 >> 1) & (b1 >> 2) & (b1 >> 3) & b2;
		res += __builtin_popcountll(b);
    	//std::cerr << "b = " << std::bitset<64>(b)  << ", popcnt : " << __builtin_popcountll(b) << ", res : " << res << std::endl;


    	// ----
		b  = b1 & (b1 >> 7) & (b1 >> 14) & (b2 >> 21);
		b |= b1 & (b1 >> 7) & (b1 >> 21) & (b2 >> 14);
		b |= b1 & (b1 >> 14) & (b1 >> 21) & (b2 >> 7); 
		b |= (b1 >> 7) & (b1 >> 14) & (b1 >> 21) & b2; 
		res += __builtin_popcountll(b);
    	//std::cerr << "b = " << std::bitset<64>(b)  << ", popcnt : " << __builtin_popcountll(b) << ", res : " << res << std::endl;

		b  = b1 & (b1 >> 8) & (b1 >> 16) & (b2 >> 24);
		b |= b1 & (b1 >> 8) & (b1 >> 24) & (b2 >> 16);
		b |= b1 & (b1 >> 16) & (b1 >> 24) & (b2 >> 8); 
		b |= (b1 >> 8) & (b1 >> 16) & (b1 >> 24) & b2; 
		res += __builtin_popcountll(b);
    	//std::cerr << "b = " << std::bitset<64>(b)  << ", res : " << res << std::endl;

		return res;
	}

	inline int numberOfTwos(int PlayerID)
	{
		uint64_t b1 = Board[PlayerID];
		uint64_t b2 = ~Board[1 - PlayerID] & P4MASK;

		int res = 0;

		uint64_t nb2 = b2 & (b2 >> 1) & (b2 >> 2) & (b2 >> 3); 
		uint64_t b = (b1 & (b1 >> 1)) | ((b1 >> 2) & (b1 >> 3));
		b |= (b1 | (b1 >> 1)) & ((b1 >> 2) | (b1 >> 3));
		b &= nb2; 
		res += __builtin_popcountll(b);
    	//std::cerr << "b = " << std::bitset<64>(b)  << ", res : " << res << std::endl;

		nb2 = b2 & (b2 >> 6) & (b2 >> 12) & (b2 >> 18);
		b  = (b1 & (b1 >> 6)) | ((b1 >> 12) & (b1 >> 18));
		b |= (b1 | (b1 >> 6)) & ((b1 >> 12) | (b1 >> 18));
		b &= nb2;  
		res += __builtin_popcountll(b);
		//std::cerr << "b = " << std::bitset<64>(b)  << ", res : " << res << std::endl;

		nb2 = b2 & (b2 >> 7) & (b2 >> 14) & (b2 >> 21);		
		b  = (b1 & (b1 >> 7)) | ((b1 >> 14) & (b1 >> 21));
		b |= (b1 | (b1 >> 7)) & ((b1 >> 14) | (b1 >> 21));
		b &= nb2;
		res += __builtin_popcountll(b);
		//std::cerr << "b = " << std::bitset<64>(b)  << ", res : " << res << std::endl;

		nb2 = b2 & (b2 >> 8) & (b2 >> 16) & (b2 >> 24);
		b  = (b1 & (b1 >> 8)) | ((b1 >> 16) & (b1 >> 24));
		b |= (b1 | (b1 >> 8)) & ((b1 >> 16) | (b1 >> 24));
		b &= nb2;  
		res += __builtin_popcountll(b);
		//std::cerr << "b = " << std::bitset<64>(b)  << ", res : " << res << std::endl;

		//std::cerr << "b = " << std::bitset<64>(b)  << ", popcnt : " << __builtin_popcountll(b) << ", res : " << res << std::endl;

		return res;
	}

	inline int Meval(int PlayerID)
	{
		//std::cerr << PlayerID << std::endl;
		int n = numberOfThree(PlayerID - 1);
		//std::cerr << "Nb of 3 : " << n << ", Twos : " << numberOfTwos(PlayerID) - n << std::endl;
		return n * EVAL_THREE + (numberOfTwos(PlayerID - 1) - n) * EVAL_TWO;
	}
};

#endif // PUISSANCE4_HPP_
