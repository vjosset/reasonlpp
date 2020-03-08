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

// ______________________________________________________________________________
// 
// Headers
// ______________________________________________________________________________

#include "app.h"

/*
	CC VALUES
		Combinator Mode:
			Faders:		20	21	22	23
			Toggles:	24	25	26	27
		X/Y Mode:
			X:			30
			Y:			31
		
		Global:
			LOOP		14
			STOP		17
			PLAY		18
			RECORD		19
			FF			16
			RW			15
			
			PREVTRACK	38
			NEXTTRACK	39
			PREVPATCH	40
			NEXTPATCH	41

*/

/*
Map:
	
		+---+---+---+---+---+---+---+---+
	    | 91| 92| 93| 94| 95| 96| 97| 98|
	+---+---+---+---+---+---+---+---+---+---+
	| 80| 81| 82| 83| 84| 85| 86| 87| 88| 89|
	+---+---+---+---+---+---+---+---+---+---+
	| 70| 71| 72| 73| 74| 75| 76| 77| 78| 79|
	+---+---+---+---+---+---+---+---+---+---+
	| 60| 61| 62| 63| 64| 65| 66| 67| 68| 69|
	+---+---+---+---+---+---+---+---+---+---+
	| 50| 51| 52| 53| 54| 55| 56| 57| 58| 59|
	+---+---+---+---+---+---+---+---+---+---+
	| 40| 42| 42| 43| 44| 45| 46| 47| 48| 49|
	+---+---+---+---+---+---+---+---+---+---+
	| 30| 31| 32| 33| 34| 35| 36| 37| 38| 39|
	+---+---+---+---+---+---+---+---+---+---+
	| 20| 21| 22| 23| 24| 25| 26| 27| 28| 29|
	+---+---+---+---+---+---+---+---+---+---+
	| 10| 11| 12| 13| 14| 15| 16| 17| 18| 19|
	+---+---+---+---+---+---+---+---+---+---+
	    | 01| 02| 03| 04| 05| 06| 07| 08|
	    +---+---+---+---+---+---+---+---+


	    +---+---+---+---+---+---+---+---+
	    | + | - | PT| NT|   | NM|   | MS|
	+---+---+---+---+---+---+---+---+---+---+
	|SHF| E3| F3|F#3| G3| F1| F2| F3| F4|   |
	+---+---+---+---+---+---+---+---+---+---+
	|   | C3|C#3| D3|D#3| F1| F2| F3| F4|   |
	+---+---+---+---+---+---+---+---+---+---+
	|   |G#2| A2|A#2| B2| F1| F2| F3| F4|   |
	+---+---+---+---+---+---+---+---+---+---+
	|   | E2| F2|F#2| G2| F1| F2| F3| F4|   |
	+---+---+---+---+---+---+---+---+---+---+
	|   | C2|C#2| D2|D#2| F1| F2| F3| F4|   |
	+---+---+---+---+---+---+---+---+---+---+
	|   |G#1| A1|A#1| B1| F1| F2| F3| F4|   |
	+---+---+---+---+---+---+---+---+---+---+
	|   | E1| F1|F#1| G1| F1| F2| F3| F4|   |
	+---+---+---+---+---+---+---+---+---+---+
	|   | C1|C#1| D1|D#1| T1| T2| T3| T4|   |
	+---+---+---+---+---+---+---+---+---+---+
	    |VEL|   |   |   |   |STP|PLY|REC|
	    +---+---+---+---+---+---+---+---+

	NM: Node mode - Toggle Notes vs Kong
	MS: Mode switch - Cycle mode (Notes, Combi, X/Y, Mixer)
	
*/

// Define bool type
typedef int bool;
#define true 1
#define false 0

