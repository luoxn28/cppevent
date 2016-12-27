#include <iostream>

#include "../event/IReactor.h"
#include "../event/EpollReactor.h"

using namespace std;

int main(int argc, char **argv)
{
	cout << "hello cppevent" << endl;

	EpollReactor epollReactor;

	cout << epollReactor.getEpollFd() << endl;

	return 0;
}

