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

    juce::TextButton generateButton{"Generate"};
    juce::Slider tempo;//1 to 200, default 120
    juce::Slider temperature;//0.1 to 5.0, default 0.1
    juce::ComboBox modelComboBox  { "Model" };
    juce::Label tempoLabel;
    juce::Label temperatureLabel; 
    juce::Label modelLabel;

    juce::ToggleButton primerKeyToggle   { "Initial key" };
    juce::ComboBox primerKeyComboBox  { "Key" };
    juce::ToggleButton harmonizeToggle   { "Harmonization" };
    juce::Slider numSteps;
    juce::Slider numOutputs;
    juce::Label numStepsLabel;
    juce::Label numOutputsLabel;

    //auto metronome = juce::ImageCache::getFromMemory (juce::BinaryData, juce::BinaryData::file_pngSize);
    juce::Image metronome = juce::PNGImageFormat::loadFrom(juce::File("/home/enrique/Descargas/juce-6.0.5-linux/JUCE/PROJECTS/Harmonization machine/metronome.png"));
    juce::Image dice = juce::PNGImageFormat::loadFrom(juce::File("/home/enrique/Descargas/juce-6.0.5-linux/JUCE/PROJECTS/Harmonization machine/dice.png"));
    juce::Image network = juce::PNGImageFormat::loadFrom(juce::File("/home/enrique/Descargas/juce-6.0.5-linux/JUCE/PROJECTS/Harmonization machine/network.png"));
    juce::ImageComponent metronomeComponent;
    juce::ImageComponent diceComponent;
    juce::ImageComponent networkComponent;

    juce::TimeSliceThread thread  { "file tree browser" };
    juce::DirectoryContentsList directoryList {nullptr, thread};
    juce::FileTreeComponent inputFileBrowser {directoryList};
    juce::Label explanation { {}, "Select a MIDI file in the treeview above" };

    bool isFileDoubleClicked = false;

    juce::Path path_parameter1, path_parameter2, path_parameter3, path_parameter4, path_parameter5, path_parameter6, path_parameter7;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HarmonizationmachineAudioProcessorEditor)
};
