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

    showHideFileBrowserButton.onClick = [this]() 
    { 
        if(showHideFileBrowserButtonClicked)
        {
            showHideFileBrowserButtonClicked = false;
            showHideFileBrowserButton.setButtonText("Show file browser");
            inputFileBrowser.setVisible(false);
            explanation.setVisible(false);
            setSize(600, 380); 
        }
        else
        {
            showHideFileBrowserButtonClicked = true;
            showHideFileBrowserButton.setButtonText("Hide file browser");
            inputFileBrowser.setVisible(true);
            explanation.setVisible(true);
            setSize(600, 550);
        }
    };

    tempo.onValueChange = [this]() { audioProcessor.setTempoFromEditor(tempo.getValue()); };

    modelComboBox.addItem("Bach chorales", 1);
    modelComboBox.addItem("Jazz", 2);

    modelComboBox.setSelectedId(1);

    metronomeComponent.setImage(metronome);
    diceComponent.setImage(dice);
    networkComponent.setImage(network);

    addAndMakeVisible(metronomeComponent);
    addAndMakeVisible(diceComponent);
    addAndMakeVisible(networkComponent);
    addAndMakeVisible(generateButton);
    addAndMakeVisible(tempo);
    addAndMakeVisible(tempoLabel);
    addAndMakeVisible(temperature);
    addAndMakeVisible(temperatureLabel);
    addAndMakeVisible(modelComboBox);
    addAndMakeVisible(modelLabel);
    addAndMakeVisible(primerKeyToggle);
    addAndMakeVisible(primerKeyComboBox);
    addAndMakeVisible(harmonizeToggle);
    addAndMakeVisible(numSteps);
    addAndMakeVisible(numStepsLabel);
    addAndMakeVisible(numOutputs);
    addAndMakeVisible(numOutputsLabel);
    addAndMakeVisible(showHideFileBrowserButton);

    tempo.setRange(10, 200, 1);
    tempo.setValue(120);
    tempo.setSliderStyle(juce::Slider::Rotary);
    tempo.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);

    tempoLabel.setSize(200, 30);
    tempoLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    tempoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    tempoLabel.setJustificationType(juce::Justification::centred);
    tempoLabel.setText("Tempo", juce::dontSendNotification);

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
    temperature.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);

    temperatureLabel.setSize(200, 30);
    temperatureLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    temperatureLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    temperatureLabel.setJustificationType(juce::Justification::centred);
    temperatureLabel.setText("Randomness", juce::dontSendNotification);
    
    primerKeyComboBox.addItem("Do M",1);
    primerKeyComboBox.addItem("La m",2);
    primerKeyComboBox.addItem("Sol m",3);
    primerKeyComboBox.addItem("Si b M",4);
    primerKeyComboBox.addItem("Do b M",5);
    primerKeyComboBox.addItem("Fa # M",6);
    primerKeyComboBox.addItem("Si m",7);

    primerKeyComboBox.setSelectedId(1);

    primerKeyComboBox.setEditableText(false);
    primerKeyComboBox.setJustificationType(juce::Justification::centred);

    numSteps.setRange(1, 1000, 1);
    numSteps.setValue(128);
    numSteps.setSliderStyle(juce::Slider::LinearHorizontal);
    numSteps.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 30);

    numStepsLabel.setSize(150, 30);
    numStepsLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    numStepsLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    numStepsLabel.setJustificationType(juce::Justification::centred);
    numStepsLabel.setText("Number of steps (Sixteenth)", juce::dontSendNotification);

    numOutputs.setRange(1, 10, 1);
    numOutputs.setValue(3);
    numOutputs.setSliderStyle(juce::Slider::LinearHorizontal);
    numOutputs.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 30);

    numOutputsLabel.setSize(150, 30);
    numOutputsLabel.setFont(juce::Font(14.0f, juce::Font::bold));
    numOutputsLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    numOutputsLabel.setJustificationType(juce::Justification::centred);
    numOutputsLabel.setText("Number of harmonizations", juce::dontSendNotification);

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

    //Generate harmonization
    polyphonyRNN.generateHarmonization(temperature.getValue(), modelComboBox.getSelectedId(), 
        primerKeyComboBox.getSelectedId(), primerKeyToggle.getToggleState(), harmonizeToggle.getToggleState(),
        numSteps.getValue(), numOutputs.getValue());
}

//==============================================================================
void HarmonizationmachineAudioProcessorEditor::paint(juce::Graphics &g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.strokePath(path_parameter1, juce::PathStrokeType(0.5f));
    g.strokePath(path_parameter2, juce::PathStrokeType(0.5f));
    g.strokePath(path_parameter3, juce::PathStrokeType(0.5f));
    g.strokePath(path_parameter4, juce::PathStrokeType(0.5f));
    g.strokePath(path_parameter5, juce::PathStrokeType(0.5f));
    g.strokePath(path_parameter6, juce::PathStrokeType(0.5f));
    g.strokePath(path_parameter7, juce::PathStrokeType(0.5f));
}

