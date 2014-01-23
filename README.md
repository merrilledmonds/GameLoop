##GameLoop


###Project Summary
This is a C++ game loop based on [deWiTTERS' game loop](http://www.koonsolo.com/news/dewitters-gameloop/).
It provides an easy-to-implement template for general game development applications.


###License
The code in this repository is provided under the [MIT License](http://opensource.org/licenses/MIT).


###How To Use
The code should be treated as a template, and hacked or modified where necessary.

Initialize and run from your main program block:
```cpp
MainLoop main = MainLoop();
main.run();
```

Once running, the basic outline of the loop looks like this:
```cpp
bool MainLoop::run(){
	while(running && !requestShutdown()){
		if(timeToCheckInput){
			captureAll();
		}
		if(timeToCheckNetwork){
			updateNetwork();
		}
		if(timeToUpdateAI){
			updateAI();
		}
		if(timeToUpdateGameworld){
			updateGameworld();
		}
		if(timeToUpdateGraphics){
			updateGraphics();
		}
		
		calculateAverageFPS();
		
		processTimers();
	}
	return true;
};
```

The loop skips over any system that's running faster than the framerate specified during MainLoop initialization, so each system can run at its own pace.
Each system is called once its counter matches or falls behind this framerate designation, so the systems will always run at that rate or lower.

Also note that the code requires you to substitute your own `Graphics Engine`, `Input Manager`, some form of `API` that can record a `string, double` pair representing values such as the `FPS`, `Input calls per second`, etc.
The methods that call these systems are left empty so you can fill them in using your own favorite library.
I've tested the loop with `Ogre3D 1.7`, my own input manager that used `OIS` and a custom API class, and found the results to be sufficient.

The `MainLoop` class also comes with a timer system, as defined in `Timers.h`, which allows any systems to request a millisecond timer.
This can be used to pass a timer through the API, which might help with mod support for games.
Again, feel free to remove code pertaining to the `Timers` - the loop should work fine without it.


###Requirements
The loop was written to run with the Standard Library, targeting C++11 on Windows.
No external libraries should be necessary.


###Suggested Modifications
I haven't really modified the game loop to be thread-safe, but getting it to work with threads should be simple enough.
The loop code by itself should be encapsulated well enough that it shouldn't require _that_ much work.
