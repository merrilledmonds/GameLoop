#ifndef SOME_INPUT_MANAGER_H
#define SOME_INPUT_MANAGER_H

#include "stdafx.h"

class SomeInputManager{
public:
	SomeInputManager();
	virtual ~SomeInputManager();

	//Some capture function
	void capture();

	static SomeInputManager* getSingletonPtr();

private:
	static SomeInputManager* mInputManager;
};

#endif