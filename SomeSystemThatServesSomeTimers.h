#ifndef SOME_SYSTEM_THAT_SERVES_SOME_TIMERS_H
#define SOME_SYSTEM_THAT_SERVES_SOME_TIMERS_H

#include "stdafx.h"
#include "Timers.h"
#include "SomeClassThatNeedsATimer.h"

class SomeSystemThatServesSomeTimers: public TimerServer{
public:
	SomeSystemThatServesSomeTimers();
	~SomeSystemThatServesSomeTimers();

	//Called from a listener.
	double requestTimer(double msLength, TimerListener* l);
	//Called from a manager.
	void relayTimerDone(double id);
	
	void setTimerManager(TimerManager* manager);

private:
	TimerManager* mManager;
	std::map<double, TimerListener*> mTimerMap;
	double nextID;
};
#endif