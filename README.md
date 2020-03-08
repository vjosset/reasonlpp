[![Build Status](https://travis-ci.org/dvhdr/launchpad-pro.svg?branch=master)](https://travis-ci.org/dvhdr/launchpad-pro)

# Launchpad Pro

Open-source firmware for the Novation Launchpad Pro grid controller! This project is based on [dvhdr's launchpad-pro project](https://github.com/dvhdr/launchpad-pro).

This firmware was developed for use with Reason Studio's Reason DAW.

# Installing the Firmware

If you can't (or don't want to) build the project from source, you can simply send the `launchpad_pro.syx` file (in the `build` directory) to your Launchpad Pro.
I use [MIDI-OX](http://www.midiox.com/) to send the firmware through, but there are other tools that can do that too.

1. Turn off your Launchpad Pro
2. While holding the "Setup" button, turn on your Launchpad Pro
3. Launch MIDI-OX 
4. Select your Launchpad Pro as an Output device (Options - Midi Devices - Midi Outputs)
5. Actions - Send - SysEx file
6. Select the sysex file (launchpad_pro.syx)
7. Wait for the send to complete
8. Done!

# Installing the Reason Remote Map

For use with Reason, you will also need to install the Remote mapping files:
- `reasonlpp.midicodec`
- `reasonlpp.remotemap`

On Windows, these are the paths where to install those files:
- C:\ProgramData\Propellerhead Software\Remote\Codecs\MIDI Codecs\reasonlpp.midicodec
- C:\ProgramData\Propellerhead Software\Remote\Maps\Other\reasonlpp.remotemap

# Usage

For a quick visual example and explanation of this firmware's feature, see the demo video here:

[TBD]

## Global Utilities

![Global Utils](ReasonLPPGlobalControls.png "Global Utils")

### Modes

Cycle through the modes by pressing the `Mode Switch` button (labeled `User` on the LPP).
Toggle keyboard/kong note mode by pressing the `Note Switch` button (labeled `Note` on the LPP).

### MIDI Transport Controls

MIDI transport buttons are defined on the very bottom row.

- Toggle Loop: `Mute` button on the LPP
- Rewind: `Solo` button on the LPP
- Fast Forward: `Volume` button on the LPP
- Stop: `Pan` button on the LPP
- Play: `Sends` button on the LPP
- Record: `Stop Clip` button on the LPP

### Velocity Toggle

The bottom-left button (labeled `Record Arm` on the LPP) toggles between full-velocity and velocity-sensitive modeds for MIDI notes sent from the LPP.

### Transpose

To transpose the notes sent by the LPP, use the `Up` and `Down` buttons (top-left of the LPP).
- `Up`: Transpose one full octave up
- `Down`: Transpose one full octave down
- `Shift Up`: Transpose one semi-tone up
- `Shift Down`: Transpose one semi-tone down

### Track Selection

To switch target tracks in Reason, use the `Left` and `Right` buttons (top row of the LPP).

## Modes

### Notes

This is the default mode for the LPP.
In this mode, each square pad is a note.

In "Keyboard" note mode, each color represents one full octave.
In "Kong" note mode, each color is a set of 16 MIDI notes, allowing you to set up one combinator with four Kongs, each responding to a set of 16 square buttons.

### Combinator

In this mode, the left half of the square buttons represents a keyboard (or two Kongs). The right half is used to control the Combinator's rotary and toggle controls.

### X/Y

In this mode, the full grid represents a two-dimensional control for two CC values.

### Mixer

#### Mixer 14:2

When targeting a Mixer 14:2, the blue buttons represent the first eight channel volumes on the mixer and the green buttons represent the "Solo" toggle for the first eight channels on the mixer.

#### Line Mixer 6:2

When targetting a Line Mixer 6:2, columns 1-6 of blue buttons represent the first fix channel volumes, column 7 controls the main Aux return level, and column 8 controls the master volume on the mixer.
The first six green buttons control each channel's "Solo". Green buttons 6 and 7 do nothing.