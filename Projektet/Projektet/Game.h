#pragma once

class Game
{
public:
	Game();
	~Game();
private:
	float m_dt;
	void newDT(float dt);
};