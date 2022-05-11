/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CapstoneSynthAudioProcessor::CapstoneSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), valueTree(*this, nullptr, "Parameters", createParameters())
#endif
{
    synth.addSound(new SynthSound());
    // Polyphony
    for(auto i = 0; i < numVoices; ++i)
        synth.addVoice(new SynthVoice());
}

CapstoneSynthAudioProcessor::~CapstoneSynthAudioProcessor() {}

//==============================================================================
const juce::String CapstoneSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CapstoneSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool CapstoneSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool CapstoneSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double CapstoneSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CapstoneSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int CapstoneSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CapstoneSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String CapstoneSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void CapstoneSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void CapstoneSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    midiMessageCollector.reset(sampleRate);

    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*> (synth.getVoice(i)))
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    }//for

    //AUDIOENGINE.prepare({ sampleRate, (juce::uint32)samplesPerBlock, 2 });
    midiMessageCollector.reset(sampleRate);
}

void CapstoneSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool CapstoneSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void CapstoneSynthAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    midiMessageCollector.removeNextBlockOfMessages(midiMessages, buffer.getNumSamples());

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for (auto i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            // Oscillator controls
            auto &OSCWaveChoice = *valueTree.getRawParameterValue("OSCWAVETYPE");
            voice->getOscillator().changeWaveType(OSCWaveChoice);
            auto &OSC2WaveChoice = *valueTree.getRawParameterValue("OSC2WAVETYPE");
            voice->getOscillator2().changeWaveType(OSC2WaveChoice);

            auto& FMDepth = *valueTree.getRawParameterValue("FMDEPTH");
            auto& FMFreq = *valueTree.getRawParameterValue("FMFREQ");
            auto& FMDepth2 = *valueTree.getRawParameterValue("FMDEPTH2");
            auto& FMFreq2 = *valueTree.getRawParameterValue("FMFREQ2");
            voice->getOscillator().setFMParameters(FMDepth, FMFreq);
            voice->getOscillator2().setFMParameters(FMDepth2, FMFreq2);
            
            // ADSR envelope
            auto &attack  = *valueTree.getRawParameterValue("ATTACK");
            auto &decay   = *valueTree.getRawParameterValue("DECAY");
            auto &sustain = *valueTree.getRawParameterValue("SUSTAIN");
            auto &release = *valueTree.getRawParameterValue("RELEASE");
            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
            
            // Filter
            //auto& filterType = *valueTree.getRawParameterValue("FILTERTYPE");
            //auto& freqCutoff = *valueTree.getRawParameterValue("FILTERCUTOFF");
            //auto& resonance = *valueTree.getRawParameterValue("FILTERRES");
            //voice->updateFilter(filterType.load(), freqCutoff.load(), resonance.load());
            auto& filterType = *valueTree.getRawParameterValue("FILTERTYPE");
            auto& freqCutoff = *valueTree.getRawParameterValue("FILTERCUTOFF");
            auto& resonance = *valueTree.getRawParameterValue("FILTERRES");
            voice->updateFilter(filterType.load(), freqCutoff.load(), resonance.load());

            // ADSR filter
            auto &modAttack  = *valueTree.getRawParameterValue("MODATTACK");
            auto &modDecay   = *valueTree.getRawParameterValue("MODDECAY");
            auto &modSustain = *valueTree.getRawParameterValue("MODSUSTAIN");
            auto &modRelease = *valueTree.getRawParameterValue("MODRELEASE");
            voice->updateModADSR(modAttack.load(), modDecay.load(), modSustain.load(), modRelease.load());

            // Reverb
            auto& roomSize = *valueTree.getRawParameterValue("ROOMSIZE");
            auto& damping = *valueTree.getRawParameterValue("DAMPING");
            auto& width = *valueTree.getRawParameterValue("WIDTH");
            auto& wetLevel = *valueTree.getRawParameterValue("WET&DRY");
            auto& dryLevel = *valueTree.getRawParameterValue("WET&DRY");
            voice->updateReverb(roomSize.load(), damping.load(), width.load(), wetLevel.load(), dryLevel.load());
        }//if
    }//for    

    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    scopeDataCollector.process(buffer.getReadPointer(0), (size_t)buffer.getNumSamples());
}

//==============================================================================
bool CapstoneSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* CapstoneSynthAudioProcessor::createEditor()
{
    return new CapstoneSynthAudioProcessorEditor (*this);
}

//==============================================================================
void CapstoneSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void CapstoneSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter(){ return new CapstoneSynthAudioProcessor(); }

