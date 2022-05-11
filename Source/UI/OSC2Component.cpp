/*
  ==============================================================================

    OSC2Component.cpp
    Created: 3 May 2022 11:11:44pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OSC2Component.h"

//==============================================================================
OSC2Component::OSC2Component(juce::AudioProcessorValueTreeState& valueTree,
    juce::String waveSelectorID, juce::String FMFreqID, juce::String FMDepthID) {

    setWantsKeyboardFocus(true);
    juce::LookAndFeel_V4::setDefaultLookAndFeel(&CLAF);

    // OSC Combobox
    juce::StringArray choices{ "Sin", "Square", "Triangle", "Saw" };
    OSC2WaveSelector.addItemList(choices, 1);
    addAndMakeVisible(OSC2WaveSelector);

    OSC2WaveSelectorAttachment = make_unique<ComboBoxAttachment>(valueTree, waveSelectorID, OSC2WaveSelector);
    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    waveSelectorLabel.setFont(15.0f);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(waveSelectorLabel);

    // FM Slider
    setSliderWithLabel(FMFreqSlider, FMFreqLabel, FMFreqID, FMFreqAttachment, valueTree, CustomColours::blue);
    setSliderWithLabel(FMDepthSlider, FMDepthLabel, FMDepthID, FMDepthAttachment, valueTree, CustomColours::blue);
}

OSC2Component::~OSC2Component() {}

void OSC2Component::paint (juce::Graphics& g) {
    auto bounds = getLocalBounds().reduced(5);
    auto labelPadding = bounds.removeFromTop(25.0f);

    auto darkBlue = juce::Colour::fromFloatRGBA(0.0f, 0.078f, 0.078f, 0.8f);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Oscillator 2", labelPadding.withX(5), juce::Justification::left);
    g.setColour(darkBlue);
    g.fillRect(bounds.toFloat());
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void OSC2Component::resized() {
    const auto startY = 55;
    const auto sliderWidth = 90;
    const auto sliderHeight = 90;

    const auto labelYOffset = 10;
    const auto labelHeight = 15;

    //                       (x, y,   w,  h)
    OSC2WaveSelector.setBounds(10, startY+25, 90, 30);
    waveSelectorLabel.setBounds(10, startY - labelYOffset - 5, 90, labelHeight);

    FMFreqSlider.setBounds(OSC2WaveSelector.getRight(), startY-5, sliderWidth, sliderHeight);
    FMFreqLabel.setBounds(FMFreqSlider.getX(), FMFreqSlider.getY() - labelYOffset-5,
        FMFreqSlider.getWidth(), labelHeight);

    FMDepthSlider.setBounds(FMFreqSlider.getRight(), startY-5, sliderWidth, sliderHeight);
    FMDepthLabel.setBounds(FMDepthSlider.getX(), FMDepthSlider.getY() - labelYOffset-5,
        FMDepthSlider.getWidth(), labelHeight);
}

void OSC2Component::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::String paramID,
    unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState& valueTree,
    const juce::Colour colour) {

    slider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    slider.setColour(juce::Slider::rotarySliderFillColourId, colour);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(slider);

    attachment = make_unique<SliderAttachment>(valueTree, paramID, slider);

    label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    label.setFont(15.0f);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}//setSliderWithLabel()