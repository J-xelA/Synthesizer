/*
  ==============================================================================

    AudioEngine.h
    Created: 27 Apr 2022 7:14:18pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../SynthVoice.h"
/*
class AudioEngine : public juce::MPESynthesiser {
public:
    static constexpr auto numVoices = 12;

    AudioEngine() {
        for (auto i = 0; i < numVoices; ++i)
            addVoice(new SynthVoice);

        setVoiceStealingEnabled(true);
    }//AudioEngine()

    void prepare(const juce::dsp::ProcessSpec& spec) noexcept {
        setCurrentPlaybackSampleRate(spec.sampleRate);

        for (auto* v : voices)
            dynamic_cast<SynthVoice*> (v)->prepare(spec);

        reverb.prepare(spec);
    }//prepare()
private:
    void renderNextSubBlock(juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples) override {
        MPESynthesiser::renderNextSubBlock(outputAudio, startSample, numSamples);

        auto block = juce::dsp::AudioBlock<float>(outputAudio);
        auto blockToUse = block.getSubBlock((size_t)startSample, (size_t)numSamples);
        auto contextToUse = juce::dsp::ProcessContextReplacing<float>(blockToUse);
        reverb.process(contextToUse);
    }//renderNextSubBlock()

    enum {
        reverbIndex
    };

    juce::dsp::ProcessorChain<juce::dsp::Reverb> reverb;
};
*/