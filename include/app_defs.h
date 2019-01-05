
#ifndef APP_TYPES_H
#define APP_TYPES_H

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

// ____________________________________________________________________________
//
// Don't modify this file!  This declares the binary interface to the library,
// so modifying it will probably break things.
// ____________________________________________________________________________
//
// Types
// ____________________________________________________________________________

typedef signed long  s32;
typedef signed short s16;
typedef signed char  s8;

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

// ____________________________________________________________________________
//
// App structure
// ____________________________________________________________________________

#define	TYPEPAD				0
#define	TYPESETUP			1

#define	MAXLED				63

// ____________________________________________________________________________
//
// Useful MIDI constants
// ____________________________________________________________________________

#define	NOTEON				0x90
#define	NOTEOFF				0x80
#define	POLYAFTERTOUCH		0xA0
#define	CC					0xB0
#define	CHANNELAFTERTOUCH   0xD0
#define	SONGPOSITIONPOINTER 0xF2
#define	MIDITIMINGCLOCK     0xF8
#define	MIDISTART           0xFA
#define	MIDICONTINUE        0xFB
#define	MIDISTOP            0xFC

// ____________________________________________________________________________
//
// MIDI ports
// ____________________________________________________________________________

// USB MIDI: "Standalone" port
#define	USBSTANDALONE		0

// USB MIDI: "MIDI" port
#define	USBMIDI				1

// MIDI DIN port
#define	DINMIDI				2

// ____________________________________________________________________________
//
// MIDI Jack Socket Switch IDs
// ____________________________________________________________________________

#define	MIDI_IN_CABLE		0
#define	MIDI_OUT_CABLE		1

// ____________________________________________________________________________
//
// Flash storage
// ____________________________________________________________________________

#define USER_AREA_SIZE 1024

// ____________________________________________________________________________
//
// Raw ADC reads. For technical reasons, the ADC layout is not the same as the
// LED layout. Be sure to translate ADC values using the table below as you
// need them!
// ____________________________________________________________________________

#define PAD_COUNT 64

static const u8 ADC_MAP[PAD_COUNT] =
{
	11, 51, 12, 52, 13, 53, 14, 54,
	15, 55, 16, 56, 17 ,57, 18, 58,
	21, 61, 22, 62, 23, 63, 24, 64,
	25, 65, 26, 66, 27, 67, 28, 68,
	31, 71, 32, 72, 33, 73, 34, 74,
	35, 75, 36, 76, 37, 77, 38, 78,
	41, 81, 42, 82, 43, 83, 44, 84,
	45, 85, 46, 86, 47, 87, 48, 88,
};

// ____________________________________________________________________________

#define BUTTON_COUNT 100


// Button shortcuts
#define TOP1 		91
#define TOP2 		92
#define TOP3 		93
#define TOP4 		94
#define TOP5 		95
#define TOP6 		96
#define TOP7 		97
#define TOP8 		98

#define LEFT1 		10
#define LEFT2 		20
#define LEFT3 		30
#define LEFT4 		40
#define LEFT5 		50
#define LEFT6 		60
#define LEFT7 		70
#define LEFT8 		80

#define RIGHT1		19
#define RIGHT2		29
#define RIGHT3		39
#define RIGHT4		49
#define RIGHT5		59
#define RIGHT6		69
#define RIGHT7		79
#define RIGHT8		89

#define BOTTOM1		 1
#define BOTTOM2		 2
#define BOTTOM3		 3
#define BOTTOM4		 4
#define BOTTOM5		 5
#define BOTTOM6		 6
#define BOTTOM7		 7
#define BOTTOM8		 8

#define PAD11		11
#define PAD12		12
#define PAD13		13
#define PAD14		14
#define PAD15		15
#define PAD16		16
#define PAD17		17
#define PAD18		18

#define PAD21		21
#define PAD22		22
#define PAD23		23
#define PAD24		24
#define PAD25		25
#define PAD26		26
#define PAD27		27
#define PAD28		28

#define PAD31		31
#define PAD32		32
#define PAD33		33
#define PAD34		34
#define PAD35		35
#define PAD36		36
#define PAD37		37
#define PAD38		38

#define PAD41		41
#define PAD42		42
#define PAD43		43
#define PAD44		44
#define PAD45		45
#define PAD46		46
#define PAD47		47
#define PAD48		48

#define PAD51		51
#define PAD52		52
#define PAD53		53
#define PAD54		54
#define PAD55		55
#define PAD56		56
#define PAD57		57
#define PAD58		58

#define PAD61		61
#define PAD62		62
#define PAD63		63
#define PAD64		64
#define PAD65		65
#define PAD66		66
#define PAD67		67
#define PAD68		68

#define PAD71		71
#define PAD72		72
#define PAD73		73
#define PAD74		74
#define PAD75		75
#define PAD76		76
#define PAD77		77
#define PAD78		78

#define PAD81		81
#define PAD82		82
#define PAD83		83
#define PAD84		84
#define PAD85		85
#define PAD86		86
#define PAD87		87
#define PAD88		88

#endif
