/*
 * PushButton.cpp
 *
 *  Created on: 22.04.2016
 *      Author: butch
 */

#include "PushButton.h"
#include "arduino.h"

#define BUTTON_STATE_PRESSED false
#define BUTTON_STATE_DEFAULT true

#define IS_DEBUG_PUSHBUTTON 0


/**
 * mode 0 for internal /use GPIO),
 * mode 1 for external (set status directly by other source).
 * buttonPin will be set to -1 if mode is external is optional and will be ignored.
 *
 */
PushButton::PushButton(char mode, int buttonPin = -1) {
	// check the given mode
	switch (mode)
	{
		// we are using pin input
		_mode = mode;
		_buttonPin = buttonPin;
		pinMode(_buttonPin, INPUT_PULLUP);



	}

	_buttonPin = buttonPin;
	pinMode(_buttonPin, INPUT_PULLUP);

	_debounceTimeMs=30;
	_logicalStateMinTimeButtonPressed=50;
	_lastButtonStateChangedTime=millis();
	_hasLogicalStateChangeBeenProcessed=false;
	_buttonState=BUTTON_STATE_DEFAULT;
	_buttonStateUnbounced=BUTTON_STATE_DEFAULT;
	_logicalState=false;
	_debounceTimeStart=-1;
	processButtonState();
	_buttonStatePressedOnlyOnce=false;
}

PushButton::~PushButton() {
	// TODO Auto-generated destructor stub
}

/**
 * sets the debounce time
 */
void PushButton::setDebounceTimeMs(int time)
{
	_debounceTimeMs=time;
}


void PushButton::processButtonState(bool flipLogicalState=true)
{
	// get current button state ti have consistent baseline for this run
	bool currentButtonState = digitalRead(_buttonPin);
	long currentTime = millis();
	
	#if IS_DEBUG_PUSHBUTTON
		Serial.print("currentButtonState=");
		Serial.print(currentButtonState);
		Serial.print(" currentTime=");
		Serial.print(currentTime);
		Serial.print(" ");
	#endif

	// mark buttonOnceState as false. It will onle be set to true if the logical state changes.
	_buttonStatePressedOnlyOnce=false;

	// check if debounce time is not started
	if (_debounceTimeStart == -1)
	{
		// debounce Time is not started

		// check if button is pressed
		if (currentButtonState == BUTTON_STATE_PRESSED)
		{
			// start debounce time
			_debounceTimeStart = currentTime;

			// mark logical button state change as outstanding
			_hasLogicalStateChangeBeenProcessed=false;



		}
		else
		{
			// debounce intervall has not been started && button is not pressed
					//-> do nothing

		}

	}
	else
	{
		// debounceTimeStart has been started
		// check if we are in debounce interval
		if (currentTime - _debounceTimeStart < _debounceTimeMs)
		{
			// since we are inside the debounce interval, ignore switches of the button state and do nothing
			// -> do nothing
			

		}
		else
		{
			// we are outside of debounce time interval / debounce time interval is through
			// check if button is pressed
			if (currentButtonState == BUTTON_STATE_PRESSED)
			{
				// debounce time has been started && and debounce time is through && button is pressed

				// check if logical button state change already has been applied
				if (_hasLogicalStateChangeBeenProcessed)
				{
					// has been applied
					//-> nothing to do
				}
				else
				{
					// only flip logicalState if  flipLogicalState == true. (can be disabled in menu for example)
					if (flipLogicalState == true)
					{
						// has not been applied yet
						// flip the logical state
						if (_logicalState)
						{
							// true -> flip to false
							_logicalState=false;
						}
						else
						{
							// false -> flip to true
							_logicalState=true;
						}
					}

					// keep original logic regardless if flipLogicalState == true

					// remember that logical state change has been done
					_hasLogicalStateChangeBeenProcessed=true;

					// mark the button as pressed (once)
					_buttonStatePressedOnlyOnce=true;
				}
			}
			else
			{
				// button is not pressed / time is through
				// -> reset the timers, etc.
				_hasLogicalStateChangeBeenProcessed = false;
				_debounceTimeStart=-1;


			}
			_buttonStateUnbounced = currentButtonState;
		}
	}

	#if IS_DEBUG_PUSHBUTTON
		Serial.print("_debounceTimeStart=");
		Serial.print(_debounceTimeStart);
		Serial.print(" _debounceTimeMs=");
		Serial.print(_debounceTimeMs);
		Serial.print(" _hasLogicalStateChangeBeenProcessed=");
		Serial.print(_hasLogicalStateChangeBeenProcessed);
		Serial.print(" _buttonState=");
		Serial.print(_buttonState);
		Serial.print(" _logicalState=");
		Serial.print(_logicalState);
		Serial.print("\n");
	#endif

}

/**
 * returns the unbounced button state
 */
bool PushButton::getButtonStateRawDebounced()
{
	return _buttonStateUnbounced;
}

/**
 * returns the unbounced button state, but in negated form.
 * intended to be used with INPUT_PULLUP, where then normal debounced state returns true, if the button is NOT pressed.
 */
bool PushButton::getButtonStateRawDebouncedInputPullup()
{
	if (_buttonStateUnbounced)
	{
		return false;
	}
	else
	{
		return true;
	}
}



/**
 * returns the real state of the button (pressed / not pressed)
 */
bool PushButton::getButtonStateRawData()
{
	return _buttonState;
}

/**
 * returns the logical state of the button which flips between true and false by each press
 */
bool PushButton::getButtonStateLogical()
{
	return _logicalState;
}


/**
 * return - time in millis since the last raw button state has been changed.
 *          In other words: How long has the button been pressed / not pressed since last action on button
 */
long PushButton::getTimeOfCurrentRawButtonState()
{
	return millis()- _lastButtonStateChangedTime;
}


bool PushButton::getButtonPressedOnlyOnce()
{
	return _buttonStatePressedOnlyOnce;
}
