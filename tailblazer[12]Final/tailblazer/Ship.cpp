#include "Ship.h"
Ship::Ship(char *mFilename)
{
	dead = false;
	position.x = 0.0;
	position.y = 0.0; 
	position.z = 0.0;

	ufo.loadModel(mFilename);
}
Ship::Ship(char *mFilename, char *tFilename)
{
	dead = false;
	position.x = 0.0; 
	position.y = 0.0; 
	position.z = 0.0;

	ufo.loadModel(mFilename);
	ufo.loadTexture(tFilename);
}

Ship::~Ship(void)
{
}
void Ship::draw()
{
	ufo.draw();
}
void Ship::kill()
{
	dead = true;
}
bool Ship::status()
{
	return dead;
}

void Ship::reducePoints(int p)
{		
		points -= p;
}
void Ship::setPoints(int p)
{
	points = p;
}
int Ship::getPoints()
{
	return points;
}