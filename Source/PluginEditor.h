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
#include "InputFileBrowser.h"

//==============================================================================
/**
*/
class HarmonizationmachineAudioProcessorEditor  : public juce::AudioProcessorEditor, 
                                                  private juce::FileBrowserListener
{
public:
    HarmonizationmachineAudioProcessorEditor (HarmonizationmachineAudioProcessor&);
    ~HarmonizationmachineAudioProcessorEditor() override;
    
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    // New methods
    void processInput();
    void fileDoubleClicked (const juce::File& p_file) override;

    void selectionChanged() override;
    void fileClicked (const juce::File&, const juce::MouseEvent&) override;
    void browserRootChanged (const juce::File&) override;

private:
    HarmonizationmachineAudioProcessor& audioProcessor;
    PolyphonyRNN polyphonyRNN;
    MidiProcessor midiProcessor;
    //InputFileBrowser inputFileBrowser;
    //OutputDragAndDrop outputDragAndDrop;

    juce::TextButton generateButton{"Generate"};
    juce::Slider tempo;//1 to 200, default 120
    juce::Slider temperature;//0.1 to 5.0, default 0.1
    juce::Label labelDial1;
    juce::Label labelDial2;

    juce::TimeSliceThread thread  { "file tree browser" };
    juce::DirectoryContentsList directoryList {nullptr, thread};
    juce::FileTreeComponent inputFileBrowser {directoryList};
    juce::Label explanation { {}, "Select a MIDI file in the treeview above" };

    bool isFileDoubleClicked = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizationmachineAudioProcessorEditor)
};