// Button shortcuts
#define BTN_TOGGLELEVEL 		BOTTOM1
#define BTN_LOOP				BOTTOM3
#define BTN_RW					BOTTOM4
#define BTN_FF					BOTTOM5
#define BTN_STOP				BOTTOM6		// Grey
#define BTN_PLAY				BOTTOM7		// White
#define BTN_RECORD				BOTTOM8		// Red
#define BTN_UP					TOP1
#define BTN_DOWN				TOP2
#define BTN_LEFT				TOP3
#define BTN_RIGHT				TOP4
#define BTN_SHIFT				LEFT8
#define BTN_MODESWITCH			TOP8
#define BTN_NOTEMODE			TOP6

// CC Numbers
#define MIDI_CC_PREVTRACK		20
#define MIDI_CC_NEXTTRACK		21

#define MIDI_CC_X				44
#define MIDI_CC_Y				45

#define MIDI_TRANSPORT_STOP		10
#define MIDI_TRANSPORT_PLAY		16
#define MIDI_TRANSPORT_REC		12
#define MIDI_TRANSPORT_LOOP		13
#define MIDI_TRANSPORT_FF		14
#define MIDI_TRANSPORT_RW		15

// Constants
#define MIDI_CHANNEL			1

// Page/Mode Shortcuts
#define MODE_NOTES				0
#define MODE_COMBI				1
#define MODE_XY					2
#define MODE_MIXER				3
#define MODE_COUNT				4

// Node modes
#define NOTE_MODE_KEYBOARD		0
#define NOTE_MODE_KONG			1
#define NOTE_MODE_COUNT			2

// Current transpose offset
static int transpose = 36;

// Full velocity toggle
bool is_full_velocity = true;

// Tracks whether we are currently recording or not
bool is_recording = false;

// Tracks whether "SHIFT" is pressed
bool is_shift_pressed = false;

bool is_loop_on = true;

// Current mode
u8 current_mode = MODE_NOTES;
u8 current_note_mode = NOTE_MODE_KEYBOARD;

// Color lookup constants
#define COLOR_OFF				 0
#define COLOR_UTIL_OFF			 1
#define COLOR_UTIL_ON			 2
#define COLOR_FADER_OFF			 3
#define COLOR_FADER_ON			 4
#define COLOR_TOGGLE_OFF		 5
#define COLOR_TOGGLE_ON			 6
#define COLOR_NOTE_OFF_1		 7
#define COLOR_NOTE_OFF_1_ALT	 8
#define COLOR_NOTE_OFF_2		 9
#define COLOR_NOTE_OFF_2_ALT	10
#define COLOR_NOTE_OFF_3		11
#define COLOR_NOTE_OFF_3_ALT	12
#define COLOR_NOTE_OFF_4		13
#define COLOR_NOTE_OFF_4_ALT	14
#define COLOR_NOTE_OFF_5		15
#define COLOR_NOTE_OFF_5_ALT	16
#define COLOR_PRESS				17
#define COLOR_PLAY				18
#define COLOR_STOP				19
#define COLOR_RECORD_OFF		20
#define COLOR_RECORD_ON			21
#define COLOR_XY_OFF			22
#define COLOR_XY_ON				23

static u8 progressive_period = 5;
static u8 progressive_current_step = 0;

// Standard colors lookup
static u8 colors[24][3] = 
{
	{ 0,  0,  0}, // COLOR_OFF
	{ 4,  0,  8}, // COLOR_UTIL_OFF
	{63,  0, 63}, // COLOR_UTIL_ON
	{ 0,  0,  8}, // COLOR_FADER_OFF
	{ 0,  0, 63}, // COLOR_FADER_ON
	{ 0,  8,  0}, // COLOR_TOGGLE_OFF
	{ 0, 63,  0}, // COLOR_TOGGLE_ON
	{63, 63,  0}, // COLOR_NOTE_OFF_1
	{ 4,  4,  0}, // COLOR_NOTE_OFF_1_ALT
	{63, 31,  0}, // COLOR_NOTE_OFF_2
	{ 4,  2,  0}, // COLOR_NOTE_OFF_2_ALT
	{63, 15,  0}, // COLOR_NOTE_OFF_3
	{ 4,  1,  0}, // COLOR_NOTE_OFF_3_ALT
	{63,  7,  0}, // COLOR_NOTE_OFF_4
	{63,  0,  0}, // COLOR_NOTE_OFF_4_ALT
	{63,  0,  0}, // COLOR_NOTE_OFF_5
	{63,  0,  0}, // COLOR_NOTE_OFF_5_ALT
	{63, 63, 63}, // COLOR_PRESS
	{63, 63, 63}, // COLOR_PLAY
	{ 8,  8,  8}, // COLOR_STOP
	{ 8,  0,  0}, // COLOR_RECORD_OFF
	{63,  0,  0}, // COLOR_RECORD_ON
	{ 0,  0, 63}, // COLOR_XY_OFF
	{63, 63, 63}  // COLOR_XY_ON
};

