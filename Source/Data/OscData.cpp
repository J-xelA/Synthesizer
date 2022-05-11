/*
  ==============================================================================

    OSCData.cpp
    Created: 25 Feb 2022 2:39:33am
    Author:  Alex Johnson

  ==============================================================================
*/

#include "OSCData.h"

void OSCData::changeWaveType(const int choice) {
    float PI = juce::MathConstants<float>::pi;
    switch(choice) {
        // Sin
        case 0:
            initialise([](float x) { return sin(x); }, 128);
            break;

        // Square
        case 1:
            initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 128);
            break;

        // Triangle
        case 2:
            initialise([PI](float x) { return ((2/PI) * (asin(sin(((2)*PI)*x)))); }, 128);
            break;

        // Sawtooth
        case 3:
            //initialise([PI](float x) { return x / PI; });
            initialise([PI](float x) { return juce::jmap(x, float(-PI), float(PI), float(-1), float(1)); }, 128);
            break;
            
        default:
            jassertfalse; // No wave type found
            break;
    }//switch
}//changeWaveType()

void OSCData::setWaveFrequency(const int midiNoteNumber) {
    setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmWave);
    lastMidiNote = midiNoteNumber;
}//setWaveFrequency()

void OSCData::prepareToPlay(juce::dsp::ProcessSpec &spec) {
    fmOsc.prepare(spec);
    prepare(spec);
}//prepareToPlay()

void OSCData::getNextAudioBlock(juce::dsp::AudioBlock<float> &block) {
    for(int channel = 0; channel < block.getNumChannels(); ++channel)
        for (int sample = 0; sample < block.getNumSamples(); ++sample)
            fmWave = (fmOsc.processSample(block.getSample(channel, sample)) * fmDepth) / 2;

    process(juce::dsp::ProcessContextReplacing<float>(block));
}//getNextAudioBlock()

void OSCData::setFMParameters(const float depth, const float freq) {
    fmOsc.setFrequency(freq);

    fmDepth = depth;
    
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz(lastMidiNote) + fmWave;
    
    setFrequency(currentFreq >= 0 ? currentFreq : currentFreq * -1.0);
}//setFMParameters