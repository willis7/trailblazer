#include "Dynamic.h"

Dynamic::Dynamic(void)
{
}

Dynamic::~Dynamic(void)
{
}
void Dynamic::setPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}
float Dynamic::getPosition(char vertex)
{
	if(vertex == 'x')
		return position.x;
	else if(vertex == 'y')
		return position.y;
	else
		return position.z;
}
void Dynamic::setSpeed(float n)
{
	speed = n;
}
float Dynamic::getSpeed()
{
	return speed;
}
void Dynamic::move()
{
	position.z += speed;
}
void Dynamic::Reset(int z)
{
	position.z = z;
}