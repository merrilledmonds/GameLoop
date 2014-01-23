
#include "stdafx.h"
#include "MainLoop.h"

MainLoop::MainLoop(){

	requestedShutdown = false;

	renderer = new SomeGraphicsEngine();
	api = SomeAPI::getSingletonPtr();

	addTimerServer(api);
	api->setTimerManager(this);

	startGraphicsEngine();
	getInputManager();

	/////////////////////////////////////////////
	// Record keeping variables: initialization
	//
	// Change these to cap/limit fps, etc.
	// Any negative number will remove limit.
	//

	fpsAI = 5; //AI decisions
	fpsGameworld = 30; //Gameworld updates; i.e. movement, deletion, state changes
	fpsInput = -1; //Input read freq. from buffer
	fpsNetwork = 15; //Network update freq.
	fpsGFX = -1; //Graphics update freq. = "real" FPS = what the player sees
	fpsFPS = 1; //Average FPS update freq. //Only for debug, doesn't affect gameplay

	//
	/////////////////////////////////////////////

	maxticksAI = 1000/fpsAI;
	maxticksGameworld = 1000/fpsGameworld;
	maxticksInput = 1000/fpsInput;
	maxticksNetwork = 1000/fpsNetwork;
	maxticksGFX = 1000/fpsGFX;
	maxticksFPS = 1000/fpsFPS;

	countAI=0;
	countGameworld=0;
	countInput=0;
	countNetwork=0;
	countGFX=0;
	
	lastAI = 0;
	lastGameworld = 0;
	lastInput = 0;
	lastNetwork = 0;
	lastGFX = 0;
	lastFPS = 0;

	cpsAI = 0;
	cpsNetwork = 0;
	cpsGfx = 0;
	cpsGameworld = 0;
	countCPSAI = 0;
	countCPSGameworld = 0;
	countCPSInput = 0;
	countCPSNetwork = 0;
	countCPSGFX = 0;
	lastcpsAI = 0;
	lastcpsGameworld = 0;
	lastcpsNetwork = 0;
	lastcpsGfx = 0;

	
	initTime = GetTickCount();
	startTime = 0;
	endTime = 0;
	lastTime = 0;
	loopCounter = 0;
	uptime = 0;
	averageFPS = 0;

	/////////////////////////////////////////////
	//
	// Simulation variables
	// Change to add exceptions to the AI, modify
	// simulation speed, etc.
	// 
	
	tickExceptionAI = false;
	permanentTickExceptionAI = false;
	SimSpeedMultiplier = 1.5;
	simTime = 0;

	//
	/////////////////////////////////////////////

	mTimerMap;
	mTimerServers;
	mNextTimer;

};
MainLoop::~MainLoop(){};

bool MainLoop::startGraphicsEngine(){
	try{
		//Your Graphics Engine initialization here
		renderer->go();
	}catch(...){
		//Your initialization error catches here
	};
	return true;
}
bool MainLoop::getInputManager(){
	inputManager = SomeInputManager::getSingletonPtr();
	return true;
}

bool MainLoop::requestShutdown(){
	requestedShutdown = true;
	return true;
};

bool MainLoop::captureAll(){
	//Can be expanded to accomodate multiple input managers,
	//but since the input manager should be handling ALL inputs,
	//this is undesirable behavior and should therefore call
	//the *input manager's captureAll* function instead.
	//At that point, you might as well just call it inputMgr->capture().
	try{
		inputManager->capture();

//////////////////////////////////////
// To simulate real applications	//
// DELETE ME						//
// Will mess up threading!			//
#ifdef _DEBUG						//
	Sleep(1); //input: 1 ms			//
#endif								//
//////////////////////////////////////
		return true;
	//catch all exceptions
	}catch(...){
		return false;
	}
};


/* Updates */

bool MainLoop::updateNetwork(){
	//Your network code here

//////////////////////////////////////
// To simulate real applications	//
// DELETE ME						//
// Will mess up threading!			//
#ifdef _DEBUG						//
	Sleep(1); //network: 1 ms		//
#endif								//
//////////////////////////////////////
	return true;
};

