#ifndef SOME_CLASS_THAT_NEEDS_A_TIMER_H
#define SOME_CLASS_THAT_NEEDS_A_TIMER_H

#include "stdafx.h"
#include "Timers.h"

class SomeClassThatNeedsATimer: public TimerListener{
public:
	SomeClassThatNeedsATimer();
	~SomeClassThatNeedsATimer();

	void someFunctionThatNeedsATimer();
	void someFunctionThatDoesSomethingWithATimer();
	void someFunctionThatAddsServers(TimerServer* server);	//So this class can call whichever Server it needs
															//To demonstrate only - you might want to handle the
															//server-listener relationship differently for
															//different systems
	//Called from a server.
	void timerDone(double id);	//Actually does the heavy-lifting
								//You might want to make it so this method doesn't do much work -
								//i.e. flags a value so some other thread does the work when it reads the flag
								//If this method is slow, the whole MainLoop is slow.

private:
	TimerServer* mTimerServer;
	std::map<double, std::string> mTimerIDMap;
};
#endif