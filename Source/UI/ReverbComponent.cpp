/*
  ==============================================================================

    Reverb.cpp
    Created: 9 May 2022 10:03:02pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ReverbComponent.h"

//==============================================================================
ReverbComponent::ReverbComponent(juce::AudioProcessorValueTreeState& valueTree, juce::String name,
    juce::String roomsizeID, juce::String dampingID, juce::String widthID, juce::String wetanddryID) {
    componentName = name;

    setSliderParams(roomsizeSlider, roomsizeLabel, valueTree, roomsizeID, roomsizeSliderAttachment, CustomColours::blue);
    setSliderParams(dampingSlider, dampingLabel, valueTree, dampingID, dampingSliderAttachment, CustomColours::blue);
    setSliderParams(widthSlider, widthLabel, valueTree, widthID, widthSliderAttachment, CustomColours::blue);
    setSliderParams(wetanddrySlider, wetanddryLabel, valueTree, wetanddryID, wetanddrySliderAttachment, CustomColours::blue);
}

ReverbComponent::~ReverbComponent() {}

void ReverbComponent::paint(juce::Graphics& g) {
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

void ReverbComponent::resized() {
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

    roomsizeSlider.setBounds(sliderStartX+21, sliderStartY-10, sliderWidth, sliderHeight);
    roomsizeLabel.setBounds(roomsizeSlider.getX(), labelStart-(2*padding), sliderWidth, labelHeight);

    dampingSlider.setBounds(sliderStartX+21, roomsizeSlider.getBottom() + labelHeight+labelYOffset-10, sliderWidth, sliderHeight);
    dampingLabel.setBounds(dampingSlider.getX(), dampingSlider.getY() - labelHeight+padding, sliderWidth, labelHeight);


    widthSlider.setBounds(sliderStartX+21, dampingSlider.getBottom() + labelHeight + labelYOffset - 10, sliderWidth, sliderHeight);
    widthLabel.setBounds(widthSlider.getX(), widthSlider.getY() - labelHeight + padding, sliderWidth, labelHeight);

    wetanddrySlider.setBounds(sliderStartX+21, widthSlider.getBottom() + labelHeight + labelYOffset - 10, sliderWidth, sliderHeight);
    wetanddryLabel.setBounds(wetanddrySlider.getX(), wetanddrySlider.getY() - labelHeight + padding, sliderWidth, labelHeight);
}

void ReverbComponent::setSliderParams(juce::Slider& slider, juce::Label& label,
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