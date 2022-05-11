/*
  ==============================================================================

    Dial.h
    Created: 27 Mar 2022 9:57:07pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "CustomLookAndFeel.h"

class Dial : public juce::Slider {
public:
    Dial();
    ~Dial();

    void paint(juce::Graphics& g) override;

    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

    bool keyPressed(const juce::KeyPress& k) override;

private:
    void drawFocusMark(juce::Graphics& g, juce::Colour colour);

    CustomLookAndFeel CLAF;
};