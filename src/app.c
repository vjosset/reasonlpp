/******************************************************************************
 
 Copyright (c) 2015, Focusrite Audio Engineering Ltd.
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 * Neither the name of Focusrite Audio Engineering Ltd., nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 *****************************************************************************/

//______________________________________________________________________________
//
// Headers
//______________________________________________________________________________

#include "app.h"

/*
Map:
	
		+---+---+---+---+---+---+---+---+
	    | 91| 92| 93| 94| 95| 96| 97| 98|
	+---+---+---+---+---+---+---+---+---+---+
	| 80| 81| 82| 83| 84| 85| 86| 87| 88| 89+
	+---+---+---+---+---+---+---+---+---+---+
	| 70| 71| 72| 73| 74| 75| 76| 77| 78| 79+
	+---+---+---+---+---+---+---+---+---+---+
	| 60| 61| 62| 63| 64| 65| 66| 67| 68| 69+
	+---+---+---+---+---+---+---+---+---+---+
	| 50| 51| 52| 53| 54| 55| 56| 57| 58| 59+
	+---+---+---+---+---+---+---+---+---+---+
	| 40| 42| 42| 43| 44| 45| 46| 47| 48| 49+
	+---+---+---+---+---+---+---+---+---+---+
	| 30| 31| 32| 33| 34| 35| 36| 37| 38| 39+
	+---+---+---+---+---+---+---+---+---+---+
	| 20| 21| 22| 23| 24| 25| 26| 27| 28| 29+
	+---+---+---+---+---+---+---+---+---+---+
	| 10| 11| 12| 13| 14| 15| 16| 17| 18| 19+
	+---+---+---+---+---+---+---+---+---+---+
	    | 01| 02| 03| 04| 05| 06| 07| 08|
	    +---+---+---+---+---+---+---+---+


	    +---+---+---+---+---+---+---+---+
	    | UP|DWN|   |   | M1| M2| M3| M4|
	+---+---+---+---+---+---+---+---+---+---+
	|   | E3| F3|F#3| G3| F1| F2| F3| F4|CH8+
	+---+---+---+---+---+---+---+---+---+---+
	|   | C3|C#3| D3|D#3| F1| F2| F3| F4|CH7+
	+---+---+---+---+---+---+---+---+---+---+
	|   |G#2| A2|A#2| B2| F1| F2| F3| F4|CH6+
	+---+---+---+---+---+---+---+---+---+---+
	|   | E2| F2|F#2| G2| F1| F2| F3| F4|CH5+
	+---+---+---+---+---+---+---+---+---+---+
	|   | C2|C#2| D2|D#2| F1| F2| F3| F4|CH4+
	+---+---+---+---+---+---+---+---+---+---+
	|   |G#1| A1|A#1| B1| F1| F2| F3| F4|CH3+
	+---+---+---+---+---+---+---+---+---+---+
	|   | E1| F1|F#1| G1| F1| F2| F3| F4|CH2+
	+---+---+---+---+---+---+---+---+---+---+
	|VEL| C1|C#1| D1|D#1| T1| T2| T3| T4|CH1+
	+---+---+---+---+---+---+---+---+---+---+
	    |   |   |   |   |   |STP|PLY|REC|
	    +---+---+---+---+---+---+---+---+

*/

// Define bool type
typedef int bool;
#define true 1
#define false 0

// Current octave
static u8 current_octave = 1;

//Full velocity toggle
bool full_velocity = false;

//Midi channel for output
u8 channel_number = 0;

//Current page/mode
u8 mode = 0;

//Button shortcuts
#define BTN_OCTAVEUP 		TOP1
#define BTN_OCTAVEDOWN 		TOP2
#define BTN_TOGGLEFULLVEL 	LEFT1
#define BTN_PLAY			BOTTOM6
#define BTN_STOP			BOTTOM7
#define BTN_REC				BOTTOM8

//Page/Mode Shortcuts
#define MODE_NOTES			0
#define MODE_MIXER			1
#define MODE_FX				2

static u8 color_util_on[3]	= {63, 0, 63};
static u8 color_util_off[3]	= { 4, 0,  4};

//Color constants
static u8 color_off[3] 		= { 0,  0,  0};
static u8 color_wht[3] 		= {63, 63, 63};

static u8 color_red[3] 		= {63,  0,  0};
static u8 color_red_dark[3]	= { 4,  0,  0};
static u8 color_grn[3] 		= { 0, 63,  0};
static u8 color_blu[3] 		= { 0,  0, 63};

