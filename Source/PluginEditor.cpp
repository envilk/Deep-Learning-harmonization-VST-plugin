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
    setSize (600, 320);
    
    generateButton.onClick = [this]() { processInput(); };

    tempo.onValueChange = [this]() { audioProcessor.setTempoFromEditor(tempo.getValue()); };
    
    addAndMakeVisible(generateButton);
    addAndMakeVisible(midiOutput);
    addAndMakeVisible(tempo);
    addAndMakeVisible(labelDial1);
    addAndMakeVisible(temperature);
    addAndMakeVisible(labelDial2);
    
    tempo.setRange(10,200,1);
    tempo.setValue(120);
    tempo.setSliderStyle(juce::Slider::Rotary);
    tempo.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);

    //labelDial1.setFont(juce::Font(16.0f, juce::Font::bold));
    //labelDial1.setColour(juce::Label::textColourId, juce::Colours::white);
    //labelDial1.setJustificationType(juce::Justification::centred);
    labelDial1.setText("Tempo", juce::dontSendNotification);
    labelDial1.attachToComponent(&tempo, false);
    
    temperature.setRange(0.1,5.0,0.1);
    temperature.setValue(0.1);
    temperature.setSliderStyle(juce::Slider::Rotary);
    temperature.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);
    
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
    polyphonyRNN.generateHarmonization(temperature.getValue(), midiProcessor.inputPath);

    //Generated notification
    midiOutput.setText("The generated files can be found at: /home/enrique/HarmonizationMachine/outputs", true);
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
    juce::Rectangle <int> parameters = getLocalBounds().removeFromTop(280);
    juce::Rectangle <int> generateButtonArea = parameters.removeFromBottom(40);
    juce::Rectangle <int> parameter1 = parameters.removeFromLeft(300);
    juce::Rectangle <int> midiOutputArea = getLocalBounds().removeFromBottom(40);
    generateButton.setBounds(generateButtonArea);
    tempo.setBounds(parameter1);
    temperature.setBounds(parameters);
    midiOutput.setBounds(midiOutputArea);
    
}
