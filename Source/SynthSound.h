/*
  ==============================================================================

    SynthSound.h
    Created: 2 Feb 2022 10:53:13pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using std::sin;
using std::vector;
using std::unique_ptr;
using std::make_unique;

class SynthSound : public juce::SynthesiserSound {
public:
    bool appliesToNote(int midiNoteNumber) override { return true; }
    bool appliesToChannel(int midiChannel) override { return true; }
private:

};