static u8 note_kong_values[100] = 
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  1,  2,  3, 32, 33, 34, 35,  0,
	0,  4,  5,  6,  7, 36, 37, 38, 39,  0,
	0,  8,  9, 10, 11, 40, 41, 42, 43,  0,
	0, 12, 13, 14, 15, 44, 45, 46, 47,  0,
	0, 16, 17, 18, 19, 48, 49, 50, 51,  0,
	0, 20, 21, 22, 23, 52, 53, 54, 55,  0,
	0, 24, 25, 26, 27, 56, 57, 58, 59,  0,
	0, 28, 29, 30, 31, 60, 61, 62, 63,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

static u8 note_keyboard_values[100] = 
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  1,  2,  3, 24, 25, 26, 27,  0,
	0,  4,  5,  6,  7, 28, 29, 30, 31,  0,
	0,  8,  9, 10, 11, 32, 33, 34, 35,  0,
	0, 12, 13, 14, 15, 36, 37, 38, 39,  0,
	0, 16, 17, 18, 19, 40, 41, 42, 43,  0,
	0, 20, 21, 22, 23, 44, 45, 46, 47,  0,
	0, 24, 25, 26, 27, 48, 49, 50, 51,  0,
	0, 28, 29, 30, 31, 52, 53, 54, 55,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

static u8 note_keyboard_colors[100] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3,  0,
	0, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3,  0,
	0, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3,  0,
	0, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4,  0,
	0, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4,  0,
	0, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4,  0,
	0, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_5, COLOR_NOTE_OFF_5, COLOR_NOTE_OFF_5, COLOR_NOTE_OFF_5,  0,
    0, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_5, COLOR_NOTE_OFF_5, COLOR_NOTE_OFF_5, COLOR_NOTE_OFF_5,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

static u8 note_kong_colors[100] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3,  0,
	0, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3,  0,
	0, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3,  0,
	0, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_1, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3, COLOR_NOTE_OFF_3,  0,
	0, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4,  0,
	0, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4,  0,
	0, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4,  0,
	0, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_2, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4, COLOR_NOTE_OFF_4,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

// Lookup of current CC values
static u8 cc_actual_values[128];

static u8 cc_target_values[128];

static u8 progressive_ccs[8] = {40, 41, 42, 43, 44, 45, 46, 47};

static u8 xy_cc_actual_values[8] = {0, 18, 36, 54, 72, 90, 108, 126};

// -----------------------------------------------------------------------------------------
//                                HELPER METHODS
// -----------------------------------------------------------------------------------------

u8 get_pad_note(u8 index) {
	if (current_note_mode == NOTE_MODE_KONG) {
		// Kong mode
		return note_kong_values[index] + transpose;
	} else {
		// Keyboard mode
		return note_keyboard_values[index] + transpose;
	}
}

u8 get_pad_fader_value(u8 index) {
	// Map pad row to Midi CC value
	u8 y_index = index / 10;
	
	if (y_index < 2) {
		return 0;
	}
	
	y_index -= 2;
	
	return y_index * 21;
}

u8 get_pad_fader_cc_number(u8 index) {
	return 39 + (index % 10);
}

u8 get_pad_toggle_cc_number(u8 index) {
	return 49 + (index % 10);
}

bool pad_is_util(u8 index) {
	u8 x_index = index % 10;
	u8 y_index = index / 10;
	
	return x_index == 0 || x_index == 9 || y_index == 0 || y_index == 9;
}

