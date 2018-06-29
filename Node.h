#ifndef NODE_H_
#define	NODE_H_

#include <string>
#include <math.h>

#include "Puissance4.hpp"

#define children_size 7
#define UCB_CONSTANT  1.4142


class Node
{
private:
	Node* parent;
	int wins;
	int visits;

public:
	Node* children[children_size];
	int move;
	Node(Node* prev, int m);
	Node(int m);

	~Node();


	inline std::string ToString()
	{	
		return "Node : " + std::to_string(move) + ", " + std::to_string(wins) + "/" + std::to_string(visits) + ", score : " + std::to_string(Score());
	}

	inline void Update(int won)
	{
		++visits;
		wins += won;
		if(parent) parent->Update(won);
	}

	inline Node* addChild(Node* parent, int move) //And return it
	{
		Node* res = new Node(parent, move);
		children[move] = res;
		return res;
	}

	inline float UCB(bool maximizePlayer, float UCBCONST)
	{
		if (visits == 0) return 0.0f;
		if (maximizePlayer)
		{
			return ((float)(wins))/(visits) + UCBCONST * std::sqrt(std::log(parent->visits)/visits);
		}
		else
		{
			return ((float)(-wins))/(visits) + UCBCONST * std::sqrt(std::log(parent->visits)/visits);
		}
	}

	inline int getFirstNotExplored(Puissance4 &LD)
	{
		for (int i = 0; i < children_size; ++i)
	    {
	        if (!children[i] && LD.IsLegal(i))
	        {
	            return i;
	        }
	    }
	    return -1;
	}

	inline float Score()
	{
		return (visits == 0) ? 0 : (float)wins / visits;
	}

	Node* GetBest()
	{
		auto best = children[0];
		float max = -100000;
		for (int i = 0; i < children_size; ++i)
		{
			if (children[i])
			{
				float v = children[i]->Score();
				if (v > max)
				{
					best = children[i];
					max = v;
				}
			}
		}
		return best;
	}

	inline Node* GetChildWithMove(int nMove)
	{			
		return children[nMove];
	}

	inline Node* GetNextBest(bool maximizePlayer, float UCBCONST)
	{
		Node* best = children[0];
	    float max = -100000;
	    for (int i = 0; i < children_size; ++i)
	    {
	        if (children[i])
	        {
	            float v = children[i]->UCB(maximizePlayer, UCBCONST);
	            if (v > max)
	            {
	                best = children[i];
	                max = v;
	            }
	        }
	    }
	    return best;
	}
};

#endif // NODE_H_