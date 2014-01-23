#include "stdafx.h"
#include "SomeClassThatNeedsATimer.h"

SomeClassThatNeedsATimer::SomeClassThatNeedsATimer(){
	mTimerServer;
	mTimerIDMap;
};
SomeClassThatNeedsATimer::~SomeClassThatNeedsATimer(){
};

void SomeClassThatNeedsATimer::someFunctionThatAddsServers(TimerServer* server){
	mTimerServer=server;
};

//Some function that, as part of its normal operations, gets called when a timer is done.
void SomeClassThatNeedsATimer::someFunctionThatDoesSomethingWithATimer(){
	//Whatever thread-safe, lag-free code you need to run

// Note that std::cout is *slow* and will lower fps		//
#ifdef _DEBUG
	std::cout<<"\n5 second timer is up.\n";
#endif
};

//Some function that, as part of its normal operations, needs to take some action not now, but later
//E.g. An AI function that needs to recalculate in 5 seconds, or
//	   A turret that can't shoot again  until 10 seconds have passed.
//In the first case, the timerDone(double id) function might change something like bool needToRecalculate so
//the next AI pass actually does some calculations.
//In the second case, the timerDone(double id) function might call some resetTurretFireLocks() function,
//so that turrets can fire again. (Instead of checking for bool canFire every time, or incrementing a timer.)
void SomeClassThatNeedsATimer::someFunctionThatNeedsATimer(){
	if(mTimerServer){
		double ID = mTimerServer->requestTimer(3250,this); //In 3.25 seconds or longer, the server will call timerDone.
		mTimerIDMap[ID]="Test";
// Note that std::cout is *slow* and will lower fps		//
#ifdef _DEBUG
	std::cout<<"Requested new timer.\n\n";
#endif
	}
};

//The server calls this function to signal that the timer is done
//i.e. Server::relayTimerDone(){
//			Listener->timerDone();
//	    }
void SomeClassThatNeedsATimer::timerDone(double id){
	if(mTimerIDMap[id] == "Test"){
		someFunctionThatDoesSomethingWithATimer();
		someFunctionThatNeedsATimer();
	}
};