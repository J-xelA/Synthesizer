/*
  ==============================================================================

    FilterData.h
    Created: 23 Mar 2022 11:04:54pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class FilterData {
public:
    void prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels);
    void process(juce::AudioBuffer<float>& buffer);
    void updateParams(const int filterType, const float freqCutoff, const float resonance, const float modulator = 1.0f);
    void reset();
private:
    //juce::dsp::StateVariableTPTFilter<float> FILTER;
    juce::dsp::ProcessorDuplicator
        <juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> filter;

    bool isPrepared{ false };
};