static u8 color_ylw[3] 		= {63, 63,  0};
static u8 color_org[3] 		= {63, 32,  0};
static u8 color_dor[3] 		= {63, 16,  0};

//Color for button presses
static u8 color_pressed[3]	= {63, 63, 63};

//Colors for toggles
static u8 color_toggle_on[3] = {0, 63, 0};
static u8 color_toggle_off[3] = {0, 4, 0};

//Colors for faders
static u8 color_fader_on[3] = {0, 0, 63};
static u8 color_fader_off[3] = {0, 0, 4};

//Default map of pad colors (init and returning to default state)
static u8 default_color_map[BUTTON_COUNT][3] = 
{  
	{ 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, {63,  0,  0}, {63,  0,  0}, {63,  0,  0}, { 0,  0,  0},
	{ 4,  0,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, { 0,  4,  0}, { 0,  4,  0}, { 0,  4,  0}, { 0,  4,  0}, {63,  0, 63},
	{ 0,  0,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 4,  0,  4},
	{ 0,  0,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 16,  0}, {63, 16,  0}, {63, 16,  0}, {63, 16,  0}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 16,  0}, {63, 16,  0}, {63, 16,  0}, {63, 16,  0}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63,  0,  0}, {63,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, {63,  0, 63}, {63,  0, 63}, {63,  0, 63}, {63,  0, 63}, { 0,  0,  0}
};

//Current map of pad colors
static u8 color_map[BUTTON_COUNT][3] = 
{  
	{ 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, {63,  0,  0}, {63,  0,  0}, {63,  0,  0}, { 0,  0,  0},
	{ 4,  0,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, { 0,  4,  0}, { 0,  4,  0}, { 0,  4,  0}, { 0,  4,  0}, {63,  0, 63},
	{ 0,  0,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, {63, 63,  0}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 0,  0, 63}, { 4,  0,  4},
	{ 0,  0,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, {63, 32,  0}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 16,  0}, {63, 16,  0}, {63, 16,  0}, {63, 16,  0}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63, 16,  0}, {63, 16,  0}, {63, 16,  0}, {63, 16,  0}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 0,  0,  4}, { 4,  0,  4}, 
	{ 0,  0,  0}, {63,  0,  0}, {63,  0,  0}, { 0,  0,  0}, { 0,  0,  0}, {63,  0, 63}, {63,  0, 63}, {63,  0, 63}, {63,  0, 63}, { 0,  0,  0}
};

static const u8 note_map[BUTTON_COUNT] =
{
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0, 24, 25, 26, 27,  0,  0,  0,  0,  0, 
	 0, 28, 29, 30, 31,  0,  0,  0,  0,  0, 
	 0, 32, 33, 34, 35,  0,  0,  0,  0,  0, 
	 0, 36, 37, 38, 39,  0,  0,  0,  0,  0, 
	 0, 40, 41, 42, 43,  0,  0,  0,  0,  0, 
	 0, 44, 45, 46, 47,  0,  0,  0,  0,  0, 
	 0, 48, 49, 50, 51,  0,  0,  0,  0,  0, 
	 0, 52, 53, 54, 55,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

//Maps x-index of fader button to CC number to send
static const u8 fader_cc_map[10] = { 0, 0, 0, 0, 0, 71, 72, 73, 74, 0 };

//Maps y-index of fader button to CC value to send
static const u8 fader_value_map[10] = { 0, 0, 0, 21, 42, 63, 84, 105, 127, 127 };

//Holds values of the toggles
static const u8 toggle_cc_map[BUTTON_COUNT] =
{
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0, 75, 76, 77, 78,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


//Holds values of the toggles
static u8 toggle_value_map[BUTTON_COUNT] =
{
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

//-----------------------------------------------------------------------------------------
//                                HELPER FUNCTIONS
//-----------------------------------------------------------------------------------------

/**
 * Returns a value indicating whether the specified button value indicates a press event
 */
bool is_press(u8 value) {
	return value != 0;
}

/**
 * Returns a value indicating whether the specified button value indicates a release event
 */
bool is_release(u8 value) {
	return value == 0;
}

/**
 * Draws the current color map to the hardward layer
 */
void draw_color_map() {
	for (u8 i = 0; i < BUTTON_COUNT; i++) {
		//Send color_map[i] to the button
		hal_plot_led(TYPEPAD, i, color_map[i][0], color_map[i][1], color_map[i][2]);
	}
}

/**
 * Returns a value indicating whether the specified button index is a pad (as opposed to a util button)
 */
bool is_pad(u8 index) {
	return index > 10 && index < 89 && index % 10 != 0 && index % 10 != 9;
}

/**
 * Returns a value indicating whether the specified button index is a note
 */
bool is_note(u8 index) {
	return is_pad(index) && (index % 10) < 5;
}

/**
 * Returns a value indicating whether the specified button index is a fader
 */
bool is_fader(u8 index) {
	return is_pad(index) && (index % 10) > 4 && (index / 10) > 1;
}

/**
 * Returns a value indicating whether the specified button index is a toggle
 */
bool is_toggle(u8 index) {
	return is_pad(index) && (index % 10) > 4 && (index / 10) == 1;
}

/**
 * Returns a value indicating whether the specified button index is a utility button
 */
bool is_util(u8 index) {
	return !is_pad(index);
}

/**
 * Sets the color on color_map for the specified index to the specified color
 */
void set_color_map(u8 index, u8 color[]) {
	color_map[index][0] = color[0];
	color_map[index][1] = color[1];
	color_map[index][2] = color[2];
}

/**
 * Increments the current octave value
 */
void octave_up() {
	if (current_octave < 7) {
		current_octave++;
	}
	
	if (current_octave == 7) {
		//Disable octave up button, already at max
		set_color_map(BTN_OCTAVEUP, color_util_off);
	}
	
	//Enable octave down button
	set_color_map(BTN_OCTAVEDOWN, color_util_on);
}

/**
 * Decrements the current octave value
 */
void octave_down() {
	if (current_octave > 0) {
		current_octave--;
	}
	
	if (current_octave == 0) {
		//Disable octave down button, already at min
		set_color_map(BTN_OCTAVEDOWN, color_util_off);
	}
	
	//Enable octave up button
	set_color_map(BTN_OCTAVEUP, color_util_on);
}

/**
 * Toggles full velocity mode
 */
void toggle_full_velocity() {
	full_velocity = !full_velocity;
	set_color_map(BTN_TOGGLEFULLVEL, full_velocity ? color_red : color_red_dark);
}

/**
 * Sets the current channel number (0-based)
 */
void set_channel(u8 channel) {
	channel_number = channel;
	
	//Set the LED to indicate the current channel
	u8 channel_pad = 0;
	
	//Clear all the channel indicators
	set_color_map(RIGHT1, color_util_off);
	set_color_map(RIGHT2, color_util_off);
	set_color_map(RIGHT3, color_util_off);
	set_color_map(RIGHT4, color_util_off);
	set_color_map(RIGHT5, color_util_off);
	set_color_map(RIGHT6, color_util_off);
	set_color_map(RIGHT7, color_util_off);
	set_color_map(RIGHT8, color_util_off);
	
	switch(channel_number) {
		case 0:
			set_color_map(RIGHT1, color_util_on);
			break;
		case 1:
			set_color_map(RIGHT2, color_util_on);
			break;
		case 2:
			set_color_map(RIGHT3, color_util_on);
			break;
		case 3:
			set_color_map(RIGHT4, color_util_on);
			break;
		case 4:
			set_color_map(RIGHT5, color_util_on);
			break;
		case 5:
			set_color_map(RIGHT6, color_util_on);
			break;
		case 6:
			set_color_map(RIGHT7, color_util_on);
			break;
		case 7:
			set_color_map(RIGHT8, color_util_on);
			break;
	}
}

//-----------------------------------------------------------------------------------------
//                                EVENTS
//-----------------------------------------------------------------------------------------

void event_note(u8 index, u8 value) {
	if (is_press(value)) {
		//Send MIDI note on (with full_velocity check)
		hal_send_midi(USBMIDI, NOTEON | channel_number, note_map[index] + (12 * current_octave), full_velocity ? 127 : value);
		
		//Set the pad to its pressed color
		set_color_map(index, color_pressed);
	} else {
		//Send MIDI note off
		hal_send_midi(USBMIDI, NOTEON | channel_number, note_map[index] + (12 * current_octave), 0);
		
		//Reset the note pad to its default color
		set_color_map(index, default_color_map[index]);
	}
}

void event_toggle(u8 index, u8 value) {
	if (is_press(value)) {
		//Send MIDI CC value
		u8 cc_number = toggle_cc_map[index];
		u8 cc_value = toggle_value_map[index];
		
		//Now negate the value
		if (cc_value == 127) {
			cc_value = 0;
		} else {
			cc_value = 127;
		}
		toggle_value_map[index] = cc_value;
		
		//Send MIDI message
		hal_send_midi(USBMIDI, CC | channel_number, cc_number, cc_value);
		
		//Set the color of the toggle
		set_color_map(index, cc_value == 0 ? color_toggle_off : color_toggle_on);
	}
}

void event_fader(u8 index, u8 value) {
	if (is_press(value)) {
		//Send MIDI CC value
		u8 y_index = index / 10;
		u8 x_index = index % 10;
		
		u8 cc_value = fader_value_map[y_index];
		u8 cc_number = fader_cc_map[x_index];
		
		hal_send_midi(USBMIDI, CC | channel_number, cc_number, cc_value);
		
		//Current button will be lit (color_fader_on)
		//Buttons below will be lit (color_fader_on)
		//Buttons above will be off (color_fader_off)
		
		for (u8 i = index; i > 20; i -= 10) {
			set_color_map(i, color_fader_on);
		}
		for (u8 i = index + 10; i < 90; i += 10) {
			set_color_map(i, color_fader_off);
		}
	} else {
		//Release - Reset color
	}
}

void event_util(u8 index, u8 value) {
	if (is_press(value)) {
		switch (index) {
			case BTN_OCTAVEUP:
				octave_up();
				break;
			case BTN_OCTAVEDOWN:
				octave_down();
				break;
			case BTN_TOGGLEFULLVEL:
				toggle_full_velocity();
				break;
			case BTN_PLAY:
				//Not implemented
				hal_send_midi(USBMIDI, MIDISTART | channel_number, 0x00, 0x00);
				break;
			case BTN_STOP:
				//Not implemented
				hal_send_midi(USBMIDI, MIDISTOP | channel_number, 0x00, 0x00);
				break;
			case BTN_REC:
				//Not implemented
				hal_send_midi(USBMIDI, 0xBF | channel_number, 0x00, 0x00);
				break;
			case RIGHT1:
				//Set channel 1 (0)
				set_channel(0);
				break;
			case RIGHT2:
				//Set channel 2
				set_channel(1);
				break;
			case RIGHT3:
				//Set channel 3
				set_channel(2);
				break;
			case RIGHT4:
				//Set channel 4
				set_channel(3);
				break;
			case RIGHT5:
				//Set channel 5
				set_channel(4);
				break;
			case RIGHT6:
				//Set channel 6
				set_channel(5);
				break;
			case RIGHT7:
				//Set channel 7
				set_channel(6);
				break;
			case RIGHT8:
				//Set channel 8
				set_channel(7);
				break;
			
		}
	}
}

/**
 * Called when the user presses or releases any button or pad on the control surface.
 *
 * @param type - TYPEPAD for normal pads or buttons, TYPESETUP for the Setup button
 * @param index - The index of the button, as detailed at the start of this file.
 * @param value - 0 for release, nonzero for press.
 */
void app_surface_event(u8 type, u8 index, u8 value) {
	if (type == TYPEPAD) {
		//Mark this button as pressed
		//set_color_map(index, color_pressed);
		
		if (is_util(index)) {
			//This is a utility button
			event_util(index, value);
		} else if (is_fader(index)) {
			//This is a fader button
			event_fader(index, value);
		} else if (is_note(index)) {
			//This is a note button
			event_note(index, value);
		} else if (is_toggle(index)) {
			//This is a toggle button
			event_toggle(index, value);
		}
		
		//Draw color_map
		draw_color_map();
	}
}

//______________________________________________________________________________

void app_midi_event(u8 port, u8 status, u8 d1, u8 d2) {
}

//______________________________________________________________________________

void app_sysex_event(u8 port, u8 * data, u16 count) {
}

//______________________________________________________________________________

void app_aftertouch_event(u8 index, u8 value) {
    if (is_note(index)) {
		//This is a note pad
		hal_send_midi(USBMIDI, POLYAFTERTOUCH | channel_number, index, value);
	}
}

//______________________________________________________________________________

void app_cable_event(u8 type, u8 value) {
}

//______________________________________________________________________________

void app_timer_event() {
}

//______________________________________________________________________________

void app_init(const u16 *adc_raw) {
	//Set up starting octave
	current_octave = 1;
	
	//Draw the default color map
	draw_color_map();
}
