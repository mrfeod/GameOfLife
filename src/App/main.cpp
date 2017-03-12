#include <iostream>
#include <algorithm>

#include <time.h>
#include <assert.h>

#include <chrono>
#include <thread>
#include <mutex>

#include "Life/LifeCicle.hpp"

#include "SDL.h"

using namespace std;

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
	srand(time(nullptr));

	SDL_Event event;
	SDL_Renderer *renderer;
	SDL_Window *window;
	SDL_Texture *texture = NULL;
	void *pixels;
	Uint8 *base;
	int pitch;

	const unsigned int WINDOW_WIDTH = 500;
	const unsigned int WINDOW_HEIGHT = WINDOW_WIDTH;
	unsigned int x;
	unsigned int y;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_WIDTH);

	std::thread lifeThread;
	std::mutex mx;
	std::unique_ptr<LifeCicle> life;
	lifeThread = std::thread([&life, &mx]()
	{
		mx.lock();
		const size_t size = 500;
		life = std::make_unique<LifeCicle>(size, size);
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
			SDL_LockTexture(texture, NULL, &pixels, &pitch);
			auto f = life->State();
			for (x = 0; x < WINDOW_WIDTH && x<f.size(); x++) {
				for (y = 0; y < WINDOW_HEIGHT && y<f[0].size(); y++) {
					base = ((Uint8 *)pixels) + (4 * (x * WINDOW_WIDTH + y));
					base[0] = f[x][y] * 255;
					base[1] = f[x][y] * 255;
					base[2] = f[x][y] * 255;
					base[3] = 0;
				}
			}
			SDL_UnlockTexture(texture);

			mx.unlock();
			lifeThread.swap(std::thread(worker));
			lifeThread.detach();
		}
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