bool MainLoop::updateGameworld(double sinceLastTime){
	//Your game state code here
	//Your entity management code here

//////////////////////////////////////
// To simulate real applications	//
// DELETE ME						//
// Will mess up threading!			//
#ifdef _DEBUG						//
	Sleep(4); //gameworld: 4 ms		//
#endif								//
//////////////////////////////////////

	return true;
};

bool MainLoop::updateAI(){
	//Your AI code here

//////////////////////////////////////
// To simulate real applications	//
// DELETE ME						//
// Will mess up threading!			//
#ifdef _DEBUG						//
	Sleep(6); //AI: 6 ms			//
#endif								//
//////////////////////////////////////

	return true;
};

bool MainLoop::updateGFX(double sinceLastTime){
	//Your graphics code here
	//Your interpolation code here

	//Render call
	renderer->renderOneFrame(SimSpeedMultiplier*sinceLastTime/1000.0);
	

//////////////////////////////////////
// To simulate real applications	//
// DELETE ME						//
// Will mess up threading!			//
#ifdef _DEBUG						//
// You should never do this*, but	//
// this example is so simple that...//
// *See Stephan T. Lavavej's		//
// "rand() Considered Harmful" Talk	//
// at Going Native 2013.			//
	int randomTime = std::rand()%50;//
	Sleep(randomTime);				//
	//gfx: 0-50 ms					//
#endif								//
//////////////////////////////////////

	return true;
};

//API Pumps to record certain loop variables
//Allows the API to provide metrics such as FPS, CPS, Uptime etc.
//You can then access these values from any subroutine that can read from the API
void MainLoop::pumpUptime(){
	if(api){ api->set("Uptime",uptime); }
};
void MainLoop::pumpFPS(){
	if(api){ api->set("FPS",averageFPS); }
};
void MainLoop::pumpAICPS(){
	if(api){ api->set("AICPS",cpsAI); }
};
void MainLoop::pumpNetworkCPS(){
	if(api){ api->set("NetworkCPS",cpsNetwork); }
};
void MainLoop::pumpGfxCPS(){
	if(api){ api->set("GfxCPS",cpsGfx); }
};
void MainLoop::pumpGameworldCPS(){
	if(api){ api->set("GameworldCPS",cpsGameworld); }
};


