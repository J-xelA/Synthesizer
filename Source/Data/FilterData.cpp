/*
  ==============================================================================

    FilterData.cpp
    Created: 23 Mar 2022 11:04:54pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include "FilterData.h"

void FilterData::prepareToPlay(double sampleRate, int samplesPerBlock, int numChannels) {
    filter.reset();
    juce::dsp::ProcessSpec spec;

    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numChannels;

    filter.prepare(spec);

    isPrepared = true;
}//prepareToPlay()

void FilterData::process(juce::AudioBuffer<float>& buffer) {
    jassert(isPrepared);
    juce::dsp::AudioBlock<float> block { buffer };
    filter.process(juce::dsp::ProcessContextReplacing<float> { block });
}//processFilter()

void FilterData::updateParams( const int filterType, const float freqCutoff, const float resonance, const float modulator) {
    switch (filterType){
        case 0: 
            *filter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(44100, freqCutoff, resonance);
            break;
        case 1:
            *filter.state = *juce::dsp::IIR::Coefficients<float>::makeBandPass(44100, freqCutoff, resonance);
            break;
        case 2:
            *filter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(44100, freqCutoff, resonance);
            break;
    }

    float modulatedFreq = freqCutoff * modulator;
    modulatedFreq = std::fmax(std::fmin(modulatedFreq, 20.0f), 20000.0f);
}//updateParams()

void FilterData::reset() {
    filter.reset();
}//reset()