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
    setSize (600, 400);
    
    generateButton.onClick = [this]() { processInput(); };
    
    addAndMakeVisible(generateButton);
    addAndMakeVisible(midiInput);
    addAndMakeVisible(midiOutput);
    
    qpm.setRange(10,200, 1);
    qpm.setValue(120);
    qpm.setSliderStyle(juce::Slider::Rotary);
    qpm.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(qpm);
    addAndMakeVisible(labelDial1);
    
    labelDial1.setFont(juce::Font(16.0f, juce::Font::bold));
    labelDial1.setColour(juce::Label::textColourId, juce::Colours::white);
    labelDial1.setJustificationType(juce::Justification::centred);
    labelDial1.setText("Tempo", juce::dontSendNotification);
    labelDial1.attachToComponent(&qpm, false);
    
    
    temperature.setRange(0.1,5.0,0.1);
    temperature.setValue(0.1);
    temperature.setSliderStyle(juce::Slider::Rotary);
    temperature.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    addAndMakeVisible(temperature);
    addAndMakeVisible(labelDial2);
    
    labelDial2.setFont(juce::Font(16.0f, juce::Font::bold));
    labelDial2.setColour(juce::Label::textColourId, juce::Colours::white);
    labelDial2.setJustificationType(juce::Justification::centred);
    labelDial2.setText("Randomness", juce::dontSendNotification);
    labelDial2.attachToComponent(&temperature, false);
    
}

HarmonizationmachineAudioProcessorEditor::~HarmonizationmachineAudioProcessorEditor()
{
}

void HarmonizationmachineAudioProcessorEditor::processInput()
{
    //Clear Generated notification
    midiOutput.setText("", true);
    
    //Magenta model activation
    polyphonyRNN.generateHarmonization(midiInput.getText(), temperature.getValue(), qpm.getValue());

    //Generated notification
    midiOutput.setText("GENERATED!", true);
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
    juce::Rectangle <int> parameters = getLocalBounds().removeFromTop(360);
    juce::Rectangle <int> generateButtonArea = parameters.removeFromBottom(40);
    juce::Rectangle <int> midiInputArea = parameters.removeFromBottom(80);
    juce::Rectangle <int> parameter1 = parameters.removeFromLeft(300);
    juce::Rectangle <int> midiOutputArea = getLocalBounds().removeFromBottom(40);
    generateButton.setBounds(generateButtonArea);
    qpm.setBounds(parameter1);
    temperature.setBounds(parameters);
    midiInput.setBounds(midiInputArea);
    midiOutput.setBounds(midiOutputArea);
}
