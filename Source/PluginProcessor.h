/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "SynthVoice.h"

//==============================================================================
/**
*/
class CapstoneSynthAudioProcessor : public juce::AudioProcessor {
public:
    //==============================================================================

    CapstoneSynthAudioProcessor();
    ~CapstoneSynthAudioProcessor() override;

    //==============================================================================

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState valueTree;
    const int numVoices = 20;

    juce::MidiMessageCollector& getMidiMessageCollector() noexcept { return midiMessageCollector; }
    AudioBufferQueue<float>& getAudioBufferQueue() noexcept { return audioBufferQueue; }

private:
    //AudioEngine AUDIOENGINE;
    juce::Synthesiser synth;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    juce::MidiMessageCollector midiMessageCollector;
    AudioBufferQueue<float> audioBufferQueue;
    ScopeDataCollector<float> scopeDataCollector{ audioBufferQueue };

    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CapstoneSynthAudioProcessor)
};