bool pad_is_note(u8 index) {
	if (pad_is_util(index) || current_mode == MODE_XY || current_mode == MODE_MIXER) {
		return false;
	}
	
	switch (current_mode) {
		case MODE_NOTES:
			return true;
		case MODE_COMBI:
			return index % 10 < 5;
		default:
			return false;
	}
}

bool pad_is_fader(u8 index) {
	if (pad_is_util(index) || current_mode == MODE_XY) {
		return false;
	}
	
	u8 x_index = index % 10;
	u8 y_index = index / 10;
	
	switch (current_mode) {
		case MODE_MIXER:
			return y_index > 1;
		case MODE_COMBI:
			return x_index > 4 && y_index > 1;
		default:
			return false;
	}
}

bool pad_is_toggle(u8 index) {
	if (pad_is_util(index) || current_mode == MODE_XY) {
		return false;
	}
	
	u8 x_index = index % 10;
	u8 y_index = index / 10;
	
	switch (current_mode) {
		case MODE_MIXER:
			return y_index == 1;
		case MODE_COMBI:
			return y_index == 1 && x_index > 4;
		default:
			return false;
	}
}

void octave_up() {
	transpose += 12;
	if (transpose > 72) {
		transpose =  72;
	}
}

void octave_down() {
	transpose -= 12;
	if (transpose < 0) {
		transpose = 0;
	}
}

void transpose_up() {
	transpose++;
	if (transpose > 72) {
		transpose =  72;
	}
}

void transpose_down() {
	transpose--;
	if (transpose < 0) {
		transpose = 0;
	}
}

// -----------------------------------------------------------------------------------------
//                                DRAWING METHODS
// -----------------------------------------------------------------------------------------

void set_color(u8 index, u8 color_index) {
	hal_plot_led(TYPEPAD, index, colors[color_index][0], colors[color_index][1], colors[color_index][2]);
}

void draw_util_buttons() {
	// Draw the octave selection buttons
	set_color(BTN_UP, COLOR_UTIL_ON);
	set_color(BTN_DOWN, COLOR_UTIL_ON);
	
	// Draw the track selection buttons
	set_color(BTN_LEFT, COLOR_UTIL_ON);
	set_color(BTN_RIGHT, COLOR_UTIL_ON);
	
	// Draw the "Shift" button
	set_color(BTN_SHIFT, is_shift_pressed ? COLOR_PRESS : COLOR_UTIL_ON);
	
	// Draw the mode selection button
	set_color(BTN_MODESWITCH, COLOR_UTIL_ON);
	
	// Draw the note mode selection button
	set_color(BTN_NOTEMODE, (current_note_mode == NOTE_MODE_KEYBOARD ? COLOR_PRESS : COLOR_UTIL_ON));
	
	// Draw the transport buttons
	set_color(BTN_LOOP, is_loop_on ? COLOR_UTIL_ON : COLOR_UTIL_OFF);
	set_color(BTN_FF, COLOR_UTIL_ON);
	set_color(BTN_RW, COLOR_UTIL_ON);
	set_color(BTN_PLAY, COLOR_PLAY);
	set_color(BTN_STOP, COLOR_STOP);
	set_color(BTN_RECORD, is_recording ? COLOR_RECORD_ON : COLOR_RECORD_OFF);
	
	// Draw the full velocity button
	set_color(BTN_TOGGLELEVEL, is_full_velocity ? COLOR_UTIL_ON : COLOR_UTIL_OFF);
}

void draw_pads_note() {
	// Draw notes
	for (u8 x = 1; x < 9; x++) {
		for (u8 y = 1; y < 9; y++) {
			if (current_note_mode == NOTE_MODE_KEYBOARD) {
				// Keyboard mode
				set_color((y * 10) + x, note_keyboard_colors[(y * 10) + x]);
			} else {
				// Kong mode
				set_color((y * 10) + x, note_kong_colors[(y * 10) + x]);
			}
		}
	}
}

