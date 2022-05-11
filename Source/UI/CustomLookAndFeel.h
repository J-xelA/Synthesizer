/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 27 Mar 2022 9:56:35pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomColours.h"

class CustomLookAndFeel : public juce::LookAndFeel_V4 {
public:
    struct CustomLabel : public juce::Label {
        static juce::String initialValue;
    };

    CustomLookAndFeel();
    ~CustomLookAndFeel();

    juce::Slider::SliderLayout getSliderLayout(juce::Slider& slider) override;
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                          float sliderPos, float startAngle, 
                          float endAngle, juce::Slider&) override;

    CustomLabel* createSliderTextBox(juce::Slider & slider);
    juce::CaretComponent* createCaretComponent(juce::Component* keyFocusOwner) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomLookAndFeel);
};