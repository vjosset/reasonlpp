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
	CC VALUES
		Device Mode:
			Faders:		20	21	22	23
			Toggles:	24	25	26	27
		Session Mode:
			Faders:		71	72	73	74	75	76	77
			Toggles:	102	103	104	105	106	107	108
		User (FX) Mode:
			Faders:		110	111	112	113	114	115	116	117
			Toggles:	118	119	28	29	30	31	86	87
		
		Global:
			LOOP		14
			STOP		17
			PLAY		18
			RECORD		19
			FF			16
			RW			15
			
			PREVTRACK	38
			NEXTTRACK	39

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
	    |   |   |   |   | M1| M2| M3| M4|
	+---+---+---+---+---+---+---+---+---+---+
	|OC7| E3| F3|F#3| G3| F1| F2| F3| F4|CH8|
	+---+---+---+---+---+---+---+---+---+---+
	|OC6| C3|C#3| D3|D#3| F1| F2| F3| F4|CH7|
	+---+---+---+---+---+---+---+---+---+---+
	|OC5|G#2| A2|A#2| B2| F1| F2| F3| F4|CH6|
	+---+---+---+---+---+---+---+---+---+---+
	|OC4| E2| F2|F#2| G2| F1| F2| F3| F4|CH5|
	+---+---+---+---+---+---+---+---+---+---+
	|OC3| C2|C#2| D2|D#2| F1| F2| F3| F4|CH4|
	+---+---+---+---+---+---+---+---+---+---+
	|OC2|G#1| A1|A#1| B1| F1| F2| F3| F4|CH3|
	+---+---+---+---+---+---+---+---+---+---+
	|OC1| E1| F1|F#1| G1| F1| F2| F3| F4|CH2|
	+---+---+---+---+---+---+---+---+---+---+
	|OC0| C1|C#1| D1|D#1| T1| T2| T3| T4|CH1|
	+---+---+---+---+---+---+---+---+---+---+
	    |VEL|   |   |   |   |STP|PLY|REC|
	    +---+---+---+---+---+---+---+---+

*/

//Define bool type
typedef int bool;
#define true 1
#define false 0

//Button shortcuts
#define BTN_OCTAVEUP 			TOP1
#define BTN_OCTAVEDOWN 			TOP2
#define BTN_TOGGLEFULLVEL 		BOTTOM1
#define BTN_STOP				BOTTOM6		//Grey
#define BTN_PLAY				BOTTOM7		//White
#define BTN_RECORD				BOTTOM8		//Red
#define BTN_PREVTRACK			TOP3
#define BTN_NEXTTRACK			TOP4
#define BTN_MODE_SESSION		TOP5
#define BTN_MODE_NOTE			TOP6
#define BTN_MODE_DEVICE			TOP7
#define BTN_MODE_USER			TOP8
#define BTN_FULL_VELOCITY		BOTTOM1

//Track selectors
#define BTN_TRACK_1				RIGHT1
#define BTN_TRACK_2				RIGHT2
#define BTN_TRACK_3				RIGHT3
#define BTN_TRACK_4				RIGHT4
#define BTN_TRACK_5				RIGHT5
#define BTN_TRACK_6				RIGHT6
#define BTN_TRACK_7				RIGHT7
#define BTN_TRACK_8				RIGHT8

//Octave selectors
#define BTN_OCTAVE_1			LEFT1
#define BTN_OCTAVE_2			LEFT2
#define BTN_OCTAVE_3			LEFT3
#define BTN_OCTAVE_4			LEFT4
#define BTN_OCTAVE_5			LEFT5
#define BTN_OCTAVE_6			LEFT6
#define BTN_OCTAVE_7			LEFT7
#define BTN_OCTAVE_8			LEFT8

//CC Numbers
#define MIDI_CC_PREVTRACK		38
#define MIDI_CC_NEXTTRACK		39

#define MIDI_TRANSPORT_LOOP		14
#define MIDI_TRANSPORT_STOP		17
#define MIDI_TRANSPORT_PLAY		18
#define MIDI_TRANSPORT_REC		19
#define MIDI_TRANSPORT_FF		16
#define MIDI_TRANSPORT_RW		15

//Constants
#define MAX_TRACKS				16
#define MIDI_CHANNEL			1

//Page/Mode Shortcuts
#define MODE_NOTE				0 //Note/keyboard
#define MODE_DEVICE				1 //Device (split notes and controls/combinator)
#define MODE_USER				2 //Sliders and toggles
#define MODE_SESSION			3 //Main mixer

//Current octave
static u8 current_octave = 1;

