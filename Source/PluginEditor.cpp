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
    setSize(600, 550);

    generateButton.onClick = [this]() { processInput(); };

    tempo.onValueChange = [this]() { audioProcessor.setTempoFromEditor(tempo.getValue()); };

    modelComboBox.addItem ("Bach chorales", 1);
    modelComboBox.addItem ("Jazz", 2);

    modelComboBox.setSelectedId(1);

    metronomeComponent.setImage(metronome);
    diceComponent.setImage(dice);
    networkComponent.setImage(network);

    addAndMakeVisible(metronomeComponent);
    addAndMakeVisible(diceComponent);
    addAndMakeVisible(networkComponent);
    addAndMakeVisible(generateButton);
    addAndMakeVisible(tempo);
    addAndMakeVisible(tempoDial);
    addAndMakeVisible(temperature);
    addAndMakeVisible(temperatureDial);
    addAndMakeVisible(modelComboBox);
    addAndMakeVisible(modelLabel);

    tempo.setRange(10, 200, 1);
    tempo.setValue(120);
    tempo.setSliderStyle(juce::Slider::Rotary);
    tempo.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);

    tempoDial.setSize(200, 30);
    tempoDial.setFont(juce::Font(16.0f, juce::Font::bold));
    tempoDial.setColour(juce::Label::textColourId, juce::Colours::white);
    tempoDial.setJustificationType(juce::Justification::centred);
    tempoDial.setText("Tempo", juce::dontSendNotification);

    modelComboBox.setEditableText(false);
    modelComboBox.setJustificationType(juce::Justification::centred);

    modelLabel.setSize(200, 30);
    modelLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    modelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    modelLabel.setJustificationType(juce::Justification::centred);
    modelLabel.setText("Harmonization style", juce::dontSendNotification);

    temperature.setRange(0.1, 5.0, 0.1);
    temperature.setValue(1.0);
    temperature.setSliderStyle(juce::Slider::Rotary);
    temperature.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 30);

    temperatureDial.setSize(200, 30);
    temperatureDial.setFont(juce::Font(16.0f, juce::Font::bold));
    temperatureDial.setColour(juce::Label::textColourId, juce::Colours::white);
    temperatureDial.setJustificationType(juce::Justification::centred);
    temperatureDial.setText("Randomness", juce::dontSendNotification);

    addAndMakeVisible(inputFileBrowser);

    directoryList.setDirectory(juce::File::getSpecialLocation(juce::File::userHomeDirectory), true, true);

    inputFileBrowser.setDragAndDropDescription("draggedFromPlugin");
    inputFileBrowser.setColour(juce::FileTreeComponent::backgroundColourId, getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
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
    
    polyphonyRNN.generateHarmonization(temperature.getValue(), modelComboBox.getSelectedId());
}

//==============================================================================
void HarmonizationmachineAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); 
    g.setColour(juce::Colours::white);
    g.strokePath(path_parameter1, juce::PathStrokeType (0.5f));
    g.strokePath(path_parameter2, juce::PathStrokeType (0.5f));
    g.strokePath(path_parameter3, juce::PathStrokeType (0.5f));
}

void HarmonizationmachineAudioProcessorEditor::resized()
{
    juce::Rectangle<int> parameter1 = getLocalBounds().removeFromTop(320);
    juce::Rectangle<int> generateButtonArea = parameter1.removeFromBottom(40);
    juce::Rectangle<int> parameter2 = parameter1.removeFromRight(400);
    juce::Rectangle<int> parameter3 = parameter2.removeFromRight(200);
    juce::Rectangle<int> inputFileBrowserArea = getLocalBounds().removeFromBottom(230);

    float cornerSize = 5;
    path_parameter1.addRoundedRectangle(parameter1.getTopLeft().getX(),parameter1.getTopLeft().getY(),parameter1.getWidth(),
    parameter1.getHeight(),cornerSize);

    path_parameter2.addRoundedRectangle(parameter2.getTopLeft().getX(),parameter2.getTopLeft().getY(),parameter2.getWidth(),
    parameter2.getHeight(),cornerSize);

    path_parameter3.addRoundedRectangle(parameter3.getTopLeft().getX(),parameter3.getTopLeft().getY(),parameter3.getWidth(),
    parameter3.getHeight(),cornerSize);

    generateButton.setBounds(generateButtonArea);

    tempoDial.setBounds(parameter1);
    tempo.setBounds(parameter1);

    metronomeComponent.setBounds(parameter1.removeFromBottom(200).removeFromTop(80));

    modelLabel.setBounds(parameter2);
    modelComboBox.setBounds(parameter2.removeFromBottom(90).removeFromTop(30));

    networkComponent.setBounds(parameter2.removeFromBottom(110).removeFromTop(80));

    temperatureDial.setBounds(parameter3);
    temperature.setBounds(parameter3);

    diceComponent.setBounds(parameter3.removeFromBottom(200).removeFromTop(80));

    inputFileBrowser.setBounds(inputFileBrowserArea);
    explanation.setBounds(inputFileBrowserArea.removeFromRight(260).removeFromBottom(20));
}

void HarmonizationmachineAudioProcessorEditor::fileDoubleClicked(const juce::File &p_file)
{
    isFileDoubleClicked = true;
    explanation.setText("File "+p_file.getFileName()+" double clicked", juce::dontSendNotification);
    polyphonyRNN.setInputPath(p_file.getFullPathName().toStdString());
}

void HarmonizationmachineAudioProcessorEditor::selectionChanged() {}
void HarmonizationmachineAudioProcessorEditor::fileClicked(const juce::File &file, const juce::MouseEvent &) {}
void HarmonizationmachineAudioProcessorEditor::browserRootChanged(const juce::File &) {}

