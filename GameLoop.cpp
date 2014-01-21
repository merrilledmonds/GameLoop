// GameLoop.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MainLoop.h"


int _tmain(int argc, _TCHAR* argv[])
{
	MainLoop main = MainLoop();
	main.run();
	std::exit(0);
}