//Full velocity toggle
bool full_velocity = true;

//Current midi track for output
u8 current_track = 0;

//Tracks whether we are currently recording or not
bool is_recording = false;

//Holds the mode for each track
static u8 track_modes[MAX_TRACKS];

//Color lookup constants
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

//Standard colors lookup
static u8 colors[22][3] = 
{
	{ 0,  0,  0}, //COLOR_OFF
	{ 4,  0,  8}, //COLOR_UTIL_OFF
	{63,  0, 63}, //COLOR_UTIL_ON
	{ 0,  0,  8}, //COLOR_FADER_OFF
	{ 0,  0, 63}, //COLOR_FADER_ON
	{ 0,  8,  0}, //COLOR_TOGGLE_OFF
	{ 0, 63,  0}, //COLOR_TOGGLE_ON
	{63, 63,  0}, //COLOR_NOTE_OFF_1
	{ 4,  4,  0}, //COLOR_NOTE_OFF_1_ALT
	{63, 31,  0}, //COLOR_NOTE_OFF_2
	{ 4,  2,  0}, //COLOR_NOTE_OFF_2_ALT
	{63, 15,  0}, //COLOR_NOTE_OFF_3
	{ 4,  1,  0}, //COLOR_NOTE_OFF_3_ALT
	{63,  7,  0}, //COLOR_NOTE_OFF_4
	{63,  0,  0}, //COLOR_NOTE_OFF_4_ALT
	{63,  0,  0}, //COLOR_NOTE_OFF_5
	{63,  0,  0}, //COLOR_NOTE_OFF_5_ALT
	{63, 63, 63}, //COLOR_PRESS
	{63, 63, 63}, //COLOR_PLAY
	{ 8,  8,  8}, //COLOR_STOP
	{ 8,  0,  0}, //COLOR_RECORD_OFF
	{63,  0,  0}  //COLOR_RECORD_ON
};

static u8 note_kong_values[100] = 
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0, 24, 25, 26, 27, 56, 57, 58, 59,  0,
	0, 28, 29, 30, 31, 60, 61, 62, 63,  0,
	0, 32, 33, 34, 35, 64, 65, 66, 67,  0,
	0, 36, 37, 38, 39, 68, 69, 70, 71,  0,
	0, 40, 41, 42, 43, 72, 73, 74, 75,  0,
	0, 44, 45, 46, 47, 76, 77, 78, 79,  0,
	0, 48, 49, 50, 51, 80, 81, 82, 83,  0,
	0, 52, 53, 54, 55, 84, 85, 86, 87,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

static u8 note_default_values[100] = 
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0, 24, 25, 26, 27, 48, 49, 50, 51,  0,
	0, 28, 29, 30, 31, 52, 53, 54, 55,  0,
	0, 32, 33, 34, 35, 56, 57, 58, 59,  0,
	0, 36, 37, 38, 39, 60, 61, 62, 63,  0,
	0, 40, 41, 42, 43, 64, 65, 66, 67,  0,
	0, 44, 45, 46, 47, 68, 69, 70, 71,  0,
	0, 48, 49, 50, 51, 72, 73, 74, 75,  0,
	0, 52, 53, 54, 55, 76, 77, 78, 79,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

static u8 note_default_colors[100] =
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
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};

//Lookup of current CC values per track
static u8 cc_values[MAX_TRACKS][128];

//Note modes - Full keyboard vs Kong mode
#define NOTE_MODE_DEFAULT		0
#define NOTE_MODE_KONG			1
static u8 note_mode = NOTE_MODE_DEFAULT;

//-----------------------------------------------------------------------------------------
//                                HELPER METHODS
//-----------------------------------------------------------------------------------------

u8 get_pad_note(u8 index) {
	if (note_mode == NOTE_MODE_DEFAULT) {
		return note_default_values[index] + (12 * current_octave);
	} else {
		return note_kong_values[index] + (12 * current_octave);
	}
}

u8 get_pad_fader_value(u8 index) {
	//Map pad row to Midi CC value
	u8 y_index = index / 10;
	
	if (y_index < 2) {
		return 0;
	}
	
	y_index -= 2;
	
	return y_index * 21;
}

u8 get_pad_fader_cc_number(u8 index) {
	//Map pad column to Midi CC number
	/*
	u8 x_index = index % 10;
	
	x_index--;
	
	return 70 + x_index;
	*/
	
	return 69 + (index % 10);
}

u8 get_pad_toggle_cc_number(u8 index) {
	//Map pad column to Midi CC number
	/*
	u8 x_index = index % 10;
	
	x_index--;
	
	return 80 + x_index;
	*/
	
	return 79 + (index % 10);
}

