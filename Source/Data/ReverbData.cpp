/*
  ==============================================================================

    ReverbData.cpp
    Created: 10 May 2022 1:06:15am
    Author:  Alex Johnson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbData.h"

void ReverbData::update(const float roomSize, const float damping, const float width, const float wetLevel, const float dryLevel) {
    reverbParams.roomSize = roomSize;
    reverbParams.damping = damping;
    reverbParams.width = width;
    reverbParams.wetLevel = wetLevel;
    reverbParams.dryLevel = dryLevel;

    setParameters(reverbParams);
}//updateADSR