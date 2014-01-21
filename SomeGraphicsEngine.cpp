#include "stdafx.h"
#include "SomeGraphicsEngine.h"

SomeGraphicsEngine* SomeGraphicsEngine::mEngine;

SomeGraphicsEngine::SomeGraphicsEngine(){

};

SomeGraphicsEngine::~SomeGraphicsEngine(){

};

void SomeGraphicsEngine::go(){

};

void SomeGraphicsEngine::renderOneFrame(double sinceLastTime){

};

SomeGraphicsEngine* SomeGraphicsEngine::getSingletonPtr(){
	if(!mEngine){
		mEngine = new SomeGraphicsEngine();
	}
	return mEngine;
};