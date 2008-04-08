#pragma once
//#include "Vector.h"
#include "Model.h"
#include <stdlib.h>
#include <gl\glut.h>

struct Vector{
	float x, y, z;
};
class Dynamic
{
protected:
	Vector position;
	float speed;

public:
	Dynamic(void);
	~Dynamic(void);

	virtual void draw() = 0;

	void setPosition(float x, float y, float z);
	float getPosition(char vertex);

	void move();

	void setSpeed(float number);
	float getSpeed();

	void Reset(int z);

};
