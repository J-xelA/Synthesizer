/*
  ==============================================================================

    NameLabel.h
    Created: 9 May 2022 9:26:52pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CustomColours.h"

class NameLabel : public juce::Label {
public:
	NameLabel() {
		setFont(20.0f);
		setColour(juce::Label::textColourId, CustomColours::grey);
		setJustificationType(juce::Justification::centred);
	}//NameLabel()

	~NameLabel() {}
};