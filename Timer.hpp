#pragma once

#include <chrono>

class Timer
{
private:
	double p_deltaTime;

	std::chrono::time_point<std::chrono::high_resolution_clock> p_timer_start;
	std::chrono::time_point<std::chrono::high_resolution_clock> p_timer_stop;
public:

	inline void start_Timer() 
	{
		p_timer_start = std::chrono::high_resolution_clock::now();
	}

	inline void stop_Timer()
	{
		p_timer_stop = std::chrono::high_resolution_clock::now();
		p_deltaTime = 0.000000001 * std::chrono::duration_cast<std::chrono::microseconds>(p_timer_stop - p_timer_start).count();
	}

	inline double delta_Time() 
	{
		return p_deltaTime;
	}
};