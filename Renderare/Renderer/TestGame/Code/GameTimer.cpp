#include "GameTimer.h"
#include <windows.h>

GameTimer::GameTimer() : seconds_per_count_(0.0), delta_time_(-1.0), base_time_(0), paused_time_(0), stop_time_(0), previous_time_(0), current_time_(0), stopped_(false)
{
	__int64 counts_per_second;
	QueryPerformanceFrequency((LARGE_INTEGER*)&counts_per_second);
	
	seconds_per_count_ = 1.0f / (float)counts_per_second;

	QueryPerformanceCounter((LARGE_INTEGER*)&previous_time_);
}

void GameTimer::Tick()
{
	if (stopped_)
	{
		delta_time_ = 0.0;
		return;
	}

	//Get the time this fram
	current_time_ = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&current_time_);

	//Time difference beteween this frame and the previous.
	delta_time_ = (current_time_ - previous_time_) * seconds_per_count_;

	//Prepare for next frame.
	previous_time_ = current_time_;

	//Force nonnegative. The DXSDK's CDXUTTimer mentions that if the processor goes into a power save mode or we get shuffled to another processor, then m_DeltaTime can be negative.
	if (delta_time_ < 0.0)
	{
		delta_time_ = 0.0;
	}
}

float GameTimer::get_delta_time() const
{
	return (float)delta_time_;
}

void GameTimer::Reset()
{
	__int64 current_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&current_time);

	base_time_ = current_time;
	previous_time_ = current_time;
	stop_time_ = 0;
	stopped_ = false;
}

void GameTimer::Stop()
{
	if (!stopped_)
	{
		__int64 current_time;
		QueryPerformanceCounter((LARGE_INTEGER*)&current_time);

		stop_time_ = current_time;
		stopped_ = true;
	}
}

void GameTimer::Start()
{
	__int64 start_time;
	QueryPerformanceCounter((LARGE_INTEGER*)&start_time);

	if (stopped_)
	{
		paused_time_ += (start_time - stop_time_);

		previous_time_ = start_time;

		stop_time_ = 0;
		stopped_ = false;
	}
}

float GameTimer::GetTotalTime() const
{
	if (stopped_)
	{
		return (float)(((stop_time_ - paused_time_) - base_time_) * seconds_per_count_);
	}
	else
	{
		return (float)(((current_time_ - paused_time_) - base_time_) * seconds_per_count_);
	}
}