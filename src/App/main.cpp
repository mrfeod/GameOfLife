#include <iostream>
#include <algorithm>

#include <time.h>
#include <assert.h>

#include <chrono>

#include "Life/LifeCicle.hpp"

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
	for (auto& line : field)
	{
		for (auto cell : line)
			cout << (cell ? "#" : " ") << " ";
		cout << endl;
	}
}

int main()
{
	const size_t size = 10000;
	srand(static_cast<uint32_t>(time(nullptr)));

	auto start = std::chrono::high_resolution_clock::now();
	LifeCicle life(size, size);
	auto finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	cout << "Init time " << elapsed.count() << endl;

	do
	{
		start = std::chrono::high_resolution_clock::now();
		life.NextStep();
		finish = std::chrono::high_resolution_clock::now();
		elapsed = finish - start;
		//print(life.State());
		cout << life.Step() << " " << elapsed.count() << endl;
		//sleepcp(DELAY);
	} while (life.Alive());
	system("pause");
	return 0;
}