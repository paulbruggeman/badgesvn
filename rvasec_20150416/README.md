RVAs4c 2015 Badges, by Hack.RVA
====================
This year's Badge features a 32-bit Microchip MCU (PIC32MX250128D), two linear capacitive touch sliders, IR Tx/Rx, USB, and buttons! We are still working out some of the features of the new badge, so we are developing on the 2014 badge for now. 

Features:
* 2 linear capactive touch sensors
* IR Tx/Rx
* 1 RGB LED
* USB
* Real time clock
* LCD
* Speaker
* Momentary button
* Are there any extra pins exposed for expansion?

Getting Started Developing for the Badge
---

I'll try to get more detailed instructions soon. But both the compiler and IDE are in the Arch User Repository, which makes it pretty easy. If you are running something else (Windows, Mac, Ubuntu, Fedora, BSD, etc.) and you are able to build this code base, feel free to send me a pull request with directions for the platform.

What you need: 
* MPLABX (http://www.microchip.com/pagehandler/en-us/family/mplabx/)
* MPLAB XC32 Compiler (32 bit C Compiler) (http://www.microchip.com/pagehandler/en-us/devtools/mplabxc/home.html)
* USB cable
* Badge Board
* Working knowledge of C

The basics: 

* Install MPLABX
* Install Microchip MPLAB XC32 Compiler (32 bit C Compiler)
* Clone this repository or download a zip of the source
* Enter bootloader mode by holding the button while plugging in USB.  The LEDs should blink to indicate bootload mode.
* Use bootloader software to upload new code!

(Is there any particular IDE configuration or set up needed?  Maybe just to disable warnings, or fix the warnings...)

TODO
---
Look here for ways to help out with software. Some of these are detailed, and some are more abstract problems with the current SW design. Suggestions, ideas, and of course pull requests welcome! Once I get specific issues nailed down, I'll start adding bug/feature requests to Git hub.

* Hardware
	* Determine how the battery attaches while keeping the badge useable from the lanyard and reducing stress on USB connector when plugged in to PC.
	* Choose LCD
	* Add schematics/board layout
	* Include BOM on wiki and GitHub
	* Source components
	* Manufacture boards
* Software
	* Framework
		* Move framework components out of main and badge files
		* Document App/Stage macro/API
		* Handle IR Tx/Rx
		    * Some of the old code attempted to do this, but I didn't have time in 2014 so it was shorted out of the code. Should consider using BadgeState's IR handler + some single queue of IR messages.
		* More Graphics primitives functions
		    * Have shitty square and horiz/vert lines drawings. May have full color LCD, so graphics stuff could use a lot of work.
		* Standardize the more complex peripheral interfaces
		    * Mainly graphics, but a standard message queue could work nicely for IR and future tx/rx (usb? other wire protocol)
		* Work with touch sliders
		    * The new badge will have the touch sliders attached to the built in capacitive touch HW on the MCU. We should research how this works. Also, getting the 2014 badge without the HW support (cap touch on the wrong ports :( ) working better would also be a plus. I think we can generally skip calibration on startup unless they select to do so.
		* Speaker/buzzer driver
		    * We'll want to use a timer interrupt. Need to make a nice interface for letting states play sounds easily.
		* Will probably have an RGB LED this year, so a nice interface for that. 
		    * Obvious start is a 3 param function with r,g,b, but some helper functions to do fading and what not could be useful. A good pwm method would be nice for leds in general.
        * Better backlight handling
            * Dimming with pwm would be nice, bit-banged or through HW if possible.
		* Context menu
		    * I like the idea of having a menu that's reachable by holding the main button. Make it so states can append extra options to this menu that apply only to the state. But more general seetings could always be adjusted here (sound on off, exit stage/game). I envision it dropping down from the top of the screen, pausing what is behind it.
        * Reading and writing to non-volatile storage
            * We get about 1000 erase/write cycles to non-volatile memory (NVM). Need interface & need to use wisely...
        * Sleep modes
            * When not in use, should consider a low power or low clock mode to use less battery.
        * Fix compiler warnings
            * Compiler complains a lot about functions and macros being removed from peripheral includes in future releases. Is there a fix? Or are they just taking these away from us...?
        * Emulated/stubebd version
            * Add ifdefs or something to swap out libraries/interfaces so this can be easily tested on a regular desktop
		* Handle user IO more cleanly
            * Create a single struct for the entire input state?
            * Use callbacks for different events (e.g. button pressed? Call this for me)
		    * Currently calling the same function to update I/O, this is perfectly maintainable, but it could probably be done better. Also, the USB stuff in ProcessIO is a mess from the very early days of dev and debug. Really needs to be sorted out.
	* Apps - Simple stage ideas
		* Clean up Badgy Bird to make a 'Best Practices' demo
		* Space invaders, pong, tetris, etc.
		* I'm toying with the idea of a simple birds eye view 2d game ala pokemon, and you have to find the "stages" or "games" in the virtual game world. A lot more to do before we get to this point, though it's good to be thinking and brainstorming about.
		* Consider integration with RVAsec CTF!
		* Puzzles and challenges that requrie some tinkering or hackery	
* Other
	* Determine how the upfront costs will be covered
	* Make a LiveCD with DEV environment
	* Document the API/Framework
	
	
Data Sheets
---
* [Processor: PIC32MX250F128D](http://ww1.microchip.com/downloads/en/DeviceDoc/60001168F.pdf)
* [IR Receiver: VISHAY TSOP39338](http://www.vishay.com/docs/81743/tsop381.pdf)
* [IR Transmit: VISHAY TSKS5400S](http://www.vishay.com/docs/81074/tsks540.pdf)


