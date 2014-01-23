#include "stdafx.h"
#include "SomeSystemThatServesSomeTimers.h"

SomeSystemThatServesSomeTimers::SomeSystemThatServesSomeTimers():TimerServer(){
	mManager;
	nextID = 1;
};
SomeSystemThatServesSomeTimers::~SomeSystemThatServesSomeTimers(){};

//The listener calls this function to request a timer.
//i.e. Listener::someFunction(){
//			Server->requestTimer(1000, this);
//	   }
double SomeSystemThatServesSomeTimers::requestTimer(double msLength, TimerListener* l){
	if(mManager && l){
		mManager->receiveTimerRequest(GetTickCount()+msLength,nextID,this);
		mTimerMap[nextID]=l;
		return nextID++;
	}
	return 0;
};

//The manager calls this function to signal a timer is done.
//i.e. Manager::someFunction(){
//			Server->relayTimerDone(123);
//	   }
void SomeSystemThatServesSomeTimers::relayTimerDone(double id){
	if(mTimerMap[id]){
		mTimerMap[id]->timerDone(id);
	}
};

void SomeSystemThatServesSomeTimers::setTimerManager(TimerManager* manager){
	mManager = manager;
};