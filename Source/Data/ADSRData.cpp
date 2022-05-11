/*
  ==============================================================================

    ADSRData.cpp
    Created: 25 Feb 2022 1:53:09am
    Author:  Alex Johnson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRData.h"

void ADSRData::update(const float attack, const float decay, const float sustain, const float release) {
    ADSRParams.attack  = attack;
    ADSRParams.decay   = decay;
    ADSRParams.sustain = sustain;
    ADSRParams.release = release;

    setParameters(ADSRParams);
}//updateADSR
