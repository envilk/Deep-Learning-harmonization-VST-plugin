/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class HarmonizationmachineAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    HarmonizationmachineAudioProcessorEditor (HarmonizationmachineAudioProcessor&);
    ~HarmonizationmachineAudioProcessorEditor() override;
    
    juce::TextEditor midiOutput;

    // New methods
    void processInput();
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    HarmonizationmachineAudioProcessor& audioProcessor;
    
    juce::TextButton generateButton{"Generate"};
    juce::TextEditor midiInput;
    juce::Slider qpm;//0 to 200, default 120
    juce::Slider temperature;//0.0 to 1.0, default 0
    juce::Label labelDial1;
    juce::Label labelDial2;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizationmachineAudioProcessorEditor)
};
