/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HarmonizationmachineAudioProcessorEditor::HarmonizationmachineAudioProcessorEditor (HarmonizationmachineAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    generateButton.onClick = [this]() { processInput(); };
    addAndMakeVisible(generateButton);
    addAndMakeVisible(midiInput);
    addAndMakeVisible(midiOutput);
}

HarmonizationmachineAudioProcessorEditor::~HarmonizationmachineAudioProcessorEditor()
{
}

void HarmonizationmachineAudioProcessorEditor::processInput()
{
    //Magenta model activation
    audioProcessor.magentaSystemCall(midiInput.getText());

    //Generated notification
    addAndMakeVisible(notification);
}

//==============================================================================
void HarmonizationmachineAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void HarmonizationmachineAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    juce::Rectangle <int> midiOutputArea = getLocalBounds().removeFromBottom(200);
    juce::Rectangle <int> midiInputArea = midiOutputArea.removeFromTop(100);
    juce::Rectangle <int> generateButtonArea = getLocalBounds().removeFromTop(100);
    generateButton.setBounds(generateButtonArea);
    midiInput.setBounds(midiInputArea);
    midiOutput.setBounds(midiOutputArea);
    notification.setBounds(generateButtonArea);
}
