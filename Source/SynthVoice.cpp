/*
  ==============================================================================

    SynthVoice.cpp
    Created: 2 Feb 2022 10:53:05pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include "SynthVoice.h"
/*
void SynthVoice::prepare(const juce::dsp::ProcessSpec& spec) {
    tempBlock = juce::dsp::AudioBlock<float>(heapBlock, spec.numChannels, spec.maximumBlockSize);
    processorChain.prepare(spec);

    lfo.prepare({ spec.sampleRate / lfoUpdateRate, spec.maximumBlockSize, spec.numChannels });
}//prepare()

void SynthVoice::noteStarted() {
    auto velocity = getCurrentlyPlayingNote().noteOnVelocity.asUnsignedFloat();
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();

    processorChain.get<osc1Index>().setFrequency(freqHz, true);
    processorChain.get<osc1Index>().setLevel(velocity);

    processorChain.get<osc2Index>().setFrequency(freqHz * 1.01f, true);
    processorChain.get<osc2Index>().setLevel(velocity);
}//noteStarted()

void SynthVoice::notePitchbendChanged() {
    auto freqHz = (float)getCurrentlyPlayingNote().getFrequencyInHertz();
    processorChain.get<osc1Index>().setFrequency(freqHz);
    processorChain.get<osc2Index>().setFrequency(freqHz * 1.01f);
}//notePitchbendChanged()

void SynthVoice::noteStopped(bool) {
    clearCurrentNote();
}//noteStopped()

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    auto output = tempBlock.getSubBlock(0, (size_t)numSamples);
    output.clear();

    for(size_t pos = 0; pos < (size_t)numSamples;) {
        auto max = juce::jmin((size_t)numSamples - pos, lfoUpdateCounter);
        auto block = output.getSubBlock(pos, max);

        juce::dsp::ProcessContextReplacing<float> context(block);
        processorChain.process(context);

        pos += max;
        lfoUpdateCounter -= max;

        if (lfoUpdateCounter == 0) {
            lfoUpdateCounter = lfoUpdateRate;
            auto lfoOut = lfo.processSample(0.0f);
            auto curoffFreqHz = juce::jmap(lfoOut, -1.0f, 1.0f, 100.0f, 2000.0f);
            processorChain.get<filterIndex>().setCutoffFrequencyHz(curoffFreqHz);
        }//if
    }//for

    juce::dsp::AudioBlock<float>(outputBuffer)
        .getSubBlock((size_t)startSample, (size_t)numSamples)
        .add(tempBlock);
}//renderNextBlock
*/


// Returns true if a sound is playable (not nullptr)
bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound) {
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}//canPlaySound

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) {
    //noteNum = midiNoteNumber;
    OSC.setWaveFrequency(midiNoteNumber);
    OSC2.setWaveFrequency(midiNoteNumber);
    level = velocity;
    ADSR.noteOn();
    modADSR.noteOn();
}//startNote

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    ADSR.noteOff();
    modADSR.noteOff();
    if (!allowTailOff || !ADSR.isActive())
        SynthesiserVoice::clearCurrentNote();
}//stopNote

void SynthVoice::pitchWheelMoved(int newPitchWheelValue) {
    pitch = (newPitchWheelValue < 8192) ? 0.5 + (newPitchWheelValue / 16384) :
        2.0 * (newPitchWheelValue / 16384);
}//pitchWheelMoved

void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
    // Modulation Wheel
    if (controllerNumber == 1)
        tuning = newControllerValue / 127;
    
    // Volume Knob
    if (controllerNumber == 7)
        volume = newControllerValue / 127;
}//controllerMoved

void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;

    OSC.prepareToPlay(spec);
    OSC2.prepareToPlay(spec);
    
    FILTER.prepareToPlay(sampleRate, samplesPerBlock, outputChannels);

    ADSR.setSampleRate(sampleRate);
    modADSR.setSampleRate(sampleRate);

    gain.prepare(spec);

    leftREVERB.prepare(spec);
    rightREVERB.prepare(spec);

    isPrepared = true;
}//prepareToPlay

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    jassert(isPrepared); // Make sure we've called prepareToPlay() first

    if (!isVoiceActive())
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    modADSR.applyEnvelopeToBuffer(synthBuffer, 0, numSamples);
    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };

    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    OSC.getNextAudioBlock(audioBlock);
    OSC2.getNextAudioBlock(audioBlock);

    auto leftBlock = audioBlock.getSingleChannelBlock(0);
    auto rightBlock = audioBlock.getSingleChannelBlock(1);
    leftREVERB.process(juce::dsp::ProcessContextReplacing<float>(leftBlock));
    rightREVERB.process(juce::dsp::ProcessContextReplacing<float>(leftBlock));

    ADSR.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());
    
    FILTER.process(synthBuffer);
    
    synthBuffer.applyGain(0.05f); // VOLUME CONTROL

    for (int curChannel = 0; curChannel < outputBuffer.getNumChannels(); ++curChannel) {
        outputBuffer.addFrom(curChannel, startSample, synthBuffer, curChannel, 0, numSamples);
        if (!ADSR.isActive())
            SynthesiserVoice::clearCurrentNote();
    }//for
}//renderNextBlock

void SynthVoice::updateADSR(const float attack, const float decay, const float sustain, const float release) {
    ADSR.update(attack, decay, sustain, release);
}

void SynthVoice::updateFilter(const int filterType, const float freqCutoff, const float resonance) {
    auto modulator = modADSR.getNextSample();
    FILTER.updateParams(filterType, freqCutoff, resonance, modulator);
}

void SynthVoice::updateModADSR(const float attack, const float decay, const float sustain, const float release) {
    modADSR.update(attack, decay, sustain, release);
}

void SynthVoice::updateReverb(const float roomSize, const float damping, const float width, const float wetLevel, const float dryLevel) {
    leftREVERB.update(roomSize, damping, width, wetLevel, 1.0f-dryLevel);
    rightREVERB.update(roomSize, damping, width, wetLevel, 1.0f-dryLevel);
}