/*** MAIN LOOP ***/
bool MainLoop::run(){
	bool running = true;
	bool emptypass = true;
	// number of loops since last average FPS calculation
	double deltaLoops = 0;
	// number of ticks(ms) since last loop
	double deltaTicks = 0;

	//Change for non-Windows
	double lastTime = GetTickCount();

	
//////////////////////////////////////////////////////////
//														//
// DELETE ME											//
//														//
// For demonstration only								//
// Note that std::cout is *slow* and will lower fps		//
#ifdef _DEBUG											//
		//A server acts as a middle-man between any listeners and the manager (the main loop)
		SomeSystemThatServesSomeTimers testTimerServer = SomeSystemThatServesSomeTimers();
		testTimerServer.setTimerManager(this);
		//Generally, the listener is where the timer actually starts doing something, such as
		//calculating stuff, triggering changes, etc.
		//TimerServer::relayTimerDone still leaves room in how it's implemented to notify
		//several Listeners that some Timer with some id has completed, so its behavior is
		//very implementation-heavy. In this implementation, the server relays the timerDone
		//call only to the TimerListener that requested it.
		SomeClassThatNeedsATimer testTimerListener = SomeClassThatNeedsATimer();
		testTimerListener.someFunctionThatAddsServers(&testTimerServer);
		//Imagine this function being called from within the loop by a part of one of the systems,
		//e.g. The AI needs a notification in 10 seconds that some building is about to collapse
		//	   This call will make sure the AI receives that notification the first loop after the
		//	   10-second mark (but not earlier).
		testTimerListener.someFunctionThatNeedsATimer();
#endif													//
//														//
//////////////////////////////////////////////////////////


	/*
	Each operational block, i.e. AI, Input, GFX, runs only if the number of ticks
	since the last loop are greater than the fps requirements, e.g. sinceLastTime
	is greater than maxticksAI. For maximum fps (no ceiling), this means that 
	fpsGFX is -1, and sinceLastTime>maxticksGFX will always be true, and the render
	call will always happen. This does not guarantee high fps, but it guarantees
	max fps possible after the AI load determined by fpsAI is satisfied.
	An exception is possible for the AI if the previous render frame has specifically
	requested a recalculation. This can be called by setting tickExceptionAI = true.
	*/
	while(running && !requestedShutdown){

		//Change for non-Windows
		startTime = GetTickCount();
		lastTime = GetTickCount();
		endTime = GetTickCount();
		//let API know how long we've been up
		uptime = startTime-initTime;
		pumpUptime();

		if((startTime-lastInput)>maxticksInput){
			captureAll();
			lastInput=startTime;
			countInput++;
			countCPSInput++;
		}

		/////////////////////////////////////////
		//
		// For the following chunks, the format
		// is as follows:
		//
		//	if it's been longer than the subsystem was allowed to go without an update{
		//		update system
		//		update timer for when this IF was last run = when the system was last updated
		//		update counter for the number of times this IF was run = the system was updated
		//		update counter for the number of loops CPS wasn't calculated, below
		//		
		//		(Calculating the average calls to the update function per second, CPS:)
		//		if the CPS loop count is >10, calculate an average CPS for the loop{
		//			calculate it
		//			pump it to API
		//			update timer for when CPS was last calculated
		//			reset CPS loop count
		//		}
		//
		//		record the fact that we did something this pass of the Game Loop,
		//		so it wasn't an empty pass. For debug purposes only.
		//	}
		//
		/////////////////////////////////////////

		//network
		if((startTime-lastNetwork)>maxticksNetwork){
			updateNetwork();
			lastNetwork=startTime;
			countNetwork++;
			countCPSNetwork++;
			if(countNetwork==10){
				cpsNetwork = 1000*countCPSNetwork/(startTime-lastcpsNetwork);
				pumpNetworkCPS();
				lastcpsNetwork = startTime;
				countCPSNetwork = 0;
			}
			emptypass = false;
		}
		
		//logic
		if((startTime-lastAI)>maxticksAI || tickExceptionAI || permanentTickExceptionAI){
			updateAI();
			lastAI=startTime;
			countAI++;
			countCPSAI++;
			if(countAI==10){
				cpsAI = 1000*countCPSAI/(startTime-lastcpsAI);
				pumpAICPS();
				lastcpsAI = startTime;
				countCPSAI = 0;
			}
			emptypass = false;
			//this resets the AI exception in case the IF check was (false||true) above
			tickExceptionAI=false;
		}

		//gameworld
		if((startTime-lastGameworld)>maxticksGameworld){
			updateGameworld(startTime-lastGameworld);
			lastGameworld=startTime;
			countGameworld++;
			countCPSGameworld++;
			if(countGameworld==10){
				cpsGameworld = 1000*countCPSGameworld/(startTime-lastcpsGameworld);
				pumpGameworldCPS();
				lastcpsGameworld = startTime;
				countCPSGameworld = 0;
			}
			emptypass = false;
		}

		//graphics
		if((startTime-lastGFX)>maxticksGFX){
			updateGFX(startTime-lastGFX);
			lastGFX = startTime;
			countGFX++;
			countCPSGFX++;
			if(countGFX==10){
				cpsGfx = 1000*countCPSGFX/(startTime-lastcpsGfx);
				pumpGfxCPS();
				lastcpsGfx = startTime;
				countCPSGFX = 0;
			}
			emptypass = false;
		}

		//Average FPS calcs
		//Similar to CPS calculations in sections above
		if((startTime-lastFPS)>maxticksFPS){
			//calculate average fps
			averageFPS = 1000*deltaLoops/(startTime-lastFPS);
			pumpFPS();
			lastFPS = startTime;
			deltaLoops = 0;	
		}

		//As the Timer Manager, we need to process Timer Requests
		//whenever possible to make sure we don't fall too far behind.
		//You can also add a maxtick timer to this like above to quantize
		//timer requests, so timers are triggered in blocks.
		processTimerRequests();

		//Calculate loop variables for next pass
		lastTime = startTime;
		loopCounter++;
		deltaLoops++;
		deltaTicks = GetTickCount()-endTime;
		simTime += deltaTicks*SimSpeedMultiplier;
		endTime = GetTickCount();
		
		//The loop should *never* do an empty pass. If it does, you should want
		//to be notified of this.
		if(emptypass){
			//Your warning code here.
			//std::cout<<"Warning: Game loop bypass.";
		}
		//reset for next pass
		emptypass = true;

//////////////////////////////////////////////////////////
//														//
// DELETE ME											//
//														//
// For demonstration only								//
// Note that std::cout is *slow* and will lower fps		//
// May print every time									//
#ifdef _DEBUG											//
		bool printEveryFrame = false;					//
	if(deltaLoops==1 || printEveryFrame){				//
		std::cout.precision(10);						//
		std::cout										//
				 <<"Loop "<<loopCounter<<"\n"			//
				 <<"   FPS (Avg.): "<<averageFPS<<"\n"	//
		         <<"   Frame length: "					//
				 <<""<<(int)(1000/(averageFPS+1e-20))	//
				 <<"ms\n"								//
		         <<"   Uptime: "<<uptime<<"ms"			//
				 <<" ("<<uptime/1000<<"s):\n"			//
		         <<"   SimTime: "<<simTime<<"ms"		//
				 <<" ("<<simTime/1000<<"s):\n"			//
		         <<"   Counts:   "						//
		         <<"AI "<<countAI<<" | "				//
		         <<"Gameworld "<<countGameworld<<" | "	//
		         <<"Input "<<countInput<<" | "			//
		         <<"Network "<<countNetwork<<" | "		//
		         <<"GFX "<<countGFX						//
		         <<std::endl;							//
		}												//
#endif													//
//														//
//////////////////////////////////////////////////////////
	}

	//Graceful shutdown
	return true;
};