void draw_pads_combi() {
	// Draw notes
	for (u8 x = 1; x < 5; x++) {
		for (u8 y = 1; y < 9; y++) {
			if (current_note_mode == NOTE_MODE_KEYBOARD) {
				// Keyboard mode
				set_color((y * 10) + x, note_keyboard_colors[(y * 10) + x]);
			} else {
				// Kong mode
				set_color((y * 10) + x, note_kong_colors[(y * 10) + x]);
			}
		}
	}
	
	// Draw faders
	for (u8 x = 5; x < 9; x++) {
		for (u8 y = 2; y < 9; y++) {
			if (cc_target_values[get_pad_fader_cc_number((y * 10) + x)] >= get_pad_fader_value((y * 10) + x)) {
				// Set this fader button to "on"
				set_color((y * 10) + x, COLOR_FADER_ON);
			} else {
				// Set this fader button to "off"
				set_color((y * 10) + x, COLOR_FADER_OFF);
			}
		}
	}
	
	// Draw toggles
	for (u8 x = 5; x < 9; x++) {
		u8 y = 1;
		
		if (cc_target_values[get_pad_toggle_cc_number((y * 10) + x)] > 0) {
			// Set this toggle button to "on"
			set_color((y * 10) + x, COLOR_TOGGLE_ON);
		} else {
			// Set this toggle button to "off"
			set_color((y * 10) + x, COLOR_TOGGLE_OFF);
		}
	}
}

void draw_pads_mixer() {
	// Draw faders
	for (u8 x = 1; x < 9; x++) {
		for (u8 y = 2; y < 9; y++) {
			if (cc_target_values[get_pad_fader_cc_number((y * 10) + x)] >= get_pad_fader_value((y * 10) + x)) {
				// Set this fader button to "on"
				set_color((y * 10) + x, COLOR_FADER_ON);
			} else {
				// Set this fader button to "off"
				set_color((y * 10) + x, COLOR_FADER_OFF);
			}
		}
	}
	
	// Draw toggles
	for (u8 x = 1; x < 9; x++) {
		u8 y = 1;
		
		if (cc_target_values[get_pad_toggle_cc_number((y * 10) + x)] > 0) {
			// Set this toggle button to "on"
			set_color((y * 10) + x, COLOR_TOGGLE_ON);
		} else {
			// Set this toggle button to "off"
			set_color((y * 10) + x, COLOR_TOGGLE_OFF);
		}
	}
}

void draw_pads_xy() {
	// All square pads start blue (COLOR_XY_OFF)
	for (u8 x = 1; x < 9; x++) {
		for (u8 y = 1; y < 9; y++) {
			// Draw this pad with default color
			set_color((y * 10) + x, COLOR_XY_OFF);
		}
	}
	
	// Current x/y column/row white (COLOR_XY_ON)
	for (u8 x = 1; x < 9; x++) {
		if (xy_cc_actual_values[x - 1] == cc_target_values[MIDI_CC_X]) {
			// This is the index of the x column to fill with "On" color
			for (u8 y = 1; y < 9; y++) {
				set_color((y * 10) + x, COLOR_XY_ON);
			}
		}
	}
	for (u8 y = 1; y < 9; y++) {
		if (xy_cc_actual_values[y - 1] == cc_target_values[MIDI_CC_Y]) {
			// This is the index of the y row to fill with "On" color
			for (u8 x = 1; x < 9; x++) {
				set_color((y * 10) + x, COLOR_XY_ON);
			}
		}
	}
}

void draw_pads() {
	switch(current_mode) {
		case MODE_NOTES:
			draw_pads_note();
			break;
		case MODE_COMBI:
			draw_pads_combi();
			break;
		case MODE_MIXER:
			draw_pads_mixer();
			break;
		case MODE_XY:
			draw_pads_xy();
			break;
		default:
			break;
	}
}


// -----------------------------------------------------------------------------------------
//                                EVENTS
// -----------------------------------------------------------------------------------------

/**
 * Called when the user presses or releases any button or pad on the control surface.
 *
 * @param type - TYPEPAD for normal pads or buttons, TYPESETUP for the Setup button
 * @param index - The index of the button, as detailed at the start of this file.
 * @param value - 0 for release, nonzero for press.
 */
