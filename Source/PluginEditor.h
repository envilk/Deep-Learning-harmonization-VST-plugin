/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PolyphonyRNN.h"
#include "MidiProcessor.h"

//==============================================================================
/**
*/
class HarmonizationmachineAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HarmonizationmachineAudioProcessorEditor (HarmonizationmachineAudioProcessor&);
    ~HarmonizationmachineAudioProcessorEditor() override;

    // New methods
    void processInput();
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the p
    HarmonizationmachineAudioProcessor& audioProcessor;
    PolyphonyRNN polyphonyRNN;
    MidiProcessor midiProcessor;

    juce::TextButton generateButton{"Generate"};
    juce::TextEditor midiOutput;
    juce::Slider tempo;//1 to 200, default 120
    juce::Slider temperature;//0.1 to 5.0, default 0.1
    juce::Label labelDial1;
    juce::Label labelDial2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizationmachineAudioProcessorEditor)
};