bool pad_is_util(u8 index) {
	u8 x_index = index % 10;
	u8 y_index = index / 10;
	
	return x_index == 0 || x_index == 9 || y_index == 0 || y_index == 9;
}

bool pad_is_note(u8 index) {
	if (pad_is_util(index)) {
		return false;
	}
	
	switch (track_modes[current_track]) {
		case MODE_NOTE:
			return true;
		case MODE_DEVICE:
			return index % 10 < 5;
		default:
			return false;
	}
}

bool pad_is_fader(u8 index) {
	if (pad_is_util(index)) {
		return false;
	}
	
	u8 x_index = index % 10;
	u8 y_index = index / 10;
	
	switch (track_modes[current_track]) {
		case MODE_SESSION:
			return y_index > 1;
		case MODE_DEVICE:
			return x_index > 4 && y_index > 1;
		default:
			return false;
	}
}

bool pad_is_toggle(u8 index) {
	if (pad_is_util(index)) {
		return false;
	}
	
	u8 x_index = index % 10;
	u8 y_index = index / 10;
	
	switch (track_modes[current_track]) {
		case MODE_SESSION:
			return y_index == 1;
		case MODE_DEVICE:
			return y_index == 1 && x_index > 4;
		default:
			return false;
	}
}

void set_track(u8 track) {
	current_track = track;
	
	//Send "PREVTRACK" messages to get to 0
	for (u8 i = 0; i < MAX_TRACKS; i++) {
		hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_CC_PREVTRACK, 127);
	}
	
	//Sent "NEXTTRACK" messages to get to the selected track
	for (u8 i = 0; i < current_track; i++) {
		hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_CC_NEXTTRACK, 127);
	}
}

void set_octave(u8 octave) {
	current_octave = octave;
}

void set_color(u8 index, u8 color_index) {
	hal_plot_led(TYPEPAD, index, colors[color_index][0], colors[color_index][1], colors[color_index][2]);
}

void draw_util_buttons() {
	//Draw the play/stop/record buttons
	set_color(BTN_STOP, COLOR_STOP);
	set_color(BTN_PLAY, COLOR_PLAY);
	set_color(BTN_RECORD, is_recording ? COLOR_RECORD_ON : COLOR_RECORD_OFF);
	
	//Draw the track selectors
	u8 color_track_off = COLOR_UTIL_OFF;
	u8 color_track_on = COLOR_UTIL_ON;
	u8 track_index = current_track;
	if (track_index > 7) {
		track_index -= 8;
		color_track_off = COLOR_UTIL_ON;
		color_track_on = COLOR_UTIL_OFF;
	}
	
	for (u8 i = 1; i < 9; i++) {
		if (i == track_index + 1) {
			set_color((i * 10) + 9, color_track_on);
		} else {
			set_color((i * 10) + 9, color_track_off);
		}
	}
	
	//Draw the octave selectors
	for (u8 y_index = 1; y_index < 9; y_index++) {
		if (y_index == (current_octave + 1)) {
			set_color((y_index * 10), COLOR_UTIL_ON);
		} else {
			set_color((y_index * 10), COLOR_UTIL_OFF);
		}
	}
	
	//Draw the mode selectors
	set_color(BTN_MODE_SESSION, track_modes[current_track] == MODE_SESSION ? COLOR_UTIL_ON : COLOR_UTIL_OFF);
	set_color(BTN_MODE_NOTE, track_modes[current_track] == MODE_NOTE ? COLOR_UTIL_ON : COLOR_UTIL_OFF);
	set_color(BTN_MODE_DEVICE, track_modes[current_track] == MODE_DEVICE ? COLOR_UTIL_ON : COLOR_UTIL_OFF);
	//set_color(BTN_MODE_USER, track_modes[current_track] == MODE_USER ? COLOR_UTIL_ON : COLOR_UTIL_OFF);
	set_color(BTN_MODE_USER, COLOR_OFF);
	
	//Draw the transport buttons
	set_color(BTN_PLAY, COLOR_PLAY);
	set_color(BTN_STOP, COLOR_STOP);
	set_color(BTN_RECORD, is_recording ? COLOR_RECORD_ON : COLOR_RECORD_OFF);
	
	//Draw the full velocity button
	set_color(BTN_FULL_VELOCITY, full_velocity ? COLOR_UTIL_ON : COLOR_UTIL_OFF);
}

