#ifndef MAIN_LOOP_H
#define MAIN_LOOP_H

#include "stdafx.h"
#include "Timers.h"
#include "SomeAPI.h"
#include "SomeInputManager.h"
#include "SomeGraphicsEngine.h"
#include "SomeSystemThatServesSomeTimers.h"
#include "SomeClassThatNeedsATimer.h"

	
class MainLoop: public TimerManager{
public:
	MainLoop();
	virtual ~MainLoop();

	bool requestShutdown();
	bool captureAll();
	bool updateNetwork();
	bool updateGameworld(double sinceLastTime);
	bool updateAI();
	bool updateGFX(double sinceLastTime);
	bool run();

	bool startGraphicsEngine();
	bool getInputManager();

	void receiveTimerRequest(double msWhen, double id, TimerServer* s);
	void processTimerRequests();
	void addTimerServer(TimerServer* server);

	void pumpUptime();
	void pumpFPS();
	void pumpAICPS();
	void pumpNetworkCPS();
	void pumpGfxCPS();
	void pumpGameworldCPS();

private:
	//desired caps
	int fpsAI;
	int fpsGameworld;
	int fpsInput;
	int fpsNetwork;
	int fpsGFX;
	int fpsFPS;

	//derived cap tick count
	int maxticksAI;
	int maxticksGameworld;
	int maxticksInput;
	int maxticksNetwork;
	int maxticksGFX;
	int maxticksFPS;

	//actual call counts
	double countAI;
	double countGameworld;
	double countInput;
	double countNetwork;
	double countGFX;

	//last calls
	double lastAI;
	double lastGameworld;
	double lastInput;
	double lastNetwork;
	double lastGFX;
	double lastFPS;

	//simulation vars
	double SimSpeedMultiplier;
	double simTime;
	
	//calls per second vars
	double cpsAI;
	double cpsNetwork;
	double cpsGfx;
	double cpsGameworld;

	//calls since last cps calcs
	double countCPSAI;
	double countCPSGameworld;
	double countCPSInput;
	double countCPSNetwork;
	double countCPSGFX;

	//time since last cps calcs
	double lastcpsAI;
	double lastcpsGameworld;
	double lastcpsNetwork;
	double lastcpsGfx;
	
	//loop vars
	bool tickExceptionAI;
	bool permanentTickExceptionAI;
	double initTime;
	double startTime;
	double endTime;
	double lastTime;
	double loopCounter;
	double uptime;
	double averageFPS;

	//shutdown
	bool requestedShutdown;
	
	//mTimerMap[TimerServer][msWhen] = vector<TimerIDs>
	std::map<TimerServer*, std::map<double, std::vector<double>>> mTimerMap;
	std::vector<TimerServer*> mTimerServers;
	std::map<TimerServer*, double> mNextTimer;

	SomeInputManager* inputManager;
	SomeGraphicsEngine* renderer;
	SomeAPI* api;
};
#endif