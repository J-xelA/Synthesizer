/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/ADSRComponent.h"
#include "UI/OSCComponent.h"
#include "UI/OSC2Component.h"
#include "UI/FilterComponent.h"
#include "UI/WaveformComponent.h"
#include "UI/ReverbComponent.h"
#include "UI/CustomLookAndFeel.h"

using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
//==============================================================================
/**
*/
class CapstoneSynthAudioProcessorEditor  : public juce::AudioProcessorEditor {
public:
    CapstoneSynthAudioProcessorEditor (CapstoneSynthAudioProcessor&);
    ~CapstoneSynthAudioProcessorEditor() override;

    //==============================================================================

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CapstoneSynthAudioProcessor &audioProcessor;
    OSCComponent OSC;
    OSC2Component OSC2;
    
    WaveformComponent<float> OSCILLOSCOPE;
    
    ADSRComponent ADSR;
    
    ReverbComponent REVERB;

    FilterComponent FILTER;
    ADSRComponent modADSR;

    CustomLookAndFeel CLAF;
    juce::Image image;
    juce::JPEGImageFormat* jpeg; // Convert background image format

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CapstoneSynthAudioProcessorEditor)
};
