/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 27 Mar 2022 9:56:35pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include "CustomLookAndFeel.h"

juce::String CustomLookAndFeel::CustomLabel::initialValue = "";

CustomLookAndFeel::CustomLookAndFeel() {};
CustomLookAndFeel::~CustomLookAndFeel() {};

juce::Slider::SliderLayout CustomLookAndFeel::getSliderLayout(juce::Slider& slider) {
    auto bounds = slider.getLocalBounds();
    juce::Slider::SliderLayout layout;

    layout.textBoxBounds = bounds.withY(-1);
    layout.sliderBounds = bounds;

    return layout;
}//getSliderLayout()

void CustomLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width,
                                         int height, float sliderPos, const float startAngle,
                                         const float endAngle, juce::Slider& slider) {
    auto fill = slider.findColour(juce::Slider::rotarySliderFillColourId);

    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(2.0f);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight())/2.0f;
    auto angle = startAngle + sliderPos * (endAngle - startAngle);
    auto lineWidth = radius * 0.085f;
    auto pathRadius = radius - lineWidth * 1.6f;

    juce::Path backgroundPath;
    backgroundPath.addCentredArc(
        bounds.getCentreX(),
        bounds.getCentreY(),
        pathRadius-5,
        pathRadius-5,
        0.0f,
        startAngle,
        endAngle,
        true
    );
    g.setColour(CustomColours::blackGrey);
    g.strokePath(backgroundPath, juce::PathStrokeType(lineWidth+2.0f,
                 juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path colouredPath;
    colouredPath.addCentredArc(
        bounds.getCentreX(),
        bounds.getCentreY(),
        pathRadius-5,
        pathRadius-5,
        0.0f,
        startAngle,
        angle,
        true
    );
    auto alphaValue = 0.1f + (float)slider.getValue() * 0.9f;
    auto brightnessValue = 0.4f + (float)slider.getValue() * 0.6f;
    g.setColour(fill.withAlpha(alphaValue).brighter(brightnessValue));
    g.strokePath(colouredPath, juce::PathStrokeType(lineWidth+2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    //g.setColour(fill);
    //g.strokePath(colouredPath, juce::PathStrokeType(lineWidth+2.0f,
    //    juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path thumb;
    auto thumbWidth = lineWidth * 3.0f;
    thumb.addRectangle(-thumbWidth/2, -thumbWidth/2, thumbWidth, radius-6.5f);
    g.setColour(CustomColours::creamWhite);
    g.fillPath(thumb, juce::AffineTransform::rotation(angle+3.14f).translated(bounds.getCentre()));

    g.fillEllipse(bounds.reduced(radius*0.43f));
}//drawRotarySlider()

CustomLookAndFeel::CustomLabel* CustomLookAndFeel::createSliderTextBox(juce::Slider& slider) {
    auto* label = new CustomLabel();

    label->setJustificationType(juce::Justification::centredBottom);
    label->setColour(juce::Label::textColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    label->setColour(juce::Label::textWhenEditingColourId, slider.findColour(juce::Slider::textBoxTextColourId));
    label->setColour(juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
    label->setInterceptsMouseClicks(false, false);
    label->setFont(16.0f);

    return label;
}//createSliderTextBox()

juce::CaretComponent* CustomLookAndFeel::createCaretComponent(juce::Component* keyFocusOwner) {
    auto caret = new juce::CaretComponent(keyFocusOwner);
    caret->setColour(juce::CaretComponent::caretColourId, juce::Colours::red);
    return caret;
}//createCaretComponent()