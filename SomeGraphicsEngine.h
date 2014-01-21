#ifndef SOME_GRAPHICS_ENGINE_H
#define SOME_GRAPHICS_ENGINE_H

#include "stdafx.h"

class SomeGraphicsEngine{
public:
	SomeGraphicsEngine();
	virtual ~SomeGraphicsEngine();

	void go();
	void renderOneFrame(double sinceLastTime);

	static SomeGraphicsEngine* getSingletonPtr();

private:
	static SomeGraphicsEngine* mEngine;
};

#endif