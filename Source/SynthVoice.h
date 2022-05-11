/*
  ==============================================================================

    SynthVoice.h
    Created: 2 Feb 2022 10:53:05pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include "SynthSound.h"
#include "Data/ADSRData.h"
#include "Data/OSCData.h"
#include "Data/FilterData.h"
#include "Data/ReverbData.h"
//#include "Data/CustomOscillator.h"
//#include "Data/AudioEngine.h"
#include "UI/WaveformComponent.h"

using namespace juce::dsp;
using std::sin;

class SynthVoice : public juce::SynthesiserVoice {
public:
    bool canPlaySound(juce::SynthesiserSound* sound) override;
    
    void startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    void stopNote(float velocity, bool allowTailOff) override;
    
    void pitchWheelMoved(int newPitchWheelValue) override;
    void controllerMoved(int controllerNumber, int newControllerValue) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    void renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;
    
    void updateADSR(const float attack, const float decay, const float sustain, const float release);
    void updateFilter(const int filterType, const float freqCutoff, const float resonance);
    void updateModADSR(const float attack, const float decay, const float sustain, const float release);
    void updateReverb(const float roomSize, const float damping, const float width, const float wetLevel, const float dryLevel);

    ADSRData &getADSR() { return ADSR; }
    OSCData &getOscillator() { return OSC; }
    OSCData &getOscillator2() { return OSC2; }

private:
    
    juce::AudioBuffer<float> synthBuffer;

    OSCData OSC, OSC2;
    ADSRData ADSR;
    FilterData FILTER;
    ADSRData modADSR;

    ReverbData leftREVERB, rightREVERB;

    
    juce::dsp::Gain<float> gain;

    bool isPrepared { false };

    int noteNum;

    float pitch = 0.0f,
        pitchBend = 0.0f,
        pitchUp = 2.0f,
        pitchDown = 2.0f,
        volume = 0.1f;

    double
        tuning = 0.0, 
        level;
    
};