#ifndef TIMERS_H
#define TIMERS_H

#include "stdafx.h"

class TimerListener;
class TimerManager;
class TimerServer;


class TimerListener{
public:
	//Notifies TimerListener that the timer with identification number id is done.
	//Timer identification number is provided by TimerServer.
	virtual void timerDone(double id) = 0;
};

struct Timer{
	double msLength;
	double msCalled;
	TimerListener* listener;
};

class TimerManager{
public:
	//Adds a TimerServer to the list of servers to receive Timer requests.
	virtual void addTimerServer(TimerServer* server) = 0;
	//Receives a Server's request to start a timer.
	virtual void receiveTimerRequest(double msWhen, double id, TimerServer* s) = 0;
	//Goes through all Servers' requests and completes those that have passed.
	virtual void processTimerRequests() = 0;
};

class TimerServer{
public:
	TimerServer();
	//Called by TimerListener to request a timer.
	//Keeps track of the TimerListener to relay the timer completion to.
	//Returns a unique timer identifier.
	virtual double requestTimer(double msLength, TimerListener* l) = 0;
	//Notifies TimerListener that the timer with identification number id is done.
	//Timer identification number is provided by TimerServer.
	virtual void relayTimerDone(double id) = 0;
	//Attaches Server to a Manager.
	virtual void setTimerManager(TimerManager* manager) = 0;
protected:
	double nextTimer;
	std::map<double, Timer> timerMap;
};

#endif