///////////////////////////////////////
// Timer Manager Code - see Timers.h //
///////////////////////////////////////

void MainLoop::receiveTimerRequest(double msWhen, double id, TimerServer* s){
	mTimerMap[s][msWhen].push_back(id);
	//next timer is the first timer in the map
	mNextTimer[s] = mTimerMap[s].begin()->first;
};
void MainLoop::processTimerRequests(){
	std::vector<std::tuple<TimerServer*, double, double>> toDelete;
	//The map will by design be sorted by the timer msWhen values
	//So we just need to iterate over all Servers to get a map of msWhens to a vector of ids
	//If the msWhen we check has already passed, we fire off timers for the vector of ids and move to next
	//If it hasn't passed yet, none in the map can have passed, so we break the loop
	std::map<TimerServer*, std::map<double, std::vector<double>>>::iterator itOverServers;
	for(itOverServers = mTimerMap.begin(); itOverServers != mTimerMap.end(); itOverServers++){
		TimerServer* server = (*itOverServers).first;
		double now = GetTickCount();
		if(mNextTimer[server]<now){
			std::map<double, std::vector<double>>::iterator itOverWhen;
			for(itOverWhen = (*itOverServers).second.begin(); itOverWhen != (*itOverServers).second.end(); itOverWhen++){
				double when = (*itOverWhen).first;
				//if the timer hasn't been reached yet, the rest haven't either
				if(when>now){
					break;
					//if we have, signal that the timer is done
				}else{
					//iterate vector of IDs that have key msWhen
					std::vector<double>::iterator itOverTimerIDs;
					for(itOverTimerIDs = (*itOverWhen).second.begin(); itOverTimerIDs != (*itOverWhen).second.end(); itOverTimerIDs++){
						//if we have a valid server to call
						if(server!=0){
							server->relayTimerDone(*itOverTimerIDs);
							toDelete.push_back(std::make_tuple(server, when, *itOverTimerIDs));
						}
					}
				}
			}
		}
		//Delete any completed timers from map
		std::vector<std::tuple<TimerServer*, double, double>>::iterator delit;
		for(delit = toDelete.begin(); delit != toDelete.end(); delit++){
			TimerServer* server = std::get<0>(*delit);
			double when = std::get<1>(*delit);
			double id = std::get<2>(*delit);
			std::vector<double>::iterator memit = std::find(mTimerMap[server][when].begin(),mTimerMap[server][when].end(),id);
			mTimerMap[server][when].erase(memit);
			//when should be erased if depleted
			if(mTimerMap[server][when].size()==0){
				mTimerMap[server].erase(when);
			}
		}
	}
};

void MainLoop::addTimerServer(TimerServer* server){
	if(std::find(mTimerServers.begin(),mTimerServers.end(),server)==mTimerServers.end()){
		mTimerServers.push_back(server);
	}
};