/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
HarmonizationmachineAudioProcessorEditor::HarmonizationmachineAudioProcessorEditor(HarmonizationmachineAudioProcessor &p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(600, 620);

    generateButton.onClick = [this]() { processInput(); };

    tempo.onValueChange = [this]() { audioProcessor.setTempoFromEditor(tempo.getValue()); };

    addAndMakeVisible(inputFileBrowser);
    addAndMakeVisible(generateButton);
    addAndMakeVisible(tempo);
    addAndMakeVisible(labelDial1);
    addAndMakeVisible(temperature);
    addAndMakeVisible(labelDial2);

    tempo.setRange(10, 200, 1);
    tempo.setValue(120);
    tempo.setSliderStyle(juce::Slider::Rotary);
    tempo.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20, 30);

    labelDial1.setSize(100, 30);
    labelDial1.setFont(juce::Font(16.0f, juce::Font::bold));
    labelDial1.setColour(juce::Label::textColourId, juce::Colours::white);
    labelDial1.setJustificationType(juce::Justification::centred);
    labelDial1.setText("Tempo", juce::dontSendNotification);
    labelDial1.attachToComponent(&tempo, false);

    temperature.setRange(0.1, 5.0, 0.1);
    temperature.setValue(0.1);
    temperature.setSliderStyle(juce::Slider::Rotary);
    temperature.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 20, 30);

    labelDial2.setSize(100, 30);
    labelDial2.setFont(juce::Font(16.0f, juce::Font::bold));
    labelDial2.setColour(juce::Label::textColourId, juce::Colours::white);
    labelDial2.setJustificationType(juce::Justification::centred);
    labelDial2.setText("Randomness", juce::dontSendNotification);
    labelDial2.attachToComponent(&temperature, false);

    addAndMakeVisible(inputFileBrowser);

    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userHomeDirectory), true, true);

    inputFileBrowser.setColour(juce::FileTreeComponent::backgroundColourId, juce::Colours::lightgrey.withAlpha(0.6f));
    inputFileBrowser.addListener(this);

    addAndMakeVisible(explanation);
    explanation.setFont(juce::Font(14.00f, juce::Font::plain));
    explanation.setJustificationType(juce::Justification::bottomRight);
    explanation.setEditable(false, false, false);
    explanation.setColour(juce::TextEditor::textColourId, juce::Colours::black);
    explanation.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0x00000000));

    thread.startThread(3); // FileTreeComponent thread communication with the editor (FileBrowserListener)
}

HarmonizationmachineAudioProcessorEditor::~HarmonizationmachineAudioProcessorEditor()
{
    inputFileBrowser.removeListener(this);
}

void HarmonizationmachineAudioProcessorEditor::processInput()
{
    //Magenta model activation
    if (!isFileDoubleClicked)
        polyphonyRNN.setInputPath(midiProcessor.inputPath);
    else
        isFileDoubleClicked = false;

    polyphonyRNN.generateHarmonization(temperature.getValue());
}

//==============================================================================
void HarmonizationmachineAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void HarmonizationmachineAudioProcessorEditor::resized()
{
    juce::Rectangle<int> parameter1 = getLocalBounds().removeFromTop(320);
    juce::Rectangle<int> generateButtonArea = parameter1.removeFromBottom(40);
    juce::Rectangle<int> parameter2 = parameter1.removeFromLeft(300);
    juce::Rectangle<int> inputFileBrowserArea = getLocalBounds().removeFromBottom(300);
    //juce::Rectangle <int> outputDragAndDropArea = inputDragAndDropArea.removeFromRight(300);
    generateButton.setBounds(generateButtonArea);
    tempo.setBounds(parameter1);
    temperature.setBounds(parameter2);
    inputFileBrowser.setBounds(inputFileBrowserArea);
    //outputDragAndDrop.setBounds(outputDragAndDropArea);
}

void HarmonizationmachineAudioProcessorEditor::fileDoubleClicked(const juce::File &p_file)
{
    isFileDoubleClicked = true;
    polyphonyRNN.setInputPath(p_file.getFullPathName().toStdString());
}

void HarmonizationmachineAudioProcessorEditor::selectionChanged() {}
void HarmonizationmachineAudioProcessorEditor::fileClicked(const juce::File &, const juce::MouseEvent &) {}
void HarmonizationmachineAudioProcessorEditor::browserRootChanged(const juce::File &) {}
