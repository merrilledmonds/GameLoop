#ifndef SOME_API_H
#define SOME_API_H

#include "stdafx.h"
#include "Timers.h"

class SomeAPI: public TimerServer{
public:
	SomeAPI();
	virtual ~SomeAPI();

	void set(std::string key, double value);

	double requestTimer(double msLength, TimerListener* l);
	void relayTimerDone(double id);
	void setTimerManager(TimerManager* manager);

	static SomeAPI* getSingletonPtr();

private:
	static SomeAPI* mAPI;
	TimerManager* mTimerManager;
};

#endif