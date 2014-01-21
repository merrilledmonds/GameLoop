#ifndef TIMERS_H
#define TIMERS_H

#include "stdafx.h"

class TimerListener;
class TimerManager;
class TimerServer;


class TimerListener{
public:
	///<summary>Notifies TimerListener that the timer with identification number id is done.</summary>
	///<param name="id">Timer identification number provided by TimerServer.</param>
	///<returns>Nothing.</returns>
	virtual void timerDone(double id) = 0;
};

struct Timer{
	double msLength;
	double msCalled;
	TimerListener* listener;
};

class TimerManager{
public:
	///<summary>Adds a TimerServer to the list of servers to receive Timer requests.</summary>
	///<param name="server">Server to add.</param>
	///<returns>Nothing.</returns>
	virtual void addTimerServer(TimerServer* server) = 0;
	///<summary>Receives a Server's request to start a timer.</summary>
	///<param name="msWhen">Tick Count when the timer is done.</param>
	///<param name="id">ID of Timer that will be passed to the Server when timer is done.</param>
	///<param name="s">Server to receive this Timer's events.</param>
	///<returns>Nothing.</returns>
	virtual void receiveTimerRequest(double msWhen, double id, TimerServer* s) = 0;
	///<summary>Goes through all Servers' requests and completes those that have passed.</summary>
	///<returns>Nothing.</returns>
	virtual void processTimerRequests() = 0;
};

class TimerServer{
public:
	TimerServer();
	///<summary>Called by TimerListener to request a timer.</summary>
	///<param name="msLength">Length of timer in milliseconds (ticks).</param>
	///<param name="l">The TimerListener to relay the timer completion to.</param>
	///<returns>double id: Unique timer identifier.</returns>
	virtual double requestTimer(double msLength, TimerListener* l) = 0;
	///<summary>Notifies TimerListener that the timer with identification number id is done.</summary>
	///<param name="id">Timer identification number provided by TimerServer.</param>
	///<returns>Nothing.</returns>
	virtual void relayTimerDone(double id) = 0;
	///<summary>Attaches Server to a Manager.</summary>
	///<param name="manager">Manager to attach to.</param>
	///<returns>Nothing.</returns>
	virtual void setTimerManager(TimerManager* manager) = 0;
protected:
	double nextTimer;
	std::map<double, Timer> timerMap;
};


#endif