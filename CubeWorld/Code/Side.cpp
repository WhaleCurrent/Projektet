#include "Side.h"

Side::Side(string sideName)
{
	side = sideName;
}
void Side::swapSides(Side *sideN)
{
	string temp = sideN->side;
	sideN->side = this->side;
	this->side = temp;
}
string Side::getString()
{
	return this->side;
}
void Side::setString(string name)
{
	this->side = name;
}