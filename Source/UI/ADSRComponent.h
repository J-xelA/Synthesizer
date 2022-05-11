/*
  ==============================================================================

    ADSRComponent.h
    Created: 25 Feb 2022 1:52:39am
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

using std::make_unique;
using std::unique_ptr;

using Attachment       = juce::AudioProcessorValueTreeState::SliderAttachment;
using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

//==============================================================================
/*
*/
class ADSRComponent : public juce::Component {
public:
    ADSRComponent(juce::AudioProcessorValueTreeState &valueTree, juce::String name,
                  juce::String attackID, juce::String decayID,
                  juce::String sustainID, juce::String releaseID);
    ~ADSRComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void setSliderParams(juce::Slider& slider, juce::Label& label,
                         juce::AudioProcessorValueTreeState& valueTree,
                         juce::String paramID, std::unique_ptr<Attachment>& attachment,
                         const juce::Colour colour);

    juce::Slider attackSlider;  unique_ptr<SliderAttachment> attackSliderAttachment;
    juce::Slider decaySlider;   unique_ptr<SliderAttachment> decaySliderAttachment;
    juce::Slider sustainSlider; unique_ptr<SliderAttachment> sustainSliderAttachment;
    juce::Slider releaseSlider; unique_ptr<SliderAttachment> releaseSliderAttachment;

    juce::String componentName{ "" };

    juce::Label attackLabel { "Attack" , "Attack" };
    juce::Label decayLabel  { "Decay"  , "Decay" };
    juce::Label sustainLabel{ "Sustain", "Sustain" };
    juce::Label releaseLabel{ "Release", "Release" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ADSRComponent)
};