void app_surface_event(u8 type, u8 index, u8 value) {
	// Only care about pads, not "setup" button
	if (type == TYPEPAD) {
		bool is_press = value > 0;
		
		u8 x_index = index % 10;
		u8 y_index = index / 10;
		
		if (pad_is_util(index)) {
			// This is a utility button - Handle it
			switch(index) {
				// SHIFT
				case BTN_SHIFT:
					is_shift_pressed = is_press;
					break;
				
				// MODES
				case BTN_MODESWITCH:
					if (is_press) {
						current_mode++;
						// Make sure we don't go beyond the expected modes (based on MODE_COUNT - 0-3)
						current_mode = current_mode % MODE_COUNT;
					}
					break;
				
				case BTN_NOTEMODE:
					if (is_press) {
						// Switch note mode - Keyboard vs Kong
						current_note_mode++;
						current_note_mode = current_note_mode % NOTE_MODE_COUNT;
					}
					break;
				
				// TRANSPORT
				case BTN_RECORD:
					if (is_press) {
						// Toggle the record mode
						is_recording = !is_recording;
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_REC, is_recording ? 127 : 0);
					}
					break;
				case BTN_PLAY:
					if (is_press) {
						// Midi play
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_PLAY, 127);
					}
					break;
				case BTN_STOP:
					if (is_press) {
						// Midi stop
						is_recording = false;
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_STOP, 127);
					}
					break;
				case BTN_LOOP:
					if (is_press) {
						// Loop toggle
						is_loop_on = !is_loop_on;
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_LOOP, is_loop_on ? 127 : 0);
					}
					break;
				case BTN_FF:
					// Fast Forward
					hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_FF, is_press ? 127 : 0);
					break;
				case BTN_RW:
					// Fast Forward
					hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_RW, is_press ? 127 : 0);
					break;
				
				// FULL VELOCITY TOGGLE
				case BTN_TOGGLELEVEL:
					if (is_press) {
						is_full_velocity = !is_full_velocity;
					}
					break;
				
				// PREV/NEXT TRACK
				case BTN_LEFT:
					if (is_press) {
						// "Left" means previous track
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_CC_PREVTRACK, 127);
					}
					break;
				case BTN_RIGHT:
					if (is_press) {
						// "Right" means next track
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_CC_NEXTTRACK, 127);
					}
					break;
				
				// OCTAVE/TRANSPOSE UP/DOWN
				case BTN_UP:
					if (is_press) {
						if (is_shift_pressed) {
							// Shift is pressed - "Up" means transpose + 1
							transpose_up();
						} else {
							// Shift is not pressed - "Up" means octave + 1
							octave_up();
						}
					}
					break;
				case BTN_DOWN:
					if (is_press) {
						if (is_shift_pressed) {
							// Shift is pressed - "Down" means transpose - 1
							transpose_down();
						} else {
							// Shift is not pressed - "Down" means octave - 1
							octave_down();
						}
					}
					break;
				
				default:
					// Do nothing
					break;
			}
				
		} else if (current_mode == MODE_XY && is_press) {
			// This is X/Y mode - set the X and Y CC values
			u8 x_cc_value = xy_cc_actual_values[x_index - 1];
			u8 y_cc_value = xy_cc_actual_values[y_index - 1];
			
			// Set the values
			cc_target_values[MIDI_CC_X] = x_cc_value;
			cc_target_values[MIDI_CC_Y] = y_cc_value;
			
			// Send the midi messages
			hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_CC_X, cc_target_values[MIDI_CC_X]);
			hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_CC_Y, cc_target_values[MIDI_CC_Y]);
		} else {
			// This is a non-util button (square button) and not X/Y mode - Handle it
			if (pad_is_fader(index)) {
				// FADER
				if (is_press) {
					// Set the fader value
					cc_target_values[get_pad_fader_cc_number(index)] = get_pad_fader_value(index);
					// Send the midi message
					hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, get_pad_fader_cc_number(index), cc_target_values[get_pad_fader_cc_number(index)]);
				}
			} else if (pad_is_toggle(index)) {
				// TOGGLE
				if (is_press) {
					// Set the toggle value
					// Get the current value
					u8 cc_value = cc_actual_values[get_pad_toggle_cc_number(index)];
					// Set the new value
					if (cc_value > 0) {
						cc_value = 0;
					} else {
						cc_value = 127;
					}
					cc_actual_values[get_pad_toggle_cc_number(index)] = cc_value;
					
					// Send the midi message
					hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, get_pad_toggle_cc_number(index), cc_value);
				}
			} else if (pad_is_note(index)) {
				// NOTE
				if (is_press) {
					// Note press - Set the "NOTE ON" color, send the midi NOTEON message
					hal_send_midi(USBMIDI, NOTEON | MIDI_CHANNEL, get_pad_note(index), is_full_velocity ? 127 : value);
				} else {
					// Note release - Set the "NOTE OFF" color, send the midi NOTEOFF message
					hal_send_midi(USBMIDI, NOTEOFF | MIDI_CHANNEL, get_pad_note(index), 0);
				}
			}
		}
		
		// Draw pads
		draw_pads();
		
		// Draw util buttons
		draw_util_buttons();
	
		// Show note buttons when pressed
		if (is_press) {
			set_color(index, COLOR_PRESS);
		}
	}
}

