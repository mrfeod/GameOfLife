#include <iostream>
#include <algorithm>

#include <time.h>
#include <assert.h>

#include <chrono>
#include <thread>
#include <mutex>

#include "Life/LifeCicle.hpp"

//#include "SDL.h"

using namespace std;

const size_t SIZE = 50;
const int DELAY = 100;

void sleepcp(int milliseconds)
{
	clock_t time_end;
	time_end = clock() + milliseconds * CLOCKS_PER_SEC / 1000;
	while (clock() < time_end)
	{
	}
}

void print(const LifeCicle::Univers& field)
{
	assert(field.size() != 0);
	system("cls");
	auto h = field.size();
	auto w = field[0].size();
	if (h > 20)
		h = 20;
	if (w > 20)
		w = 20;
	
	for (size_t i = 0; i < h; ++i)
	{
		for (size_t j = 0; j < w; ++j)
			cout << (field[i][j] ? "#" : " ") << " ";
		cout << endl;
	}
}

int main(int argc, char* argv[])
{
	srand(static_cast<uint32_t>(time(nullptr)));

	std::thread lifeThread;
	std::mutex mx;
	std::unique_ptr<LifeCicle> life;
	lifeThread = std::thread([&life, &mx]()
	{
		mx.lock();
		cout << "Hello..." << endl;
		const size_t size = 10000;
		life = std::make_unique<LifeCicle>(size, size);
		cout << "Bye..." << endl;
		mx.unlock();
	});
	lifeThread.detach();

	while (!mx.try_lock()) {}
	mx.unlock();

	auto worker = [&life, &mx]()
	{
		mx.lock();
		cout << life->Step() << endl;
		life->NextStep();
		mx.unlock();
	};

	while (life->Alive())
	{
		if (mx.try_lock())
		{
			mx.unlock();
			lifeThread.swap(std::thread(worker));
			lifeThread.detach();
		}
	}
	system("pause");
	return 0;
}