// Value Tree
juce::AudioProcessorValueTreeState::ParameterLayout CapstoneSynthAudioProcessor::createParameters() {
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    // Switch oscillator
    params.add(make_unique<juce::AudioParameterChoice>
        ("OSC", "Oscillator",  juce::StringArray{"Sin", "Square", "Triangle", "Saw"}, 0));
    params.add(make_unique<juce::AudioParameterChoice>
        ("OSCWAVETYPE", "Osc 1 Wave Type", juce::StringArray("Sin", "Square", "Triangle", "Saw"), 0));
    
    params.add(make_unique<juce::AudioParameterChoice>
        ("OSC2", "Oscillator2", juce::StringArray{ "Sin", "Square", "Triangle", "Saw" }, 0));
    params.add(make_unique<juce::AudioParameterChoice>
        ("OSC2WAVETYPE", "Osc 2 Wave Type", juce::StringArray("Sin", "Square", "Triangle", "Saw"), 0));

    // ADSR Envelope
    params.add(make_unique<juce::AudioParameterFloat>
        ("ATTACK",  "Attack",  juce::NormalisableRange<float>{0.1f, 1.0f, 0.01f}, 0.25f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("DECAY",   "Decay",   juce::NormalisableRange<float>{0.1f, 1.0f, 0.01f}, 0.25f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("SUSTAIN", "Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.01f}, 0.75f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("RELEASE", "Release", juce::NormalisableRange<float>{0.1f, 4.0f, 0.01f}, 0.33f));

    // Frequency Modulation
    params.add(make_unique<juce::AudioParameterFloat>
        ("FMFREQ", "FM Frequency", juce::NormalisableRange<float>{0.1f, 100.0f, 0.01f, 0.5f}, 0.1f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("FMDEPTH", "FM Depth", juce::NormalisableRange<float>{0.1f, 100.0f, 0.01f, 0.5f}, 0.1f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("FMFREQ2", "FM Frequency 2", juce::NormalisableRange<float>{0.1f, 100.0f, 0.01f, 0.5f}, 0.1f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("FMDEPTH2", "FM Depth 2", juce::NormalisableRange<float>{0.1f, 100.0f, 0.01f, 0.5f}, 0.1f));

    // Filter
    params.add(make_unique<juce::AudioParameterChoice>
        ("FILTERTYPE", "Filter Type", juce::StringArray{"Low-Pass", "Band-Pass", "High-Pass"}, 0));
    params.add(make_unique<juce::AudioParameterFloat>
        ("FILTERCUTOFF", "Filter Cutoff", juce::NormalisableRange<float>{ 20.0, 20000.0, 0.5f, 0.6f}, 440.0f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("FILTERRES", "Filter Resonance", juce::NormalisableRange<float>{ 0.5, 20.0, 0.5f}, 1.0/std::sqrt(2.0)));

    // Filter ADSR
    params.add(make_unique<juce::AudioParameterFloat>
        ("MODATTACK", "Mod Attack", juce::NormalisableRange<float>{0.1f, 1.0f, 0.01f}, 0.25f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("MODDECAY", "Mod Decay", juce::NormalisableRange<float>{0.1f, 1.0f, 0.01f}, 0.25f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("MODSUSTAIN", "Mod Sustain", juce::NormalisableRange<float>{0.1f, 1.0f, 0.01f}, 0.75f));
    params.add(make_unique<juce::AudioParameterFloat>
        ("MODRELEASE", "Mod Release", juce::NormalisableRange<float>{0.1f, 4.0f, 0.01f}, 0.33f));

    params.add(make_unique<juce::AudioParameterFloat>
        ("ROOMSIZE", "Room size", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), 0.5f, juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {
            if (value * 100 < 10.0f)
                return juce::String(value * 100, 2);
            else if (value * 100 < 100.0f)
                return juce::String(value * 100, 1);
            else
                return juce::String(value * 100, 0);
        },
        nullptr));

    params.add(make_unique<juce::AudioParameterFloat>
        ("DAMPING", "Damping", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), 0.5f, juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {
            if (value * 100 < 10.0f)
                return juce::String(value * 100, 2);
            else if (value * 100 < 100.0f)
                return juce::String(value * 100, 1);
            else
                return juce::String(value * 100, 0);
        },
        nullptr));

    params.add(make_unique<juce::AudioParameterFloat>
        ("WIDTH", "Width", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), 0.5f, juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {
            if (value * 100 < 10.0f)
                return juce::String(value * 100, 2);
            else if (value * 100 < 100.0f)
                return juce::String(value * 100, 1);
            else
                return juce::String(value * 100, 0);
        },
        nullptr));

    params.add(make_unique<juce::AudioParameterFloat>
        ("WET&DRY", "Wet & Dry level", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f, 1.0f), 0.5f, juce::String(),
        juce::AudioProcessorParameter::genericParameter,
        [](float value, int) {
            if (value * 100 < 10.0f)
                return juce::String(value * 100, 2);
            else if (value * 100 < 100.0f)
                return juce::String(value * 100, 1);
            else
                return juce::String(value * 100, 0);
        },
        nullptr));

    return { params };
}//createParameters()