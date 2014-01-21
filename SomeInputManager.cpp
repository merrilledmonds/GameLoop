#include "stdafx.h"
#include "SomeInputManager.h"

SomeInputManager* SomeInputManager::mInputManager;

SomeInputManager::SomeInputManager(){

};

SomeInputManager::~SomeInputManager(){

};

void SomeInputManager::capture(){
	//Your Input Manager's capture method here
};

SomeInputManager* SomeInputManager::getSingletonPtr(){
	if(!mInputManager){
		mInputManager = new SomeInputManager();
	}
	return mInputManager;
};