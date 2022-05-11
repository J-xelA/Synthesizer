/*
  ==============================================================================

    ReverbComponent.h
    Created: 9 May 2022 11:26:19pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

using std::make_unique;
using std::unique_ptr;

using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

class ReverbComponent  : public juce::Component {
public:
    ReverbComponent(juce::AudioProcessorValueTreeState& valueTree, juce::String name,
        juce::String roomSizeID, juce::String dampingID,
        juce::String widthID, juce::String wetanddryID);
    ~ReverbComponent() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void setSliderParams(juce::Slider& slider, juce::Label& label,
        juce::AudioProcessorValueTreeState& valueTree,
        juce::String paramID, std::unique_ptr<Attachment>& attachment,
        const juce::Colour colour);

    juce::Slider roomsizeSlider;  unique_ptr<SliderAttachment> roomsizeSliderAttachment;
    juce::Slider dampingSlider;   unique_ptr<SliderAttachment> dampingSliderAttachment;
    juce::Slider widthSlider;     unique_ptr<SliderAttachment> widthSliderAttachment;
    juce::Slider wetanddrySlider; unique_ptr<SliderAttachment> wetanddrySliderAttachment;

    juce::String componentName{ "" };

    juce::Label roomsizeLabel{ "Room Size" , "Room size" };
    juce::Label dampingLabel{ "Damping"  , "Damping" };
    juce::Label widthLabel{ "Width", "Width" };
    juce::Label wetanddryLabel{ "Wet & Dry level", "Wet & Dry level" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReverbComponent)
};
