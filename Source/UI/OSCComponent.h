/*
  ==============================================================================

    OSCComponent.h
    Created: 26 Feb 2022 12:08:55am
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
class OSCComponent  : public juce::Component
{
public:
    OSCComponent(juce::AudioProcessorValueTreeState &valueTree,
        juce::String waveSelectorID, juce::String FMFreqID, juce::String FMDepthID);
    ~OSCComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox OSCWaveSelector;
    unique_ptr<ComboBoxAttachment> OSCWaveSelectorAttachment;

    juce::Slider FMFreqSlider;
    juce::Slider FMDepthSlider;

    juce::Label waveSelectorLabel{ "Wave Type", "Wave Type" };

    juce::Label FMFreqLabel{ "FM Freq", "FM Frequency" };
    unique_ptr<SliderAttachment> FMFreqAttachment;
    juce::Label FMDepthLabel{ "FM Depth", "FM Depth" };
    unique_ptr<SliderAttachment> FMDepthAttachment;

    void setSliderWithLabel(juce::Slider &slider, juce::Label &label, juce::String paramID,
        unique_ptr<SliderAttachment> &attachment, juce::AudioProcessorValueTreeState &valueTree,
        const juce::Colour colour);

    CustomLookAndFeel CLAF;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSCComponent)
};
