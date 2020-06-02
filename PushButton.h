/*
 * PushButton.h
 *
 *  Created on: 22.04.2016
 *      Author: butch
 */

#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

/**
 * this class is a push button implementation. it will use INPUT_PULLUP as pinMode for the given pin.
 * It will debounce the (un)pressing of the button. Only if state stays for longer than the debounceTimeMs it will switch the state.
 *
 * real state will give debounced value if button is pressed or not
 * logical state is the logical on / off status which will be flipped by each button press. It will flip after debounce time.
 *   If the button stays pressed it will flip only once. For further flip the button needs to be relased and pressed again.
 */
class PushButton {
private:
	int _buttonPin;
	int _debounceTimeMs; // default 100ms // if state consists longer than that time, the state will switch
	long _debounceTimeStart;
	
	bool _buttonState; // real state of button (pressed / not pressed). Debounce filter has been applied.
	bool _buttonStateUnbounced;
	long _lastButtonStateChangedTime; // time of last state change of real button press
	bool _hasLogicalStateChangeBeenProcessed; // internal bool variable. Will be reset to false if button press is released.
	

	bool _logicalState; // Switch button will flip a logical state like a real on / off switch
	long _logicalStateMinTimeButtonPressed; // button needs to be pressed at least for this time to recognize the press and initiate the logical state change

	char _mode; // mode which will be used. (0: use pin input, 1: use alternate input -> set state from outside)

	bool _buttonStatePressedOnlyOnce;


public:
	PushButton(char _mode, int _buttonPin); // mode 0 for internal /use GPIO), mode 1 for external (set status directly by other source). buttonPin will be set to -1 if mode is external is optional and will be ignored.
	void setPin();
	virtual ~PushButton();
	void setDebounceTimeMs(int time);
	void processButtonState();
	bool getButtonStateRawData();
	bool getButtonStateRawDebounced();
	bool PushButton::getButtonStateRawDebouncedInputPullup(); // negated getButtonStateRawDebounced()
	bool getButtonStateLogical();
	long getTimeOfCurrentRawButtonState(); // how long is the current raw button state existing?
	bool getButtonPressedOnlyOnce(); // on long press, it returns only once true

};

#endif /* PUSHBUTTON_H_ */
