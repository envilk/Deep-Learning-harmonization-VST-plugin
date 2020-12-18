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
    juce::TextEditor midiOutput;
    juce::TextButton notification{ "Generated!" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizationmachineAudioProcessorEditor)
};
