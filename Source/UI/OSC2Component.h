/*
  ==============================================================================

    OSC2Component.h
    Created: 3 May 2022 11:11:44pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

using std::unique_ptr;
using std::make_unique;

using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

//==============================================================================
/*
*/
class OSC2Component  : public juce::Component
{
public:
    OSC2Component(juce::AudioProcessorValueTreeState &valueTree,
        juce::String waveSelectorID, juce::String FMFreqID, juce::String FMDepthID);
    ~OSC2Component() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox OSC2WaveSelector;
    unique_ptr<ComboBoxAttachment> OSC2WaveSelectorAttachment;

    juce::Slider FMFreqSlider;
    juce::Slider FMDepthSlider;

    juce::Label waveSelectorLabel{ "Wave Type", "Wave Type" };

    juce::Label FMFreqLabel{ "FM Freq", "FM Frequency" };
    unique_ptr<SliderAttachment> FMFreqAttachment;
    juce::Label FMDepthLabel{ "FM Depth", "FM Depth" };
    unique_ptr<SliderAttachment> FMDepthAttachment;

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::String paramID,
        unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& valueTree,
        const juce::Colour colour);

    CustomLookAndFeel CLAF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSC2Component)
};
