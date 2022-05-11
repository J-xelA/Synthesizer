/*
  ==============================================================================

    ADSRComponent.cpp
    Created: 25 Feb 2022 1:52:39am
    Author:  Alex Johnson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ADSRComponent.h"

//==============================================================================
ADSRComponent::ADSRComponent(juce::AudioProcessorValueTreeState& valueTree, juce::String name,
    juce::String attackID, juce::String decayID, juce::String sustainID, juce::String releaseID) {
    componentName = name;

    setSliderParams(attackSlider,  attackLabel,  valueTree, attackID,  attackSliderAttachment,  CustomColours::blue);
    setSliderParams(decaySlider,   decayLabel,   valueTree, decayID,   decaySliderAttachment,   CustomColours::blue);
    setSliderParams(sustainSlider, sustainLabel, valueTree, sustainID, sustainSliderAttachment, CustomColours::blue);
    setSliderParams(releaseSlider, releaseLabel, valueTree, releaseID, releaseSliderAttachment, CustomColours::blue);
}

ADSRComponent::~ADSRComponent() {}

void ADSRComponent::paint (juce::Graphics& g) {
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

void ADSRComponent::resized() {
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


    attackSlider.setBounds(sliderStartX + 21, sliderStartY-10, sliderWidth, sliderHeight);
    attackLabel .setBounds(attackSlider.getX(), labelStart-(2*padding), sliderWidth, labelHeight);
    
    decaySlider.setBounds(sliderStartX + 21, attackSlider.getBottom() + labelHeight+labelYOffset-10, sliderWidth, sliderHeight);
    decayLabel.setBounds(decaySlider.getX(), decaySlider.getY() - labelHeight+padding, sliderWidth, labelHeight);


    sustainSlider.setBounds(sliderStartX + 21, decaySlider.getBottom() + labelHeight+labelYOffset-10, sliderWidth, sliderHeight);
    sustainLabel.setBounds(sustainSlider.getX(), sustainSlider.getY() - labelHeight + padding, sliderWidth, labelHeight);

    releaseSlider.setBounds(sliderStartX + 21, sustainSlider.getBottom() + labelHeight+labelYOffset-10, sliderWidth, sliderHeight);
    releaseLabel.setBounds(releaseSlider.getX(), releaseSlider.getY() - labelHeight + padding, sliderWidth, labelHeight);
}

void ADSRComponent::setSliderParams(juce::Slider& slider, juce::Label& label,
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
