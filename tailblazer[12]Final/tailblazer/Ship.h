#pragma once
#include "Dynamic.h"


class Ship: public Dynamic
{
protected:
	bool dead;
	Model ufo;
	int points;

public:
	Ship(char *mFilename);
	Ship(char *mFilename, char *tFilename);
	~Ship(void);

	void draw ();
	void kill ();
	bool status();

	void reducePoints(int p);
	void setPoints(int p);
	int getPoints(void);
};
