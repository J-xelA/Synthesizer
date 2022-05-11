/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CapstoneSynthAudioProcessorEditor::CapstoneSynthAudioProcessorEditor(CapstoneSynthAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    OSC(audioProcessor.valueTree, "OSCWAVETYPE", "FMFREQ", "FMDEPTH"),
    OSC2(audioProcessor.valueTree, "OSC2WAVETYPE", "FMFREQ2", "FMDEPTH2"),
    ADSR(audioProcessor.valueTree, "ADSR Envelope", "ATTACK", "DECAY", "SUSTAIN", "RELEASE"),
    OSCILLOSCOPE(audioProcessor.getAudioBufferQueue()),
    REVERB(audioProcessor.valueTree, "Reverb", "ROOMSIZE", "DAMPING", "WIDTH", "WET&DRY"),
    FILTER(audioProcessor.valueTree, "FILTERTYPE", "FILTERCUTOFF", "FILTERRES"),
    modADSR(audioProcessor.valueTree, "Mod Envelope", "MODATTACK", "MODDECAY", "MODSUSTAIN", "MODRELEASE")
    {
    //const double min = 750;
    //const double max = 900;
    //const double ratio = 5.0 / 3.0;

    //setResizable(true, true);
    //setResizeLimits(min, juce::roundToInt(min / ratio), max, juce::roundToInt(max / ratio));
    //getConstrainer()->setFixedAspectRatio(ratio);
    setSize(800, 480);

    addAndMakeVisible(OSC);
    addAndMakeVisible(OSC2);

    addAndMakeVisible(OSCILLOSCOPE);

    addAndMakeVisible(ADSR);

    addAndMakeVisible(REVERB);

    addAndMakeVisible(FILTER);
    //addAndMakeVisible(modADSR);

    //midiKeyboardState.addListener(&dspProcessor.getMidiMessageCollector());

    juce::File backgroundFile = juce::File::getCurrentWorkingDirectory().getChildFile("../../Images/two.jpg");
    image = jpeg->loadFrom(backgroundFile);
}

CapstoneSynthAudioProcessorEditor::~CapstoneSynthAudioProcessorEditor() {}

//==============================================================================
void CapstoneSynthAudioProcessorEditor::paint (juce::Graphics& g) {
    //g.drawImageAt(image, 0, 0);
    g.fillAll(CustomColours::black);// steelblue);
}

void CapstoneSynthAudioProcessorEditor::resized() {
    const auto paddingX = 5;
    const auto paddingY = 5;
    
    // Set OSC bounds
    OSC.setBounds(paddingX, paddingY, getWidth()/2.75 - paddingX, getHeight()/3 - paddingY);
    OSC2.setBounds(paddingX, getHeight()/3, getWidth()/2.75 - paddingX, getHeight()/3 - paddingY);

    // Set OSCILLOSCOPE bounds
    OSCILLOSCOPE.setBounds(paddingX, (getHeight() / 3) * 2, getWidth() / 2.75 - paddingX, getHeight() / 3 - paddingY);
    
    // Set ADSR bounds
    ADSR.setBounds(OSC.getRight(), paddingY, getWidth()/5 - paddingX-15, getHeight() - paddingY);
    
    // Set Reverb bounds
    REVERB.setBounds(ADSR.getRight(), paddingY, getWidth()/5 - paddingX-15, getHeight() - paddingY);

    // Set FILTER bounds
    FILTER.setBounds(REVERB.getRight(), paddingY, getWidth() / 3.55, getHeight()/1.8 - paddingY);
    // Set mod ADSR bounds
    // modADSR.setBounds(FILTER.getBottom(), getHeight()/2, getWidth()/2-paddingX, getHeight()/2-paddingY);
}