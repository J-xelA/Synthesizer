/*
  ==============================================================================

    CustomOscillator.h
    Created: 27 Apr 2022 7:14:09pm
    Author:  Alex Johnson

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

template <typename Type>
class CustomOscillator {
public:
    CustomOscillator() {
        auto& osc = processorChain.template get<oscIndex>();
        osc.initialise([](Type x) { return sin(x); }, 128);
    }//CustomOscillator()

    enum class Waveform {
        sine, saw
    };

    template <typename ProcessContext>
    void process(const ProcessContext& context) noexcept {
        processorChain.process(context);
    }//process()

    void reset() noexcept {
        processorChain.reset();
    }//reset()

    void setFrequency(Type newVal, bool force = false);
    void setLevel(Type newVal);
    void prepare(const juce::dsp::ProcessSpec &spec);

private:
    juce::dsp::ProcessorChain<
        juce::dsp::Oscillator<Type>,
        juce::dsp::Gain<Type>> processorChain;

    enum {
        oscIndex,
        gainIndex,
    };
};