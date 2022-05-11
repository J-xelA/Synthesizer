/*
  ==============================================================================

    Dial.cpp
    Created: 27 Mar 2022 9:57:07pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include "Dial.h"

Dial::Dial() {
    setSliderStyle(juce::Slider::SliderStyle::Rotary);
    setRotaryParameters(juce::MathConstants<float>::pi * 1.25f,
                        juce::MathConstants<float>::pi * 2.75f, true);
    setColour(juce::Slider::textBoxTextColourId, CustomColours::blackGrey);
    setColour(juce::Slider::textBoxOutlineColourId, CustomColours::grey);
    setLookAndFeel(&CLAF);
    setVelocityBasedMode(true);
    setRange(0.0, 100.0, 0.01);
    setValue(0.0);
    setDoubleClickReturnValue(true, 0.0);
    setWantsKeyboardFocus(true);
    setTextValueSuffix(" %");
    onValueChange = [&]() {
        if (getValue() < 10)
            setNumDecimalPlacesToDisplay(2);
        else if (10 <= getValue() && getValue() < 100)
            setNumDecimalPlacesToDisplay(1);
        else
            setNumDecimalPlacesToDisplay(0);
    };
}//Dial()

Dial::~Dial() { setLookAndFeel(nullptr); }

void Dial::paint(juce::Graphics& g) {
    juce::Slider::paint(g);

    if (hasKeyboardFocus(true))
        drawFocusMark(g, findColour(juce::Slider::textBoxOutlineColourId));
}//paint()

void Dial::drawFocusMark(juce::Graphics& g, juce::Colour colour) {
    g.setColour(colour);

    auto bounds = getLocalBounds().toFloat();
    auto length = juce::jmin(bounds.getHeight(), bounds.getWidth()) * 0.07f;
    auto thickness = length * 0.5f;
    auto radian = 0.0f;

    auto topL = bounds.getTopLeft();
    auto topR = bounds.getTopRight();
    auto bottomR = bounds.getBottomRight();
    auto bottomL = bounds.getBottomLeft();

    std::array<juce::Point<float>, 4> corners{ topL, topR, bottomR, bottomL };

    // Draw in clockwise order, starting from top left.
    for (auto corner : corners) {
        juce::Path path;

        // vertical path
        path.startNewSubPath(corner);
        path.lineTo(corner.x, corner.y + length);

        // horizontal path
        path.startNewSubPath(corner);
        path.lineTo(corner.x + length, corner.y);

        g.strokePath(path, juce::PathStrokeType(thickness),
            juce::AffineTransform::rotation(radian, corner.x, corner.y));

        radian += juce::MathConstants<float>::halfPi;
    }//for
}//drawFocusMark()

void Dial::mouseDown(const juce::MouseEvent& event) {
    juce::Slider::mouseDown(event);
    setMouseCursor(juce::MouseCursor::NoCursor);
}//mouseDown()

void Dial::mouseDrag(const juce::MouseEvent& event) {
    juce::Slider::mouseDrag(event);

    if (event.mods.isShiftDown())
        setVelocityModeParameters(0.1, 1, 0.1, false);
    else
        setVelocityModeParameters(1.0, 1, 0.1, false);
}//mouseDrag()

void Dial::mouseUp(const juce::MouseEvent& event) {
    juce::Slider::mouseUp(event);

    juce::Desktop::getInstance().getMainMouseSource().setScreenPosition(event.source.getLastMouseDownPosition());

    setMouseCursor(juce::MouseCursor::NormalCursor);
}//mouseUp()

bool Dial::keyPressed(const juce::KeyPress& k) {
    if ('0' <= k.getKeyCode() && k.getKeyCode() <= '9') {
        CustomLookAndFeel::CustomLabel::initialValue = juce::String::charToString(k.getTextCharacter());
        showTextBox();
        return true;
    }//if
    return false;
}//keyPressed()