void draw_session_pads() {
	//Draw faders
	for (u8 x = 1; x < 9; x++) {
		for (u8 y = 2; y < 9; y++) {
			if (cc_values[current_track][get_pad_fader_cc_number((y * 10) + x)] >= get_pad_fader_value((y * 10) + x)) {
				//Set this fader button to "on"
				set_color((y * 10) + x, COLOR_FADER_ON);
			} else {
				//Set this fader button to "off"
				set_color((y * 10) + x, COLOR_FADER_OFF);
			}
		}
	}
	
	//Draw toggles
	for (u8 x = 1; x < 9; x++) {
		u8 y = 1;
		
		if (cc_values[current_track][get_pad_toggle_cc_number((y * 10) + x)] > 0) {
			//Set this toggle button to "on"
			set_color((y * 10) + x, COLOR_TOGGLE_ON);
		} else {
			//Set this toggle button to "off"
			set_color((y * 10) + x, COLOR_TOGGLE_OFF);
		}
	}
}

void draw_note_pads() {
	//Draw notes
	for (u8 x = 1; x < 9; x++) {
		for (u8 y = 1; y < 9; y++) {
			if (note_mode == NOTE_MODE_DEFAULT) {
				set_color((y * 10) + x, note_default_colors[(y * 10) + x]);
			} else {
				set_color((y * 10) + x, note_kong_colors[(y * 10) + x]);
			}
		}
	}
}

void draw_device_pads() {
	//Draw notes
	for (u8 x = 1; x < 5; x++) {
		for (u8 y = 1; y < 9; y++) {
			if (note_mode == NOTE_MODE_DEFAULT) {
				set_color((y * 10) + x, note_default_colors[(y * 10) + x]);
			} else {
				set_color((y * 10) + x, note_kong_colors[(y * 10) + x]);
			}
		}
	}
	
	//Draw faders
	for (u8 x = 5; x < 9; x++) {
		for (u8 y = 2; y < 9; y++) {
			if (cc_values[current_track][get_pad_fader_cc_number((y * 10) + x)] >= get_pad_fader_value((y * 10) + x)) {
				//Set this fader button to "on"
				set_color((y * 10) + x, COLOR_FADER_ON);
			} else {
				//Set this fader button to "off"
				set_color((y * 10) + x, COLOR_FADER_OFF);
			}
		}
	}
	
	//Draw toggles
	for (u8 x = 5; x < 9; x++) {
		u8 y = 1;
		
		if (cc_values[current_track][get_pad_toggle_cc_number((y * 10) + x)] > 0) {
			//Set this toggle button to "on"
			set_color((y * 10) + x, COLOR_TOGGLE_ON);
		} else {
			//Set this toggle button to "off"
			set_color((y * 10) + x, COLOR_TOGGLE_OFF);
		}
	}
}

void draw_user_pads() {
	//Draw faders
	//[TBD]
	
	//Draw toggles
	//[TBD]
}

void draw_color_map() {
	switch(track_modes[current_track]) {
		case MODE_SESSION:
			draw_session_pads();
			break;
		case MODE_NOTE:
			draw_note_pads();
			break;
		case MODE_DEVICE:
			draw_device_pads();
			break;
		case MODE_USER:
			draw_user_pads();
			break;
	}
	
	//Draw util buttons
	draw_util_buttons();
}


//-----------------------------------------------------------------------------------------
//                                EVENTS
//-----------------------------------------------------------------------------------------

/**
 * Called when the user presses or releases any button or pad on the control surface.
 *
 * @param type - TYPEPAD for normal pads or buttons, TYPESETUP for the Setup button
 * @param index - The index of the button, as detailed at the start of this file.
 * @param value - 0 for release, nonzero for press.
 */
