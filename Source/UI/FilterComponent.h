/*
  ==============================================================================

    FilterComponent.h
    Created: 23 Mar 2022 11:50:29pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

using std::unique_ptr;
using std::make_unique;

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

//==============================================================================
/*
*/
class FilterComponent : public juce::Component
{
public:
    FilterComponent(juce::AudioProcessorValueTreeState& valueTree,
        juce::String filterTypeSelectorID, juce::String filterCutoffID, juce::String filterResonanceID);
    ~FilterComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::ComboBox FilterTypeSelector;
    unique_ptr<ComboBoxAttachment> FilterTypeSelectorAttachment;
    
    juce::Slider FilterCutoffSlider;
    juce::Label FilterSelectorLabel{ "Filter Type", "Filter Type" };
    juce::Label FilterCutoffLabel{ "Filter Freq", "Cutoff" };
    unique_ptr<SliderAttachment> FilterCutoffAttachment;

    juce::Slider FilterResonanceSlider;
    juce::Label FilterResonanceLabel{ "Filter Resonance", "Resonance" };
    unique_ptr<SliderAttachment> FilterResonanceAttachment;

    void setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::String paramID,
        unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& valueTree,
        const juce::Colour colour);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterComponent)
};
