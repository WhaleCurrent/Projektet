#pragma once

class GameTimer
{
private:
	float seconds_per_count_;
	float delta_time_;

	__int64 base_time_;
	__int64 paused_time_;
	__int64 stop_time_;
	__int64 previous_time_;
	__int64 current_time_;

	bool stopped_;

public:
	GameTimer();
	float GetTotalTime() const; //in seconds.
	float get_delta_time() const; //in seconds.

	void Reset(); //Call before message loop.
	void Start(); //Call when unpasued.
	void Stop(); //Call when paused.
	void Tick(); //Call every frame.
};