void app_surface_event(u8 type, u8 index, u8 value) {
	//Only care about pads, not "setup" button
	if (type == TYPEPAD) {
		bool is_press = value > 0;
		
		u8 x_index = index % 10;
		u8 y_index = index / 10;
		
		u8 track = 0;
		
		if (pad_is_fader(index)) {
			//FADER
			if (is_press) {
				//Set the fader value
				cc_values[current_track][get_pad_fader_cc_number(index)] = get_pad_fader_value(index);
				//Send the midi message
				hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, get_pad_fader_cc_number(index), cc_values[current_track][get_pad_fader_cc_number(index)]);
			}
		} else if (pad_is_toggle(index)) {
			//TOGGLE
			if (is_press) {
				//Set the toggle value
				//Get the current value
				u8 cc_value = cc_values[current_track][get_pad_toggle_cc_number(index)];
				//Set the new value
				if (cc_value > 0) {
					cc_value = 0;
				} else {
					cc_value = 127;
				}
				cc_values[current_track][get_pad_toggle_cc_number(index)] = cc_value;
				
				//Send the midi message
				hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, get_pad_toggle_cc_number(index), cc_value);
			}
		} else if (pad_is_note(index)) {
			//NOTE
			if (is_press) {
				//Note press - Set the "NOTE ON" color, send the midi NOTEON message
				hal_send_midi(USBMIDI, NOTEON | MIDI_CHANNEL, get_pad_note(index), full_velocity ? 127 : value);
			} else {
				//Note release - Set the "NOTE OFF" color, send the midi NOTEOFF message
				hal_send_midi(USBMIDI, NOTEOFF | MIDI_CHANNEL, get_pad_note(index), 0);
			}
		} else {
			//Util buttons
			if (is_press) {
				switch(index) {
					//MODES
					case BTN_MODE_SESSION:
						track_modes[current_track] = MODE_SESSION;
						break;
					case BTN_MODE_NOTE:
						if (track_modes[current_track] == MODE_NOTE) {
							//Toggle the note mode
							if (note_mode == NOTE_MODE_DEFAULT) {
								note_mode = NOTE_MODE_KONG;
							} else {
								note_mode = NOTE_MODE_DEFAULT;
							}
						}
						track_modes[current_track] = MODE_NOTE;
						break;
					case BTN_MODE_DEVICE:
						if (track_modes[current_track] == MODE_DEVICE) {
							//Toggle the note mode
							if (note_mode == NOTE_MODE_DEFAULT) {
								note_mode = NOTE_MODE_KONG;
							} else {
								note_mode = NOTE_MODE_DEFAULT;
							}
						}
						track_modes[current_track] = MODE_DEVICE;
						break;
					/*
					//Disabled for now
					case BTN_MODE_USER:
						track_modes[current_track] = MODE_USER;
						break;
					*/
					
					//TRANSPORT
					case BTN_RECORD:
						//Toggle the record mode
						is_recording = !is_recording;
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_REC, is_recording ? 127 : 0);
						break;
					case BTN_PLAY:
						//Midi play
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_PLAY, 127);
						break;
					case BTN_STOP:
						//Midi stop
						is_recording = false;
						hal_send_midi(USBMIDI, CC | MIDI_CHANNEL, MIDI_TRANSPORT_STOP, 127);
						break;
					
					//FULL VELOCITY TOGGLE
					case BTN_FULL_VELOCITY:
						full_velocity = !full_velocity;
						break;
					
					//TRACK SELECTOR
					case BTN_TRACK_1:
					case BTN_TRACK_2:
					case BTN_TRACK_3:
					case BTN_TRACK_4:
					case BTN_TRACK_5:
					case BTN_TRACK_6:
					case BTN_TRACK_7:
					case BTN_TRACK_8:
						track = y_index - 1;
						if (track == current_track) {
							track += 8;
						}
						set_track(track);
						break;
					
					//OCTAVE SELECTOR
					case BTN_OCTAVE_1:
					case BTN_OCTAVE_2:
					case BTN_OCTAVE_3:
					case BTN_OCTAVE_4:
					case BTN_OCTAVE_5:
					case BTN_OCTAVE_6:
					case BTN_OCTAVE_7:
					case BTN_OCTAVE_8:
						set_octave(y_index - 1);
						break;
					
					default:
						break;
				}
			}
		}
		
		//Draw color_map
		draw_color_map();
	
		//Show note buttons when pressed
		if (is_press) {
			set_color(index, COLOR_PRESS);
		}
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
    if (pad_is_note(index)) {
		//This is a note pad
		hal_send_midi(USBMIDI, POLYAFTERTOUCH | MIDI_CHANNEL, index, value);
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
	
	//Set up starting track
	current_track = 0;
	
	//Set up default note mode
	note_mode = NOTE_MODE_DEFAULT;
	
	//Set up starting mode
	for (u8 i = 0; i < MAX_TRACKS; i++) {
		track_modes[i] = MODE_DEVICE;
	}
	
	//Set up default values for CC
	for (u8 i = 0; i < MAX_TRACKS; i++) {
		for (u8 j = 0; j < 128; j++) {
			cc_values[i][j] = 63;
		}
	}
	
	//Set up default values for toggles (off)
	for (u8 i = 0; i < MAX_TRACKS; i++) {
		for (u8 j = 1; j <= 8; j++)
		cc_values[i][79 + j] = 0;
	}
	
	//Draw the default color map
	draw_color_map();
}
