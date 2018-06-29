// LightRider.cpp?: d?finit le point d'entr?e pour l'application console.
//
#pragma GCC optimize "O3,omit-frame-pointer,inline"
#include "Session.hpp"
#include <ctime>

int main()
{
	std::srand(time(0)); 
    Session::RunParallel();
    return 0;
}