void HarmonizationmachineAudioProcessorEditor::resized() 
{
    juce::Rectangle<int> parameter1 = getLocalBounds().removeFromTop(340);
    juce::Rectangle<int> parameter4 = parameter1.removeFromBottom(110);
    juce::Rectangle<int> generateButtonArea = parameter4.removeFromBottom(40);

    juce::Rectangle<int> parameter2 = parameter1.removeFromRight(400);
    juce::Rectangle<int> parameter3 = parameter2.removeFromRight(200);

    float cornerSize = 5;
    path_parameter1.addRoundedRectangle(parameter1.getTopLeft().getX(), parameter1.getTopLeft().getY(), parameter1.getWidth(),
                                        parameter1.getHeight(), cornerSize);

    path_parameter2.addRoundedRectangle(parameter2.getTopLeft().getX(), parameter2.getTopLeft().getY(), parameter2.getWidth(),
                                        parameter2.getHeight(), cornerSize);

    path_parameter3.addRoundedRectangle(parameter3.getTopLeft().getX(), parameter3.getTopLeft().getY(), parameter3.getWidth(),
                                        parameter3.getHeight(), cornerSize);

    juce::Rectangle<int> parameter1label = parameter1.removeFromTop(30);
    juce::Rectangle<int> parameter2label = parameter2.removeFromTop(30);
    juce::Rectangle<int> parameter3label = parameter3.removeFromTop(30);

    juce::Rectangle<int> parameter5 = parameter4.removeFromRight(450);
    juce::Rectangle<int> parameter6 = parameter5.removeFromRight(300);
    juce::Rectangle<int> parameter7 = parameter6.removeFromRight(150);    
    
    juce::Rectangle<int> inputFileBrowserArea = getLocalBounds().removeFromBottom(170);

    juce::Rectangle<int> showHideFileBrowserButtonArea = getLocalBounds().removeFromTop(380).removeFromBottom(40);

    path_parameter4.addRoundedRectangle(parameter4.getTopLeft().getX(), parameter4.getTopLeft().getY(), parameter4.getWidth(),
                                        parameter4.getHeight(), cornerSize);

    path_parameter5.addRoundedRectangle(parameter5.getTopLeft().getX(), parameter5.getTopLeft().getY(), parameter5.getWidth(),
                                        parameter5.getHeight(), cornerSize);

    path_parameter6.addRoundedRectangle(parameter6.getTopLeft().getX(), parameter6.getTopLeft().getY(), parameter6.getWidth(),
                                        parameter6.getHeight(), cornerSize);

    path_parameter7.addRoundedRectangle(parameter7.getTopLeft().getX(), parameter7.getTopLeft().getY(), parameter7.getWidth(),
                                        parameter7.getHeight(), cornerSize);

    generateButton.setBounds(generateButtonArea);

    showHideFileBrowserButton.setBounds(showHideFileBrowserButtonArea);

    tempoLabel.setBounds(parameter1label);
    tempo.setBounds(parameter1);

    metronomeComponent.setBounds(parameter1.removeFromBottom(155).removeFromTop(80));

    modelLabel.setBounds(parameter2label);
    modelComboBox.setBounds(parameter2.removeFromBottom(30));

    networkComponent.setBounds(parameter2.removeFromBottom(130).removeFromTop(80));

    temperatureLabel.setBounds(parameter3label);
    temperature.setBounds(parameter3);

    diceComponent.setBounds(parameter3.removeFromBottom(160).removeFromTop(80));

    primerKeyToggle.setBounds(parameter4.removeFromTop(30).removeFromLeft(120).removeFromRight(90));
    primerKeyComboBox.setBounds(parameter4.removeFromBottom(30));

    harmonizeToggle.setBounds(parameter5.removeFromLeft(130).removeFromRight(110));

    numSteps.setBounds(parameter6.removeFromBottom(30));
    numStepsLabel.setBounds(parameter6.removeFromTop(30));

    numOutputs.setBounds(parameter7.removeFromBottom(30));
    numOutputsLabel.setBounds(parameter7.removeFromTop(30));

    inputFileBrowser.setBounds(inputFileBrowserArea);
    explanation.setBounds(inputFileBrowserArea.removeFromRight(260).removeFromBottom(20));
}

void HarmonizationmachineAudioProcessorEditor::fileDoubleClicked(const juce::File &p_file)
{
    isFileDoubleClicked = true;
    explanation.setText("File " + p_file.getFileName() + " double clicked", juce::dontSendNotification);
    polyphonyRNN.setInputPath(p_file.getFullPathName().toStdString());
}

void HarmonizationmachineAudioProcessorEditor::selectionChanged() {}
void HarmonizationmachineAudioProcessorEditor::fileClicked(const juce::File &file, const juce::MouseEvent &) {}
void HarmonizationmachineAudioProcessorEditor::browserRootChanged(const juce::File &) {}
