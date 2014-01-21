#include "stdafx.h"
#include "SomeAPI.h"

SomeAPI* SomeAPI::mAPI;

SomeAPI::SomeAPI(): TimerServer(){
	mTimerManager;
};

SomeAPI::~SomeAPI(){

};

void SomeAPI::set(std::string key, double value){
	//Your value storage function here
};

double SomeAPI::requestTimer(double msLength, TimerListener *l){
	Timer newTimer;
	newTimer.listener = l;
	newTimer.msLength = msLength;
	newTimer.msCalled = GetTickCount();

	double timerID = nextTimer++;
	timerMap[timerID] = newTimer;
	
	mTimerManager->receiveTimerRequest(GetTickCount()+msLength, timerID, this);

	return timerID;
};

void SomeAPI::relayTimerDone(double id){
	Timer t = timerMap[id];
	TimerListener* l = t.listener;
	if(l){
		l->timerDone(id);
	}
	timerMap.erase(id);
};

void SomeAPI::setTimerManager(TimerManager* manager){
	mTimerManager = manager;
};


SomeAPI* SomeAPI::getSingletonPtr(){
	if(!mAPI){
		mAPI = new SomeAPI();
	}
	return mAPI;
}