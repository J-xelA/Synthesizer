/*
  ==============================================================================

    ADSRData.h
    Created: 25 Feb 2022 1:53:09am
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ADSRData : public juce::ADSR {
public:
    void update(const float attack, const float decay, const float sustain, const float release);

private:
    juce::ADSR::Parameters ADSRParams;
};
