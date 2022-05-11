/*
  ==============================================================================

    Reverb.cpp
    Created: 9 May 2022 10:03:02pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Reverb.h"

//==============================================================================
Reverb::Reverb(juce::AudioProcessorValueTreeState &valueTree, juce::String name,
    juce::String roomsizeID, juce::String dampingID, juce::String widthID, juce::String wetanddryID) {
    componentName = name;

    setSliderParams(roomsizeSlider, roomsizeLabel, valueTree, roomsizeID, roomsizeSliderAttachment, CustomColours::blue);
    setSliderParams(dampingSlider, dampingLabel, valueTree, dampingID, dampingSliderAttachment, CustomColours::blue);
    setSliderParams(widthSlider, widthLabel, valueTree, widthID, widthSliderAttachment, CustomColours::blue);
    setSliderParams(wetanddrySlider, wetanddryLabel, valueTree, wetanddryID, wetanddrySliderAttachment, CustomColours::blue);
}

Reverb::~Reverb() {}

void Reverb::paint (juce::Graphics& g) {
    auto bounds = getLocalBounds().reduced(5);
    auto labelPadding = bounds.removeFromTop(25.0f);

    auto darkBlue = juce::Colour::fromFloatRGBA(0.0f, 0.078f, 0.078f, 0.8f);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText(componentName, labelPadding.withX(5), juce::Justification::left);
    g.setColour(darkBlue);
    g.fillRect(bounds.toFloat());
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void Reverb::resized() {
    const auto startY = 55;
    const auto sliderWidth = 90;
    const auto sliderHeight = 90;

    const auto labelYOffset = 15;
    const auto labelHeight = 15;

    const auto bounds = getLocalBounds().reduced(10);
    const auto padding = 3;
    const auto sliderStartX = padding;
    const auto sliderStartY = 55;
    const auto labelStart = sliderStartY - labelYOffset;


    //roomsizeSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    //roomsizeLabel.setBounds(roomsizeSlider.getX(), labelStart, sliderWidth, labelHeight);

    //dampingSlider.setBounds(roomsizeSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    //dampingLabel.setBounds(dampingSlider.getX(), labelStart, sliderWidth, labelHeight);


    //widthSlider.setBounds(dampingSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    //widthLabel.setBounds(widthSlider.getX(), labelStart, sliderWidth, labelHeight);

    //wetanddrySlider.setBounds(widthSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);
    //wetanddryLabel.setBounds(wetanddrySlider.getX(), labelStart, sliderWidth, labelHeight);

    roomsizeSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    roomsizeLabel.setBounds(roomsizeSlider.getX(), labelStart, sliderWidth, labelHeight);

    dampingSlider.setBounds(sliderStartX, roomsizeSlider.getBottom() + padding, sliderWidth, sliderHeight);
    dampingLabel.setBounds(dampingSlider.getX(), labelStart, sliderWidth, labelHeight);


    widthSlider.setBounds(sliderStartX, dampingSlider.getBottom() + padding, sliderWidth, sliderHeight);
    widthLabel.setBounds(widthSlider.getX(), labelStart, sliderWidth, labelHeight);

    wetanddrySlider.setBounds(sliderStartX, widthSlider.getBottom() + padding, sliderWidth, sliderHeight);
    wetanddryLabel.setBounds(wetanddrySlider.getX(), labelStart, sliderWidth, labelHeight);
}

void Reverb::setSliderParams(juce::Slider& slider, juce::Label& label,
    juce::AudioProcessorValueTreeState& valueTree,
    juce::String paramID, std::unique_ptr<SliderAttachment>& attachment,
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
}