/*
  ==============================================================================

    OSCData.h
    Created: 25 Feb 2022 2:39:33am
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using std::sin;
using std::asin;

using namespace juce::dsp;

class OSCData : juce::dsp::Oscillator<float> {
public:
    void changeWaveType(const int choice);
    void setWaveFrequency(const int midiNoteNumber);
    void prepareToPlay(juce::dsp::ProcessSpec &spec);
    void getNextAudioBlock(juce::dsp::AudioBlock<float> &block);
    void setFMParameters(const float depth, const float freq);

private:
    // Frequency modulation synthesis
    juce::dsp::Oscillator<float> fmOsc { [](float x) { return sin(x); } };

    //using Filter = IIR::Filter<Type>;
    //using FilterCoefs = IIR::Coefficients<Type>;
    //ProcessorChain<ProcessorDuplicator<Filter, FilterCoefs>, Gain<Type>, WaveShaper<Type>, Gain<Type>> processorChain;

    float fmWave { 0.0f };
    float fmDepth { 0.0f };
    int lastMidiNote { 0 };
};