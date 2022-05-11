/*
  ==============================================================================

    FilterComponent.cpp
    Created: 23 Mar 2022 11:50:29pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterComponent.h"

//==============================================================================
FilterComponent::FilterComponent(juce::AudioProcessorValueTreeState& valueTree,
    juce::String filterTypeSelectorID, juce::String filterCutoffID, juce::String filterResonanceID) {
    // Filter ComboBox
    juce::StringArray choices{ "Low-Pass", "Band-Pass", "High-Pass" };
    FilterTypeSelector.addItemList(choices, 1);
    addAndMakeVisible(FilterTypeSelector);

    FilterTypeSelectorAttachment = make_unique<ComboBoxAttachment>(valueTree, filterTypeSelectorID, FilterTypeSelector);
    FilterSelectorLabel.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
    FilterSelectorLabel.setFont(15.0f);
    FilterSelectorLabel.setJustificationType(juce::Justification::left);
    addAndMakeVisible(FilterSelectorLabel);

    // Filter Slider
    setSliderWithLabel(FilterCutoffSlider, FilterCutoffLabel, filterCutoffID, FilterCutoffAttachment, valueTree, CustomColours::blue);
    setSliderWithLabel(FilterResonanceSlider, FilterResonanceLabel, filterResonanceID, FilterResonanceAttachment, valueTree, CustomColours::blue);
}//FilterComponent()

FilterComponent::~FilterComponent() {}

void FilterComponent::paint(juce::Graphics& g) {
    auto bounds = getLocalBounds().reduced(5);
    auto labelPadding = bounds.removeFromTop(25.0f);

    auto darkBlue = juce::Colour::fromFloatRGBA(0.0f, 0.078f, 0.078f, 0.8f);

    g.setColour(juce::Colours::white);
    g.setFont(20.0f);
    g.drawText("Filter", labelPadding.withX(5), juce::Justification::left);
    g.setColour(darkBlue);
    g.fillRect(bounds.toFloat());
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(bounds.toFloat(), 5.0f, 2.0f);
}

void FilterComponent::resized() {
    const auto startY = 55;
    const auto sliderWidth = 90;
    const auto sliderHeight = 90;
    const auto padding = 3;
    const auto labelYOffset = 10;
    const auto labelHeight = 15;

    //                           (x,          y,  w,  h)
    FilterTypeSelector.setBounds(10, startY + 25, 90, 30);
    FilterSelectorLabel.setBounds(10, startY - labelYOffset-5, 90, labelHeight);

    FilterCutoffSlider.setBounds(FilterTypeSelector.getRight(), startY-5, sliderWidth, sliderHeight);
    FilterCutoffLabel.setBounds(FilterCutoffSlider.getX(), FilterCutoffSlider.getY() - labelYOffset-5,
        sliderWidth, labelHeight);

    FilterResonanceSlider.setBounds(FilterTypeSelector.getRight(), FilterCutoffSlider.getBottom() + labelHeight+labelYOffset-10, sliderWidth, sliderHeight);
    FilterResonanceLabel.setBounds(FilterResonanceSlider.getX(), FilterResonanceSlider.getY() - labelHeight+padding,
        sliderWidth, labelHeight);
}

void FilterComponent::setSliderWithLabel(juce::Slider& slider, juce::Label& label, juce::String paramID,
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
