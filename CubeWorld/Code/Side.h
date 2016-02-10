#ifndef SIDE_H
#define SIDE_H
#include <string>
using namespace std;

class Side
{
private:
	string side;

public:
	Side(string sideName);
	void swapSides(Side *side);
	string getString();
	void setString(string name);


};
#endif