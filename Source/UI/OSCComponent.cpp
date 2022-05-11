/*
  ==============================================================================

    OSCComponent.cpp
    Created: 26 Feb 2022 12:08:55am
    Author:  Alex Johnson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OSCComponent.h"

//==============================================================================
OSCComponent::OSCComponent(juce::AudioProcessorValueTreeState &valueTree,
    juce::String waveSelectorID, juce::String FMFreqID, juce::String FMDepthID) {

    setWantsKeyboardFocus(true);
    juce::LookAndFeel_V4::setDefaultLookAndFeel(&CLAF);

    // OSC Combobox
    juce::StringArray choices{ "Sin", "Square", "Triangle", "Saw" };
    OSCWaveSelector.addItemList(choices, 1);
    addAndMakeVisible(OSCWaveSelector);

    OSCWaveSelectorAttachment = make_unique<ComboBoxAttachment>(valueTree, waveSelectorID, OSCWaveSelector);
    waveSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    waveSelectorLabel.setFont(15.0f);
    waveSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(waveSelectorLabel);

    // FM Slider
    setSliderWithLabel(FMFreqSlider, FMFreqLabel, FMFreqID, FMFreqAttachment, valueTree, CustomColours::blue);
    setSliderWithLabel(FMDepthSlider, FMDepthLabel, FMDepthID, FMDepthAttachment, valueTree, CustomColours::blue);
}

OSCComponent::~OSCComponent() {}

void OSCComponent::paint (juce::Graphics& g) {
    auto bounds = getLocalBounds().reduced(5);
    auto labelPadding = bounds.removeFromTop(25.0f);

    auto darkBlue = juce::Colour::fromFloatRGBA(0.0f, 0.078f, 0.078f, 0.8f);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Oscillator 1", labelPadding.withX(5), juce::Justification::left);
    g.setColour(darkBlue);
    g.fillRect(bounds.toFloat());
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void OSCComponent::resized() {
    const auto startY = 55;
    const auto sliderWidth = 90;
    const auto sliderHeight = 90;

    const auto labelYOffset = 10;
    const auto labelHeight = 15;

    //                       (x, y,   w,  h)
    OSCWaveSelector.setBounds(10, startY+25, 90, 30);
    waveSelectorLabel.setBounds(10, startY - labelYOffset - 5, 90, labelHeight);

    FMFreqSlider.setBounds(OSCWaveSelector.getRight(), startY-5, sliderWidth, sliderHeight);
    FMFreqLabel.setBounds(FMFreqSlider.getX(), FMFreqSlider.getY() - labelYOffset - 5,
                          FMFreqSlider.getWidth(), labelHeight);

    FMDepthSlider.setBounds(FMFreqSlider.getRight(), startY-5, sliderWidth, sliderHeight);
    FMDepthLabel.setBounds(FMDepthSlider.getX(), FMDepthSlider.getY() - labelYOffset - 5,
                           FMDepthSlider.getWidth(), labelHeight);
}

void OSCComponent::setSliderWithLabel(juce::Slider &slider, juce::Label& label, juce::String paramID,
    unique_ptr<SliderAttachment>& attachment, juce::AudioProcessorValueTreeState &valueTree, 
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