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

    modelComboBox.addItem ("Bach chorales model", 1);
    modelComboBox.addItem ("Jazz model", 2);

    modelComboBox.setSelectedId(1);

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

    modelLabel.setSize(200, 50);
    modelLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    modelLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    modelLabel.setJustificationType(juce::Justification::centred);
    modelLabel.setText("Choose a machine learning model", juce::dontSendNotification);

    temperature.setRange(0.1, 5.0, 0.1);
    temperature.setValue(0.1);
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
}

void HarmonizationmachineAudioProcessorEditor::resized()
{
    juce::Rectangle<int> parameter1 = getLocalBounds().removeFromTop(320);
    juce::Rectangle<int> generateButtonArea = parameter1.removeFromBottom(40);
    juce::Rectangle<int> parameter2 = parameter1.removeFromRight(400);
    juce::Rectangle<int> parameter3 = parameter2.removeFromRight(200);
    juce::Rectangle<int> inputFileBrowserArea = getLocalBounds().removeFromBottom(300);
    generateButton.setBounds(generateButtonArea);

    tempoDial.setBounds(parameter1);
    tempo.setBounds(parameter1);

    modelLabel.setBounds(parameter2);
    modelComboBox.setBounds(parameter2);

    temperatureDial.setBounds(parameter3);
    temperature.setBounds(parameter3);

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
void HarmonizationmachineAudioProcessorEditor::fileClicked(const juce::File &file, const juce::MouseEvent &) 
{
    itemVector.push_back(new TreeViewItemComponent(inputFileBrowser.getSelectedItem(0), file));
}
void HarmonizationmachineAudioProcessorEditor::browserRootChanged(const juce::File &) {}

