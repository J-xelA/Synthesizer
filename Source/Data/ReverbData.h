/*
  ==============================================================================

    ReverbData.h
    Created: 10 May 2022 1:06:15am
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ReverbData : public juce::dsp::Reverb {
public:
    void update(const float roomSize, const float damping, const float width, const float wetLevel, const float dryLevel);

private:
    juce::dsp::Reverb::Parameters reverbParams;
};