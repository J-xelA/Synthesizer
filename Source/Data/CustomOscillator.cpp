/*
  ==============================================================================

    CustomOscillator.cpp
    Created: 27 Apr 2022 7:14:09pm
    Author:  Alex Johnson

  ==============================================================================
*/

#include "CustomOscillator.h"

template <typename Type>
void CustomOscillator<Type>::setFrequency(Type newVal, bool force = false) {
    auto &osc = processorChain.template get<oscIndex>();
    osc.setFrequency(newValue, force);
}//setFrequency()

template <typename Type>
void CustomOscillator<Type>::setLevel(Type newVal) {
    auto &gain = processorChain.template get<gainIndex>();
    gain.setGainLinear(newValue);
}//setLevel()

template <typename Type>
void CustomOscillator<Type>::prepare(const juce::dsp::ProcessSpec& spec) {
    processorChain.prepare(spec);
}//prepare()