// ______________________________________________________________________________

void app_midi_event(u8 port, u8 status, u8 d1, u8 d2) {
	// Incoming MIDI - do nothing
}

// ______________________________________________________________________________

void app_sysex_event(u8 port, u8 * data, u16 count) {
}

// ______________________________________________________________________________

void app_aftertouch_event(u8 index, u8 value) {
    if (pad_is_note(index)) {
		// This is a note pad - Send aftertouch value
		hal_send_midi(USBMIDI, POLYAFTERTOUCH | MIDI_CHANNEL, index, value);
	}
}

// ______________________________________________________________________________

void app_cable_event(u8 type, u8 value) {
}

// ______________________________________________________________________________

void app_timer_event() {
	// Handle progressive instead of immediate CC updates (e.g. go from 0 to 128 progressively)
	progressive_current_step = (progressive_current_step + 1) % progressive_period;
	if (progressive_current_step == 0) {
		for (u8 i = 0; i < sizeof(progressive_ccs); i++) {
			if (cc_actual_values[progressive_ccs[i]] < cc_target_values[progressive_ccs[i]]) {
				// Actual CC value is not yet at its target value
				//  Progressively match the target value
				cc_actual_values[progressive_ccs[i]] = cc_actual_values[progressive_ccs[i]] + 1;
				
				// Send the updated value to the LPP
				hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, progressive_ccs[i], cc_actual_values[progressive_ccs[i]]);
			} else if (cc_actual_values[progressive_ccs[i]] > cc_target_values[progressive_ccs[i]]) {
				// Actual CC value is not yet at its target value
				//  Progressively match the target value
				cc_actual_values[progressive_ccs[i]] = cc_actual_values[progressive_ccs[i]] - 1;
				
				// Send the updated value to the LPP
				hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, progressive_ccs[i], cc_actual_values[progressive_ccs[i]]);
			}
		}
	}
}

// ______________________________________________________________________________

void app_init(const u16 *adc_raw) {
	// Set up starting octave/transpose
	transpose = 36;
	
	// Set up default note mode
	current_note_mode = NOTE_MODE_KEYBOARD;
	
	// Set up default values for CC at 50% (63)
	for (u8 i = 0; i < 128; i++) {
		cc_actual_values[i] = 63;
		cc_target_values[i] = 63;
	}
	
	// Set up default values for toggles (off)
	for (u8 i = 1; i <= 8; i++) {
		cc_actual_values[49 + i] = 0;
		cc_target_values[49 + i] = 0;
	}
	
	// Draw pads
	draw_pads();
	
	// Draw util buttons
	draw_util_buttons();
}
