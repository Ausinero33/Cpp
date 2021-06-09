#pragma once
#include <chrono>
#include<Windows.h>

class Timer
{
public:
	Timer() : beg_(clock_::now()) {}
	void reset() { beg_ = clock_::now(); }
	float elapsed() const {
		return std::chrono::duration_cast<second_>
			(clock_::now() - beg_).count();
	}

private:
	typedef std::chrono::high_resolution_clock clock_;
	typedef std::chrono::duration<float, std::ratio<1> > second_;
	std::chrono::time_point<clock_> beg_;
};

class Motor
{
public:
	void ejecutar() {
		const float fps = 300.0f;
		const float dt = 1.0f / fps;
		float accumulator = 0.0f;
		Timer tmr;
		float frameStart = tmr.elapsed();

		while (true) {
			gotoxy(0, 0);
			//time
			float currentTime = tmr.elapsed();
			accumulator += currentTime - frameStart;

			frameStart = currentTime;


			if (accumulator >= 1.0f / 30.0f)
				accumulator = 1.0f / 30.0f;

			while (accumulator > dt) {
				logica();
			}
		}
	}
	virtual void logica() = 0;

private:
	void gotoxy(short x, short y)
	{
		COORD coord = { x, y };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
};
